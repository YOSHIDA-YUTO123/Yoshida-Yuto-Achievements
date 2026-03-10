//===================================================
//
// プレイヤー制御処理 [player_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player_system.h"
#include "tag_component.hpp"
#include "velocity_component.hpp"
#include "manager.h"
#include "input.h"
#include "player_component.hpp"
#include "character_component.hpp"
#include "motion_component.hpp"
#include "motion_manager.h"
#include "player_constants.hpp"
#include "field_collision_component.hpp"
#include "collision.h"
#include "transform_component.hpp"
#include "player_spec.h"
#include "spec_combinators.h"
#include "character_motion_spec.h"
#include "character_transform_spec.h"
#include "camera.h"
#include "utility_math.h"
#include "game_controller.h"
#include "hitstop_controller.h"
#include "character_constants.hpp"
#include "player_command_component.hpp"
#include "collision_effect_wall_component.hpp"
#include "mesh_effect_manager.h"
#include "mesh_wall_collision_component.hpp"
#include "sound.h"
#include "character_collision_spec.h"

#ifdef _DEBUG
#include "debug_proc.h"
#include "line.h"
#include "color_constants.hpp"
#endif // _DEBUG

//***************************************************
// 定数宣言
//***************************************************
namespace PlayerSystemConst
{
	constexpr float WALL_EFFECT_RADIUS	= 50.0f;	// 壁のエフェクトの半径
	constexpr float WALL_EFFECT_ROUGH	= 0.9f;		// 壁のエフェクトの粗さ
	constexpr int WALL_EFFECT_TIME		= 20;		// 壁のエフェクトの発生時間
}

//===================================================
// 移動の更新処理
//===================================================
void PlayerSystem::Update(entt::registry& registry)
{
	// ゲームを操作するクラスの取得
	auto pGameController = CGameController::GetInstance();

	// ヒットストップしているときにHitStop対象のentityだったら
	if (pGameController != nullptr)
	{
		// ヒットストップしているなら処理を飛ばす
		if (pGameController->GetHitStop()->IsStop())
		{
			return;
		}
	}

	// 指定したエンティティの取得
	auto view = registry.view<Tag::PlayerTag>();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	// 取得できなかったら処理しない
	if (pKeyboard == nullptr) return;
	if (pJoyPad == nullptr) return;

	// player分更新
	for (auto player : view)
	{
		// 移動量の取得
		auto& velocityComp = registry.get<VelocityComponent>(player);

		// モーションのコンポーネントの取得
		auto& motionComp = registry.get<MotionComponent>(player);
		
		// キャラクターのコンポ―ネント
		auto& characterComp = registry.get<CharacterComponent>(player);

		auto& fieldCollisionComp = registry.get<FieldCollisionComponent>(player);

		// キーボードの移動
		InputMove(registry, player, pKeyboard, pJoyPad, motionComp, velocityComp);

		// ボールの発射角度の設定
		SetShotPitchAngle(registry, player, characterComp, pKeyboard, pJoyPad);

		// ショットかサーブのモーションの再生処理
		StartShotOrServeMotion(registry, player, motionComp, characterComp);
		
		// エフェクトの壁との当たり判定
		SetCollisionEffectWall(registry, player);

		// ジャンプ中で地面に着地していたら
		if (MotionManager::Check::Type(motionComp, MotionComponent::MOTIONTYPE_JUMP) && fieldCollisionComp.bLanding)
		{
			// モーションの再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_LANDING, 10, true);
		}

		// 範囲制限
		characterComp.fShotPower = math::Clamp(characterComp.fShotPower, 0.0f, 100.0f);			
	}
}

