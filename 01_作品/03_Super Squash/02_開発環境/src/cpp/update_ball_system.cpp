//===================================================
//
// ボールの更新処理 [update_ball_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_ball_system.h"
#include "velocity_component.hpp"
#include "transform_component.hpp"
#include "tag_component.hpp"
#include "ball_component.hpp"
#include "velocity_component.hpp"
#include "ball_field_collision_component.hpp"
#include "utility_math.h"
#include "mesh_wall_collision_component.hpp"
#include "mesh_effect_manager.h"
#include "player_component.hpp"
#include "parent_component.hpp"
#include "collision.h"
#include "sphere_collider_component.hpp"
#include "capsule_collider_component.hpp"
#include "factory_billboard.h"
#include "motion_manager.h"
#include "motion_component.hpp"
#include "factory_mesh.h"
#include "factory_model.h"
#include "character_model_component.hpp"
#include "move_to_rotation_component.hpp"
#include "character_component.hpp"
#include "character_collision_spec.h"
#include "mesh_vtx_component.hpp"
#include "color_constants.hpp"
#include "ball_constants.hpp"
#include "effect_constants.hpp"
#include "game_controller.h"
#include "hitstop_controller.h"
#include "camera_shake.h"
#include "effect_component.hpp"
#include "manager.h"
#include "particle_helper.h"
#include "character_constants.hpp"
#include "character_motion_spec.h"
#include "mesh_sphere_component.hpp"
#include "ball_spec.h"
#include "sound.h"
#include "explotion_manager.h"
#include "score_manager.h"
#include "motion_blur_component.hpp"


#ifdef _DEBUG
#include "line.h"
#include "debug_proc.h"

#endif // _DEBUG
#include "size_component.hpp"
#include "mesh_wall_component.hpp"
#include "spec_combinators.h"
#include "collision_effect_wall_component.hpp"
#include "minigame_player_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace UpdateBallConst
{
	constexpr int NUM_EXPLOSION				= 8;		// 煙の数
	constexpr int WAVE_TIME					= 30;		// メッシュの波の時間
	constexpr float EXPLOSION_MOVE_SPEED	= 2.0f;		// 煙を出す移動量
	constexpr float WAVE_INRADIUS			= 0.0f;		// メッシュの波の内側の半径
	constexpr float WAVE_OUTRADIUS			= 100.0f;	// メッシュの波の外側の半径
	constexpr float WAVE_HEIGHT_OFFSET		= 50.0f;	// メッシュの波の高さのオフセット
	constexpr float WAVE_SPEED				= 5.0f;		// メッシュの波の速さ
	constexpr float WAVE_COEF				= 0.01f;	// メッシュの波の係数
	constexpr float SHOW_MOTION_BLUR_SPEED	= 40.0f;	// モーションブラーを表示する速度
}

