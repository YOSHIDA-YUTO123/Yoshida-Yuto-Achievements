//===================================================
//
// プレイヤーのモーションに応じた処理 [player_motion_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player_motion_system.h"
#include "tag_component.hpp"
#include "motion_manager.h"
#include "motion_component.hpp"
#include "velocity_component.hpp"
#include "character_motion_spec.h"
#include "particle_helper.h"
#include "manager.h"
#include "factory_billboard.h"
#include "parent_component.hpp"
#include "character_model_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"
#include "effect_component.hpp"
#include "color_constants.hpp"
#include "effect_constants.hpp"
#include "locator_component.hpp"
#include "character_motion_spec.h"
#include "sound.h"
#include "spec_combinators.h"

//***************************************************
// 定数宣言
//***************************************************
namespace PlayerMotionConst
{
	constexpr float MOVE_EFFECT_POSY = 25.0f;	// 移動エフェクトの高さ
	constexpr int MOVE_EFFECT_ANIM_SPEED = 3;	// 移動エフェクトのアニメーションの速さ
	constexpr int MOVE_EFFECT_SEGMENT_U = 4;	// 移動エフェクトのテクスチャの分割数横
	constexpr int MOVE_EFFECT_SEGMENT_V = 3;	// 移動エフェクトのテクスチャの分割数縦
	const D3DXVECTOR3 CHAGE_EFFECT_RANGE = { 25.0f,25.0f,25.0f };
}

//===================================================
// 更新処理
//===================================================
void PlayerMotionSystem::Update(entt::registry& registry)
{
	// プレイヤーの取得
	auto view = registry.view<Tag::PlayerTag>();

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// プレイヤー分回す
	for (auto player : view)
	{
		// モーションの取得
		auto& velocityComp = registry.get<VelocityComponent>(player);
		 
		CharacterSpec::Motion::CJumpShot	is_jumpShot;	// ジャンプモーションかどうか

		// ジャンプ攻撃モーションだったら
		if (is_jumpShot.IsSatisfiedBy(registry,player))
		{
			velocityComp.move.y = 0.0f;
		}

		CharacterSpec::Motion::CCharge is_charge; // チャージモーションかどうか

		// チャージモーションじゃないなら
		if (CNotSpec(is_charge).IsSatisfiedBy(registry, player))
		{
			// チャージの音を止める
			pSound->StopSound(CSound::LABEL_SE_CHARGE);
		}

		// チャージエフェクトの設定処理
		SetChargeEffect(registry, player);

		// 移動モーションのエフェクトの設定処理
		SetMoveEffect(registry, player);
	}
}

//===================================================
// チャージエフェクトの設定処理
//===================================================
void PlayerMotionSystem::SetChargeEffect(entt::registry& registry, const entt::entity player)
{
	CharacterSpec::Motion::CCharge		is_charge;		// チャージモーションかどうか

	//auto& velocityComp = registry.get<VelocityComponent>(player);
	auto& childrenComp = registry.get<ChildrenComponent>(player);

	// チャージモーションかどうか
	if (is_charge.IsSatisfiedBy(registry, player))
	{
		// ラケットの取得
		auto racketID = childrenComp.aChildrenID[CharacterModelComponent::MODEL_RACKET];
		auto& racketTransform = registry.get<Transform3DComponent>(racketID);
		auto& locatorComp = registry.get<LocatorComponent>(racketID);
		auto& racketLocatorTransformComp = registry.get<Transform3DComponent>(locatorComp.locatorID);

		D3DXVECTOR3 effectPos = Const::VEC3_NULL;
		D3DXVECTOR3 worldPos = math::GetPositionFromMatrix(racketTransform.mtxWorld);
		D3DXVECTOR3 racketTopWorldPos = math::GetPositionFromMatrix(racketLocatorTransformComp.mtxWorld);

		// ラケットの根元からてっぺんまでのベクトルを求める
		D3DXVECTOR3 vecRacketTop = racketTopWorldPos - worldPos;

		// 割合を求める
		float fRateRacketPos = math::Random(0.0f, 1.0f);

		// エフェクトの位置を求める
		effectPos = worldPos + vecRacketTop * fRateRacketPos;

		// マネージャーの取得
		CManager* pManager = CManager::GetInstance();

		// パーティクルの補助クラスの取得
		CParticleHelper* pParticleHelper = pManager->GetParticleHelper();

		// 取得できないなら処理しない
		if (pParticleHelper == nullptr) return;

		D3DXVECTOR3 range = PlayerMotionConst::CHAGE_EFFECT_RANGE;
		D3DXVECTOR3 particlePos;

		// ランダムな位置の設定
		particlePos.x = effectPos.x + math::Random(-range.x, range.x);
		particlePos.y = effectPos.y + math::Random(-range.y, range.y);
		particlePos.z = effectPos.z + math::Random(-range.z, range.z);

		ParticleComponent::Param param = {};
		pParticleHelper->GetParam(param, particlePos, "charge", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);

		FactoryBillboard::Create::Particle(registry, param);
	}
}

//===================================================
// 移動モーションのエフェクトの設定処理
//===================================================
void PlayerMotionSystem::SetMoveEffect(entt::registry& registry, const entt::entity player)
{
	// 名前空間の使用
	using namespace PlayerMotionConst;

	// コンポーネントの取得
	auto& motionComp = registry.get<MotionComponent>(player);
	auto& transformComp = registry.get<Transform3DComponent>(player);
	auto& velocityComp = registry.get<VelocityComponent>(player);

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// 音の取得
	CSound* pSound = pManager->GetSound();

	// 移動モーションのフレームないだったら
	if (MotionManager::Check::EventFrame(motionComp, MotionComponent::MOTIONTYPE_MOVE) ||
		MotionManager::Check::EventFrame(motionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE) || 
		MotionManager::Check::EventFrame(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE))
	{
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_WORK);
		}
		// パーティクルの補助クラスの取得
		CParticleHelper* pParticleHelper = pManager->GetParticleHelper();

		// 取得できないなら処理しない
		if (pParticleHelper == nullptr) return;

		D3DXVECTOR3 particlePos = transformComp.pos;
		D3DXVECTOR3 moveDir = velocityComp.move;
		D3DXVec3Normalize(&moveDir, &moveDir);

		particlePos.y += MOVE_EFFECT_POSY;

		ParticleComponent::Param param = {};
		pParticleHelper->GetParam(param, particlePos, "move_smoke", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);

		param.move.x *= moveDir.y;
		param.move.z *= moveDir.y;

		auto particleID = FactoryBillboard::Create::Particle(registry, param);
		FactoryBillboard::Build::AnimationParticle(registry, particleID, MOVE_EFFECT_ANIM_SPEED, false, MOVE_EFFECT_SEGMENT_U, MOVE_EFFECT_SEGMENT_V);
	}
}