//===================================================
// キーボードの移動
//===================================================
void PlayerSystem::InputMove(entt::registry& registry, const entt::entity player, CInputKeyboard* pKeyboard, CInputJoypad* pJoypad, MotionComponent& motionComp, VelocityComponent& velocityComp)
{
	// キャラクターのコンポーネントの取得
	auto& characterComp = registry.get<CharacterComponent>(player);

	// プレイヤーのコマンドの取得
	auto& playerCommand = registry.get<PlayerCommandComponent>(player);

	bool bMove = false;

	CharacterSpec::Motion::CBlowOff					isBlowOff;			// 吹き飛びモーション
	CharacterSpec::Motion::CJetPackMoveLeft			isJetpackMoveLeft;	// ジェットパックの素早い左移動か判定
	CharacterSpec::Motion::CJetPackMoveRight		isJetpackMoveRight;	// ジェットパックの素早い右移動か判定

	// ショットモーションの判定
	CharacterSpec::Motion::CCheckShot				isShot;	// ショットモーションかどうか
	CharacterSpec::Motion::CCharge					isCharge;	// チャージモーションかどうか

	// 吹き飛び,ジェットパックの素早い移動だったら
	if (COrSpec(isBlowOff, isJetpackMoveLeft).IsSatisfiedBy(registry, player) ||
		isJetpackMoveRight.IsSatisfiedBy(registry, player))
	{
		return;
	}

	// キーボードの移動処理
	if (KeyboardMove(registry, player, pKeyboard, pJoypad, velocityComp))
	{
		bMove = true;
	}
	// パッドの移動処理
	else if (JoyPadMove(registry, player, pJoypad, velocityComp))
	{
		bMove = true;
	}

	CharacterSpec::Transform::CBallOnLeftSide		isballOnLeftSideSpec;	// ボールがキャラクターの左にいるかどうか
	CharacterSpec::Motion::CJetPack					isJetpack;				// ジェットパックモーションかどうか

	// 移動しているなら
	if (bMove)
	{
		bool bPlayMoveMotion = false;

		// ジェットパックモーションとチャージモーションなら
		COrSpec isJetOrCharge = { isJetpack, isCharge };

		// 移動モーションを出せるなら
		if (CNotSpec(isShot).IsSatisfiedBy(registry, player) &&
			CNotSpec(isJetOrCharge).IsSatisfiedBy(registry,player))
		{
			// 移動状態にする
			characterComp.state = CharacterComponent::State::Move;

			// 移動モーションが発動できる
			bPlayMoveMotion = true;
		}

		// 移動モーションを発動できないなら
		if (!bPlayMoveMotion) return;
		
		// 右移動
		if (playerCommand.moveDir == PlayerCommandComponent::MoveDir::Right)
		{
			// モーションの再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE, 10, true);
		}
		else if (playerCommand.moveDir == PlayerCommandComponent::MoveDir::Left)
		{
			// モーションの再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE, 10, true);
		}
		else
		{
			// モーションの再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_MOVE, 10, true);
		}

		return;
	}

	// 移動状態にする
	characterComp.state = CharacterComponent::State::Normal;
	
	// 移動モーションからニュートラルにできるか
	CharacterSpec::Motion::CMoveToNeutral isMoveToNeutral;

	// 移動モーションじゃないなら
	if (CNotSpec(isMoveToNeutral).IsSatisfiedBy(registry, player)) return;
	
	// ボールがプレイヤーの左側にあるなら
	if (isballOnLeftSideSpec.IsSatisfiedBy(registry, player))
	{
		// モーションの再生
		MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_NEUTRAL_LEFT_HAND, 10, true);

		return;
	}

	// モーションの再生
	MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_NEUTRAL, 10, true);
	
}