//===================================================
// 更新処理
//===================================================
void UpdateBallSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::BallTag>();

	// ボールの数分回す
	for (auto ball : view)
	{
		// コンポーネントを取得
		auto& ballTransformComp = registry.get<Transform3DComponent>(ball);
		auto& ballComp = registry.get<BallComponent>(ball);
		auto& fieldCollisionComp = registry.get <BallFieldCollisionComponent > (ball);
		auto& wallCollisionComp = registry.get<MeshWallCollisionComponent>(ball);
		auto& velocityComp = registry.get<VelocityComponent>(ball);

		// インターバル中の設定処理
		SetInterVal(registry, ball);

		// 着地していたら
		if (fieldCollisionComp.bLanding)
		{
			// 反射方向の移動量を取得
			velocityComp.move = math::Bound(velocityComp.move, fieldCollisionComp.normal, 1.0f);

			// yの移動量が低いなら煙を出さない
			if (fabsf(velocityComp.move.y) >= UpdateBallConst::EXPLOSION_MOVE_SPEED)
			{
				ExplotionManager::BallLandingExplotion(registry, ballTransformComp.pos, UpdateBallConst::NUM_EXPLOSION, D3DXVECTOR3(velocityComp.fMoveLength * 0.1f, 0.2f, velocityComp.fMoveLength * 0.1f));
			}

			fieldCollisionComp.bLanding = false;
		}

		// エフェクト用の壁に当たった時の処理
		SetCollisionEffectWall(registry, ball);

		// 壁に当たった時の処理
		SetCollisionWall(registry, ball, wallCollisionComp, ballComp);

		wallCollisionComp.bCollision = false;

		D3DXVECTOR3 dir;

		// 球が打つ力
		float fPower = 0.0f;

		// メッシュの頂点のコンポーネント
		auto& meshSphereComp = registry.get<MeshSphereComponent>(ball);

		// ボールのメッシュの高さを移動速度に応じて伸ばす
		meshSphereComp.fHeight = meshSphereComp.fRadius + (BallConstants::MAX_HEIGHT * (velocityComp.fMoveLength / BallConstants::MOVE_THRESHOLD_LEVEL_MAX));

		// ラケットの当たり判定
		if (CollisionRacket(registry, ball, dir, fPower))
		{
			velocityComp.move.x = sinf(dir.y) * -velocityComp.fMoveLength;
			velocityComp.move.y = sinf(dir.x) * velocityComp.fMoveLength;
			velocityComp.move.z = cosf(dir.y) * -velocityComp.fMoveLength;

			// 移動量の設定
			velocityComp.move.x += sinf(dir.y) * -fPower;
			velocityComp.move.y += sinf(dir.x) * fPower;
			velocityComp.move.z += cosf(dir.y) * -fPower;

			// 下で発射速度による処理をするためここで算出
			velocityComp.fMoveLength = D3DXVec3Length(&velocityComp.move);

			// ヒットストップのエフェクトの設定処理
			SetHitStopEffect(registry, ball, ballTransformComp);

			// バウンドしていない
			ballComp.bBound = false;

			// 音の取得
			CSound* pSound = CManager::GetInstance()->GetSound();

			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_NORMAL_SHOT);
			}
		}

		// 軌跡の頂点の取得
		auto& orbitVtx = registry.get<MeshVtxComponent>(ballComp.orbitID);

		// エフェクトの設定
		SetEffect(registry, ball, velocityComp, orbitVtx, ballTransformComp);

		// 最大の移動速度
		const float maxSpeed = BallConstants::MOVE_THRESHOLD_LEVEL_MAX;

		// 移動量、移動スピードを制限
		ballComp.fSpeed = math::Clamp(ballComp.fSpeed, 0.0f, maxSpeed);
		velocityComp.move.x = math::Clamp(velocityComp.move.x, -maxSpeed, maxSpeed);
		velocityComp.move.y = math::Clamp(velocityComp.move.y, -maxSpeed, maxSpeed);
		velocityComp.move.z = math::Clamp(velocityComp.move.z, -maxSpeed, maxSpeed);

		// 移動速度によってモーションブラーの表示を切り替える処理
		UpdateMotionBlurBySpeed(registry, ball, velocityComp);
	}
}

//===================================================
// インターバル中の設定処理
//===================================================
void UpdateBallSystem::SetInterVal(entt::registry& registry, entt::entity ball)
{
	// ボールの取得
	auto& ballComp = registry.get<BallComponent>(ball);
	auto& transformComp = registry.get<Transform3DComponent>(ball);

	// ゲームのコントローラーを取得
	CGameController* pGameController = CGameController::GetInstance();

	// ヒットストップの取得
	CHitStopController* pHitStopController = pGameController->GetHitStop();

	BallSpec::CInterval is_interval; // インターバルがあるなら

	if (pGameController != nullptr)
	{
		// インターバルがあるかつ、ヒットストップしていないなら
		if (is_interval.IsSatisfiedBy(registry, ball) && !pHitStopController->IsStop())
		{
			ParticleComponent::Param particle;

			// パーティクルの補助クラスの取得
			CParticleHelper* pParticleHelper = CManager::GetInstance()->GetParticleHelper();

			// パラメータの取得
			pParticleHelper->GetParam(particle, transformComp.pos, "Ball_Smoke", EffectComponent::FLAG_DEC_ALPHA);

			// パーティクルの生成
			auto parcileID = FactoryBillboard::Create::Particle(registry, particle);
			FactoryBillboard::Build::AnimationParticle(registry, parcileID, 2, false, 4, 3);

			// インターバルを減らす
			ballComp.nInterval--;
		}
	}
}

//===================================================
// 壁に当たったときの処理
//===================================================
void UpdateBallSystem::SetCollisionWall(entt::registry& registry, const entt::entity ball, MeshWallCollisionComponent& collisionComp, BallComponent& ballComp)
{
	// 当たってないなら
	if (!collisionComp.bCollision)
	{
		return;
	}

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_SE_BOUND);
	}

	// 移動量のコンポーネントの取得
	auto& velocityComp = registry.get<VelocityComponent>(ball);
	auto& trasformComp = registry.get<Transform3DComponent>(ball);

	// バウンドした
	ballComp.bBound = true;

	// 反射方向の移動量を取得
	velocityComp.move = math::Bound(velocityComp.move, collisionComp.normal, ballComp.fRestitution);

	collisionComp.bCollision = false;

	BallSpec::CSpeedLevelAboveMedium is_SpeedMedium; // 速さがMedium以上なら

	// IDが無いなら
	if (!registry.valid(collisionComp.meshID))
	{
		return;
	}

	// 速度がmedium以上だったら
	if (is_SpeedMedium.IsSatisfiedBy(registry,ball))
	{
		// パラメータの設定
		MeshWaveComponent::Param param = {};

		param.epicenter = trasformComp.pos;
		param.fInRadius = UpdateBallConst::WAVE_INRADIUS;
		param.fOutRadius = UpdateBallConst::WAVE_OUTRADIUS;
		param.fHeight = (velocityComp.fMoveLength + UpdateBallConst::WAVE_HEIGHT_OFFSET);
		param.fSpeed = UpdateBallConst::WAVE_SPEED;
		param.fCoef = UpdateBallConst::WAVE_COEF;
		param.nTime = UpdateBallConst::WAVE_TIME;

		// 波の生成
		MeshEffectManager::CreateWave(param, MeshWaveComponent::Type::MULT_XY, registry, collisionComp.meshID, Color::WHITE);
	}

	BallSpec::CSpeedLevelAboveSmall	 is_SpeedSmall;	// 速さがSmall以上なら

	collisionComp.meshID = entt::null;
}

//===================================================
// エフェクトの壁の当たり判定処理
//===================================================
void UpdateBallSystem::SetCollisionEffectWall(entt::registry& registry, const entt::entity ball)
{
	// エフェクトの壁の当たり判定の取得
	auto& effectWallCollisionComp = registry.get<CollisionEffectWallComponent>(ball);

	// 使えないIDだったら
	if (!registry.valid(effectWallCollisionComp.wallID))
	{
		return;
	}

	// 当たっていたら
	if (effectWallCollisionComp.bCollision)
	{
		MeshEffectManager::CreateEffect(registry, effectWallCollisionComp.wallID, effectWallCollisionComp.hitPos, 70.0f, 0.5f, 20);
		effectWallCollisionComp.bCollision = false;
	}
}

//===================================================
// ラケットの当たり判定
//===================================================
bool UpdateBallSystem::CollisionRacket(entt::registry& registry, const entt::entity ball, D3DXVECTOR3& OutDir, float& fOutShotPower)
{
	auto ballColliderID = registry.view<ColliderTag::BallSphere>().front();
	
	// ボールのコンポーネントの取得
	auto& sphereComp = registry.get<SphereColliderComponent>(ballColliderID);
	auto& ballComp = registry.get<BallComponent>(ball);
	auto& velocityComp = registry.get<VelocityComponent>(ball);

	// キャラクターの取得
	for (auto character : registry.view<Tag::CharacterTag>())
	{
		// コンポーネントの取得
		auto& childrenComp = registry.get<ChildrenComponent>(character);
		auto& characterComp = registry.get<CharacterComponent>(character);
		auto& characterTransform = registry.get<Transform3DComponent>(character);
		auto racketID = childrenComp.aChildrenID[CharacterModelComponent::MODEL_RACKET];
		auto& capsuleComp = registry.get<CapsuleColliderComponent>(racketID);

		CharacterSpec::Collision::CBallRacket	is_BallCollSpec;	// キャラクターのラケットがボールと当たったかどうか
		BallSpec::CInterval						is_interval;		// インターバルがあるかどうか

		// ラケットに当たったら&& インターバルがないなら
		if (Collision::Capsule::CollisionSphere(registry, capsuleComp, sphereComp) &&
			is_BallCollSpec.IsSatisfiedBy(registry, character) &&
			CNotSpec(is_interval).IsSatisfiedBy(registry, ball))
		{
			// 移動量をintに変換
			int nMoveLength = static_cast<int>(velocityComp.fMoveLength);

			ScoreManager::AddScore(registry, ScoreManager::BASE_SCORE * nMoveLength);

			// 発射方向の設定
			OutDir = SetShotAngle(registry, character, characterTransform);

			// ボールのインターバルの設定
			ballComp.nInterval = BallConstants::INTERVAVL;

			// 発射力を設定
			fOutShotPower = characterComp.fShotPower;
			
			// ミニゲームのプレイヤーの取得
			auto pMinigamePlayerComp = registry.try_get<MinigamePlayerComponent>(character);

			// 取得できないなら処理しない
			if (pMinigamePlayerComp == nullptr) return true;

			// コンボを加算
			pMinigamePlayerComp->nCombo++;

			// コンボをリセットするまでの時間
			pMinigamePlayerComp->nComboResetTime = pMinigamePlayerComp->COMBO_RESET_TIME;

			return true;
		}
	}
	return false;
}