//===================================================
// キーボードの移動処理
//===================================================
bool PlayerSystem::KeyboardMove(entt::registry& registry, const entt::entity player, CInputKeyboard* pKeyboard, CInputJoypad* pJoypad, VelocityComponent& velocityComp)
{
	// パッドが移動しているなら
	if (pJoypad->GetJoyStickL())
	{
		return false;
	}

	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 取得できないなら処理しない
	if (pCamera == nullptr) return false;

	// カメラの向きの取得
	D3DXVECTOR3 cameraRot = pCamera->GetRotation();

	// キャラクターのコンポーネントの取得
	auto& characterComp = registry.get<CharacterComponent>(player);

	// プレイヤーのコマンドの取得
	auto& playerCommand = registry.get<PlayerCommandComponent>(player);

	bool bMove = false;

	const float fSpeed = characterComp.fSpeed;

	if (pKeyboard->GetPress(DIK_A))
	{
		velocityComp.move.x -= fSpeed;

		//// 目的の向き
		//characterComp.fRotDest = fCameraRotY + D3DX_PI * 0.5f;

		// 目的の向き
		characterComp.fRotDest = cameraRot.y + D3DX_PI;

		// 左移動
		playerCommand.moveDir = PlayerCommandComponent::MoveDir::Left;

		bMove = true;
	}
	//プレイヤーの移動(右)
	else if (pKeyboard->GetPress(DIK_D))
	{
		velocityComp.move.x += fSpeed;

		// 右移動
		playerCommand.moveDir = PlayerCommandComponent::MoveDir::Right;

		// 目的の向き
		characterComp.fRotDest = cameraRot.y + D3DX_PI;

		//// 目的の向き
		//characterComp.fRotDest = fCameraRotY - D3DX_PI * 0.5f;

		bMove = true;
	}

	return bMove;
}

//===================================================
// ジョイパッドの移動
//===================================================
bool PlayerSystem::JoyPadMove(entt::registry& registry, const entt::entity player, CInputJoypad* pJoypad, VelocityComponent& velocityComp)
{
	bool bMove = false;

	// キャラクターのコンポーネントの取得
	auto& characterComp = registry.get<CharacterComponent>(player);

	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	XINPUT_STATE* pStick = pJoypad->GetJoyStickAngle();

	// 取得できなかったら処理しない
	if (pCamera == nullptr) return false;

	// カメラの向き
	D3DXVECTOR3 cameraRot = pCamera->GetRotation();

	// Lスティックの角度
	float LStickAngleY = pStick->Gamepad.sThumbLY;
	float LStickAngleX = pStick->Gamepad.sThumbLX;

	// デッドゾーン
	float deadzone = 32767.0f * 0.25f;

	// スティックの傾けた角度を求める
	float magnitude = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

	// 動かせる
	if (magnitude > deadzone)
	{
		bMove = true;

		// アングルを正規化
		float normalizeX = (LStickAngleX / magnitude);
		float normalizeY = (LStickAngleY / magnitude);

		// プレイヤーの移動量
		float moveX = normalizeX * cosf(-cameraRot.y) - normalizeY * sinf(-cameraRot.y);
		float moveZ = normalizeX * sinf(-cameraRot.y) + normalizeY * cosf(-cameraRot.y);

		// 移動量をスティックの角度によって変更
		float speedWk = characterComp.fSpeed * ((magnitude - deadzone) / (32767.0f - deadzone));

		// 計算用移動量
		D3DXVECTOR3 moveWk = velocityComp.move;

		// プレイヤーの移動
		velocityComp.move.x += moveX * speedWk;
		velocityComp.move.z += moveZ * speedWk;

		// 目的の向き
		characterComp.fRotDest = cameraRot.y + D3DX_PI;

		// プレイヤーの角度を移動方向にする
		float fDestAngle = atan2f(-moveX, -moveZ);

		PlayerSec::CJoyPadMoveLeft	is_LeftMove;	// 左移動かどうか
		PlayerSec::CJoyPadMoveRight is_RightMove;	// 右移動かどうか

		// プレイヤーのコマンドの取得
		auto& playerCommand = registry.get<PlayerCommandComponent>(player);

		// プレイヤーのコンポーネントの取得
		auto& playerComp = registry.get<PlayerComponent>(player);

		// パッドの移動方向の設定
		playerComp.fPadMoveAngle = fDestAngle;

		// 左移動だったら
		if (is_LeftMove.IsSatisfiedBy(registry, player))
		{
			playerCommand.moveDir = PlayerCommandComponent::MoveDir::Left;
		}
		else if (is_RightMove.IsSatisfiedBy(registry, player))
		{
			playerCommand.moveDir = PlayerCommandComponent::MoveDir::Right;
		}
		else
		{
			playerCommand.moveDir = PlayerCommandComponent::MoveDir::Forward;
		}
	}

	return bMove;

}