//===================================================
// 発射角度の設定
//===================================================
D3DXVECTOR3 UpdateBallSystem::SetShotAngle(entt::registry& registry, const entt::entity character, Transform3DComponent& characterTransform)
{
	auto& motionComp = registry.get<MotionComponent>(character);
	auto& characterComp = registry.get<CharacterComponent>(character);

	// 計算後の角度を返すための変数
	D3DXVECTOR3 outAngle = Const::VEC3_NULL;

	// 上下の発射角度の設定
	outAngle.x = characterComp.shotAngle.x;
	outAngle.y = characterComp.shotAngle.y;

	return outAngle;
}

//===================================================
// エフェクトの設定処理
//===================================================
void UpdateBallSystem::SetEffect(entt::registry& registry, const entt::entity ball, VelocityComponent& velocityComp, MeshVtxComponent& meshVtxComp, Transform3DComponent& transformComp)
{
	BallSpec::CSpeedLevelAboveSmall	 is_SpeedSmall;	// 速さがSmall以上なら
	BallSpec::CSpeedLevelAboveMedium is_SpeedMedium; // 速さがMedium以上なら

	// Small以下だったら
	if (CNotSpec(is_SpeedSmall).IsSatisfiedBy(registry, ball))
	{
		meshVtxComp.col = Color::WHITE;
	}
	// レベルSmall以上～Medium未満だったら
	else if (CAndSpec(is_SpeedSmall, CNotSpec(is_SpeedMedium)).IsSatisfiedBy(registry, ball))
	{
		ParticleComponent::Param particle;

		// パーティクルの補助クラスの取得
		CParticleHelper* pParticleHelper = CManager::GetInstance()->GetParticleHelper();

		// パラメータの取得
		pParticleHelper->GetParam(particle, transformComp.pos, "ballSpeedLevel_2", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);

		// パーティクルの生成
		FactoryBillboard::Create::Particle(registry, particle);

		meshVtxComp.col = Color::AQUA;
	}
	// レベルMedium以上だったら
	else if (is_SpeedMedium.IsSatisfiedBy(registry, ball))
	{
		ParticleComponent::Param particle;

		// パーティクルの補助クラスの取得
		CParticleHelper* pParticleHelper = CManager::GetInstance()->GetParticleHelper();

		// パラメータの取得
		pParticleHelper->GetParam(particle, transformComp.pos, "ballSpeedLevel_3", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);

		// パーティクルの生成
		FactoryBillboard::Create::Particle(registry, particle);

		meshVtxComp.col = Color::MAGENTA;
	}

	// 移動速度がSmall1以上だったら
	if (is_SpeedSmall.IsSatisfiedBy(registry, ball))
	{
		// 移動方向
		D3DXVECTOR3 vecMove = velocityComp.move;
		D3DXVec3Normalize(&vecMove, &vecMove);

		// 横方向のベクトルを出す
		D3DXVECTOR3 axis = math::Cross(Const::VEC_U, vecMove);
		
		// 横方向ベクトルを回転軸にして、回転行列を生成して、つむじの方向を進行方向に向ける
		D3DXQuaternionRotationAxis(&transformComp.quaternion, &axis, acosf(math::Dot(Const::VEC_U, vecMove)));
	}

	// ゲームのコントローラーの取得
	CGameController *pGameController = CGameController::GetInstance();
	if (pGameController != nullptr)
	{
		// 止まっていないなら
		if (!pGameController->GetHitStop()->IsStop())
		{
			// 処理を飛ばす
			return;
		}

		D3DXVECTOR3 effectPos = Const::VEC3_NULL;

		// ボールの位置から範囲内に位置を設定
		effectPos.x = transformComp.pos.x + math::Random(-150.0f, 150.0f);
		effectPos.y = transformComp.pos.y + math::Random(-150.0f, 150.0f);
		effectPos.z = transformComp.pos.z + math::Random(-150.0f, 150.0f);

		// エフェクトの生成
		auto effect = FactoryBillboard::Create::Effect(registry, effectPos, Color::ORANGERED, 8.0f, EffectConstants::TEXTURE_NORMAL);
		FactoryBillboard::Build::Effect(registry, effect, 60, math::GetVector(transformComp.pos,effectPos) * 5.0f,
			EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);
	}
}