//===================================================
// サーブモーションかショットモーションの再生処理
//===================================================
void PlayerSystem::StartShotOrServeMotion(entt::registry& registry, const entt::entity player, MotionComponent& motionComp, CharacterComponent& characterComp)
{
	CharacterSpec::Motion::CBlowOff	is_BlowOff;	// プレイヤーが吹き飛ばされているかどうか

	// 吹き飛び中なら処理を飛ばす
	if (is_BlowOff.IsSatisfiedBy(registry, player))
	{
		return;
	}

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	// 取得できなかったら処理しない
	if (pKeyboard == nullptr) return;
	if (pJoyPad == nullptr) return;

	// プレイヤーの定数の使用
	using namespace PlayerConstants;

	// プレイヤーのコンポーネントの取得
	auto& playerComp = registry.get<PlayerComponent>(player);

	CharacterSpec::Motion::CCharge	is_chargeMotion;		// チャージモーションかどうか

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// チャージキーを離したら
	ReleaseChargeKey(registry, player, motionComp, characterComp);

	CharacterSpec::Collision::CShotRange		is_shotRange;			// ボールを打てる範囲にいるかどうか	
	CharacterSpec::Transform::CBallOnLeftSide	is_ballOnleftSideSpec;	// ボールがキャラクターの左にいるかどうか

	// 球を打てる範囲に入ったら
	if (is_shotRange.IsSatisfiedBy(registry, player))
	{
		// プレイヤーがボールの左側にいるなら
		if (is_ballOnleftSideSpec.IsSatisfiedBy(registry, player))
		{
			// ショットモーションを再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT_LEFT, 1, false);
		}
		else
		{
			// ショットモーションを再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT, 1, false);
		}
	}

	// チャージボタンを押していたら
	if (pKeyboard->GetPress(DIK_L) || pJoyPad->GetTriggerPress(pJoyPad->JOYKEY_R2))
	{
		// チャージ時間を加算
		playerComp.nChargeTime++;

		CharacterSpec::Motion::CCheckShot is_shot; // ショットモーションかどうか

		// チャージモーションが再生できたかどうか
		bool bPlayChageMotion = false;

		// チャージ時間がある、ショットモーションじゃないなら
		if (playerComp.nChargeTime >= CHAGE_TIME && CNotSpec(is_shot).IsSatisfiedBy(registry, player))
		{
			// モーションの再生
			bPlayChageMotion = MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_CHARGE, 5, true);
		}

		// 再生できたなら
		if (pSound != nullptr && bPlayChageMotion)
		{
			pSound->Play(CSound::LABEL_SE_CHARGE);
		}

		if (MotionManager::Check::Type(motionComp, MotionComponent::MOTIONTYPE_JUMP))
		{
			// モーションの再生
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_JUMP_SHOT, 4, true);
		}
	}

	CharacterSpec::Motion::CFinishCharge is_FinishChargeMotion; // チャージモーションが終わったかどうか

// チャージモーションが終わったかどうか
	if (is_FinishChargeMotion.IsSatisfiedBy(registry, player))
	{
		characterComp.fShotPower = MAX_CHAGE_POWER;
		playerComp.nChargeTime = 0;

		// モーションの再生
		MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT, 4, true);
	}
	else if (is_chargeMotion.IsSatisfiedBy(registry, player))
	{
		// モーションの全体のフレームの割合
		float fRateMotion = MotionManager::Helper::GetAnimationRatio(motionComp, MotionComponent::MOTIONTYPE_CHARGE);

		// モーションのフレームに応じて打つ力を設定
		float fShotPower = (MAX_CHAGE_POWER * fRateMotion) + CHAGE_POWER_OFFSET;
		characterComp.fShotPower = fShotPower;

#ifdef _DEBUG
		CDebugProc::Print("ショットパワー = [ %.2f / %.2f ]\n", characterComp.fShotPower, MAX_CHAGE_POWER);
#endif // _DEBUG
	}
}