//===================================================
// 発射のアニメーションの設定
//===================================================
void UpdateBallSystem::SetHitStopEffect(entt::registry& registry, const entt::entity ball, Transform3DComponent& transformComp)
{
	BallSpec::CSpeedLevelAboveMedium	is_speedAboveMidium;	// ボールの速度の判定
	BallSpec::CSpeedLevelAboveSmall		is_speedAboveSamll;		// ボールの速度の判定
	BallSpec::CInterval					is_interval;			// インターバルがあるなら

	// ゲームのコントローラーを取得
	CGameController* pGameController = CGameController::GetInstance();

	// 発射速度を判定
	if (is_speedAboveSamll.IsSatisfiedBy(registry, ball) && CNotSpec(is_speedAboveMidium).IsSatisfiedBy(registry, ball))
	{
		// ヒットストップ
		CHitStopController* pHitStopController = pGameController->GetHitStop();

		// 取得できないなら
		if (pHitStopController == nullptr) return;

		// ヒットストップの開始
		pHitStopController->Begin(20);

		// 音の取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_SHOT_MEDIUM);
		}
	}

	// 発射速度を判定
	if (is_speedAboveMidium.IsSatisfiedBy(registry, ball))
	{
		// ヒットストップ、カメラの揺れ
		CCameraShake* pCameraShake = CGameController::GetInstance()->GetCameraShake();
		CHitStopController* pHitStopController = pGameController->GetHitStop();

		// 取得できないなら
		if (pCameraShake == nullptr || pHitStopController == nullptr) return;

		// カメラの揺れの開始
		pCameraShake->Begin(2.0f, 60);

		// ヒットストップの開始
		pHitStopController->Begin(60);

		// 音の取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_SHOT_MEDIUM);
		}

		// エフェクトの生成処理の登録
		auto Effect = [&](void)
		{
			// 音の取得
			CSound* pSound = CManager::GetInstance()->GetSound();

			// 再生できたなら
			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_MEDIUM_SHOT_MOVE);
			}

			ParticleComponent::Param particle;

			// パーティクルの補助クラスの取得
			CParticleHelper* pParticleHelper = CManager::GetInstance()->GetParticleHelper();

			// パラメータの取得
			pParticleHelper->GetParam(particle, transformComp.pos, "Ball_Shot_000", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);

			// パーティクルの生成
			FactoryBillboard::Create::Particle(registry, particle);

			// メッシュのサークルの生成
			auto circle = FactoryMesh::Create::Circle(registry, transformComp.pos, { D3DX_PI * 0.5f,0.0f ,0.0f },
				D3DXCOLOR(Color::WHITE.r, Color::WHITE.g, Color::WHITE.b, 0.5f), "");

			FactoryMesh::Build::Circle(registry, circle, 8.0f, 0.0f, 40.0f, 20);
			FactoryMesh::Build::Circle(registry, circle, 25.0f);
		};

		// ヒットストップのイベント
		pHitStopController->AddFinishEvent(Effect);
	}
}

//===================================================
// 速度によってモーションブラーを設定する関数
//===================================================
void UpdateBallSystem::UpdateMotionBlurBySpeed(entt::registry& registry, const entt::entity ball, const VelocityComponent& velocityComp)
{
	// コンポーネントを持っていないなら
	if (!registry.any_of<MotionBlurComponent>(ball))
	{
		return;
	}

	// コンポーネントの取得
	auto& motionBlurComp = registry.get<MotionBlurComponent>(ball);

	// 移動した幅
	float fMoveLength = velocityComp.fMoveLength;

	// モーションブラーを表示する速度に達したか判定
	const bool bShowMotionBlur = fMoveLength >= BallConstants::MOVE_THRESHOLD_LEVEL_SMALL;

	// モーションブラーの設定
	motionBlurComp.bShow = bShowMotionBlur;
}