//===================================================
// チャージキーを離したら
//===================================================
void PlayerSystem::ReleaseChargeKey(entt::registry& registry, entt::entity player, MotionComponent& motionComp, CharacterComponent& characterComp)
{
	CharacterSpec::Transform::CBallOnLeftSide	is_ballOnleftSideSpec;	// ボールがキャラクターの左にいるかどうか

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	// 取得できなかったら処理しない
	if (pKeyboard == nullptr) return;
	if (pJoyPad == nullptr) return;

	// プレイヤーの定数の使用
	using namespace PlayerConstants;

	// プレイヤーのコンポーネントの取得
	auto& playerComp = registry.get<PlayerComponent>(player);

	CharacterSpec::Motion::CCharge	is_chargeMotion;		// チャージモーションかどうか

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// チャージ中にキーを離したら
	if ((!pKeyboard->GetPress(DIK_L) && !pJoyPad->GetTriggerPress(pJoyPad->JOYKEY_R2)) &&
		(playerComp.nChargeTime > 0 || is_chargeMotion.IsSatisfiedBy(registry, player)))
	{
		// powerがチャージされていないなら
		if (characterComp.fShotPower <= 0.0f)
		{
			// 最低保障
			characterComp.fShotPower = SHOT_POWER;
		}

		// 左側のいるなら
		if (is_ballOnleftSideSpec.IsSatisfiedBy(registry, player))
		{
			// モーションの再生
			bool bPlayMotion = MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT_LEFT, 7, true);

			// 再生できたなら
			if (pSound != nullptr && bPlayMotion)
			{
				pSound->Play(CSound::LABEL_SE_SHOT);
			}
		}
		else
		{
			// モーションの再生
			bool bPlayMotion = MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT, 4, true);

			// 再生できたなら
			if (pSound != nullptr && bPlayMotion)
			{
				pSound->Play(CSound::LABEL_SE_SHOT);
			}
		}

		playerComp.nChargeTime = 0;
	}
}

//===================================================
// ボールの上下の角度の設定処理
//===================================================
void PlayerSystem::SetShotPitchAngle(entt::registry& registry, const entt::entity player, CharacterComponent& characterComp, CInputKeyboard* pKeyboard, CInputJoypad* pJoyPad)
{
	// プレイヤーの定数の使用
	using namespace PlayerConstants;

	// ショットモーションの判定
	CharacterSpec::Motion::CQuickShot		is_quickshot;
	CharacterSpec::Motion::CLeftQuickShot	is_left_quickshot;

	PlayerSec::CJoyPadMoveForward			is_move_forward; // 正面に移動しているか

	// ショット中かどうか判定
	COrSpec is_shot = { is_quickshot, is_left_quickshot };


	// ショット中じゃないなら
	if (CNotSpec(is_shot).IsSatisfiedBy(registry,player))
	{
		return;
	}
	
	// 角度の設定処理
	if (pKeyboard->GetPress(DIK_W) || is_move_forward.IsSatisfiedBy(registry, player))
	{
		characterComp.fShotPitchAngle = KEY_W_PITCH_ANGLE;
	}
	else if (pKeyboard->GetPress(DIK_S) || pJoyPad->GetPressAnyButton())
	{
		characterComp.fShotPitchAngle = KEY_S_PITCH_ANGLE;
	}
	else
	{
		// 正面に飛ばす
		characterComp.fShotPitchAngle = PlayerConstants::NO_INPUT_ANGLE;
	}
}

//===================================================
// エフェクトの壁に当たったときの処理
//===================================================
void PlayerSystem::SetCollisionEffectWall(entt::registry& registry, const entt::entity player)
{
	// 名前空間の使用
	using namespace PlayerSystemConst;

	// エフェクトの壁の当たり判定の取得
	auto& effectWallCollisionComp = registry.get<CollisionEffectWallComponent>(player);

	// 使えないIDだったら
	if (!registry.valid(effectWallCollisionComp.wallID))
	{
		return;
	}

	// 当たっていたら
	if (effectWallCollisionComp.bCollision)
	{
		// エフェクトの生成
		MeshEffectManager::CreateEffect(registry, effectWallCollisionComp.wallID, effectWallCollisionComp.hitPos, WALL_EFFECT_RADIUS, WALL_EFFECT_ROUGH, WALL_EFFECT_TIME);
		effectWallCollisionComp.bCollision = false;
	}
}