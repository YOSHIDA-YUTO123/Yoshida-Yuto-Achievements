//===================================================
//
// キャラクターのモーションの条件クラス [character_motion_spec.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character_motion_spec.h"
#include "motion_component.hpp"
#include "motion_manager.h"
#include "spec_combinators.h"

//===================================================
// 判定処理(キャラクターのモーションが素早いショットかどうか)
//===================================================
bool CharacterSpec::Motion::CQuickShot::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションが素早いショットかどうかを判定
	const bool is_Quick_Shot = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT);

	return is_Quick_Shot;
}

//===================================================
// 判定処理(キャラクターのモーションがショットかどうか)
//===================================================
bool CharacterSpec::Motion::CShotMotion::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションがショットかどうかを判定
	const bool is_Shot = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_SHOT);

	return is_Shot;
}
//===================================================
// 素早い左ショットをしているかどうか
//===================================================
bool CharacterSpec::Motion::CLeftQuickShot::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションが素早い左ショットかどうかを判定
	const bool is_leftShot = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT_LEFT);

	return is_leftShot;
}

//===================================================
// ジャンプショットかどうか
//===================================================
bool CharacterSpec::Motion::CJumpShot::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションがジャンプショットかどうかを判定
	const bool is_JumpShot = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_JUMP_SHOT);

	return is_JumpShot;
}

//===================================================
// ジャンプショットのアタックフレーム内にいるかどうか
//===================================================
bool CharacterSpec::Motion::CJumpShotInAttackFrame::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションがジャンプショットのアタックフレーム内かどうかを判定
	const bool is_JumpShotAttack = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_JUMP_SHOT, 0);

	return is_JumpShotAttack;
}

//===================================================
// チャージモーションかどうか
//===================================================
bool CharacterSpec::Motion::CCharge::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// モーションがチャージを判定
	const bool is_ChargeShot = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_CHARGE);

	return is_ChargeShot;
}

//===================================================
// チャージモーションが終了したかどうか
//===================================================
bool CharacterSpec::Motion::CFinishCharge::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// チャージモーションが終わったかを判定
	const bool is_FinishChargeShot = MotionManager::Check::Finish(MotionComp, MotionComponent::MOTIONTYPE_CHARGE);

	return is_FinishChargeShot;
}

//===================================================
// 吹き飛びモーションかどうか
//===================================================
bool CharacterSpec::Motion::CBlowOff::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// 吹き飛びモーションかどうかを判定
	const bool is_BlowOff = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_BLOW_OFF);

	return is_BlowOff;
}

//===================================================
// ジェットパックモーションかどうか
//===================================================
bool CharacterSpec::Motion::CJetPack::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// ジェットパックモーションかどうかを判定
	const bool is_JetPack = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_JET_PACK);

	return is_JetPack;
}

//===================================================
// ショットモーションかどうか
//===================================================
bool CharacterSpec::Motion::CCheckShot::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// ショットモーションの判定の取得
	CharacterSpec::Motion::CShotMotion		is_ShotMotionSpec;			// 通常ショットモーション判定
	CharacterSpec::Motion::CQuickShot		is_QuickShotMotionSpec;		// 素早いショットモーション判定
	CharacterSpec::Motion::CLeftQuickShot	is_LeftQuickShotMotionSpec;	// 左打ち素早いショットモーション判定
	CharacterSpec::Motion::CJumpShot		is_JumpShotMotionSpec;		// ジャンプショット判定

	// チャージモーションかどうか判定
	if (COrSpec(is_ShotMotionSpec, is_QuickShotMotionSpec).IsSatisfiedBy(registry, character) ||
		COrSpec(is_LeftQuickShotMotionSpec, is_JumpShotMotionSpec).IsSatisfiedBy(registry, character))
	{
		return true;
	}
	return false;
}

//===================================================
// ジェットパックの素早い左移動かどうか
//===================================================
bool CharacterSpec::Motion::CJetPackMoveLeft::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// ジェットパックの素早い左移動かどうかを判定
	const bool is_JetPack_Left = MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_JETPACK_MOVE_LEFT);

	return is_JetPack_Left;
}

//===================================================
// ジェットパックの素早い左移動のイベントフレームかどうか
//===================================================
bool CharacterSpec::Motion::CCheckJetLeftMoveFrame::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// ジェットパックの素早い左移動のイベントフレームかどうかを判定
	const bool is_JetPack_Left = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_JETPACK_MOVE_LEFT, 0);

	return is_JetPack_Left;
}

//===================================================
// ジェットパックの素早い右移動のイベントフレームかどうか
//===================================================
bool CharacterSpec::Motion::CJetPackMoveRight::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// ジェットパックの素早い右移動のイベントフレームかどうかを判定
	const bool is_JetPack_Right = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_JETPACK_MOVE_RIGHT, 0);

	return is_JetPack_Right;
}

//===================================================
// 移動モーションからニュートラルモーションに移行できるか
//===================================================
bool CharacterSpec::Motion::CMoveToNeutral::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	Motion::CMove		isMove(false);			// 移動モーションかどうか
	Motion::CRightMove	isRightMove(false);		// 右移動モーションかどうか
	Motion::CLeftMove	isLeftMove(false);		// 左移動モーションかどうか

	// ニュートラルモーションが発動できるかどうか
	COrSpec isMoveright = { isMove,isRightMove };

	const bool isPlayNeutral = isMoveright.IsSatisfiedBy(registry, character) || isLeftMove.IsSatisfiedBy(registry, character);

	return isPlayNeutral;
}

//===================================================
// 移動モーションかどうか
//===================================================
bool CharacterSpec::Motion::CMove::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& motionComp = registry.get<MotionComponent>(character);

	// 左・右移動のブレンドモーション
	const bool bBlendLeftMove = MotionManager::Check::BlendType(motionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE);
	const bool bBlendRightMove = MotionManager::Check::BlendType(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE);

	// 左・右移動のモーション
	const bool bLeftMove = MotionManager::Check::Type(motionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE);
	const bool bRightMove = MotionManager::Check::Type(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE);

	if (m_bBlend)
	{
		// ブレンドモーションだったらだったら
		if (bBlendLeftMove || bBlendRightMove)
		{
			return true;
		}
	}
	// ブレンドモーションだったらだったら
	else if (bLeftMove || bRightMove)
	{
		return true;
	}

	return false;
}

//===================================================
// 右移動モーションかどうか
//===================================================
bool CharacterSpec::Motion::CRightMove::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& motionComp = registry.get<MotionComponent>(character);

	// ブレンドモーションだったらだったら
	if (MotionManager::Check::BlendType(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE) && m_bBlend)
	{
		return true;
	}

	// モーションだったらだったら
	if (MotionManager::Check::Type(motionComp, MotionComponent::MOTIONTYPE_RIGHT_MOVE) && !m_bBlend)
	{
		return true;
	}

	return false;
}

//===================================================
// 左移動か判定
//===================================================
bool CharacterSpec::Motion::CLeftMove::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// ブレンドモーションだったら
	if (m_bBlend)
	{
		if (MotionManager::Check::BlendType(MotionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE))
		{
			return true;
		}
	}
	// モーションじゃないなら
	else if (MotionManager::Check::Type(MotionComp, MotionComponent::MOTIONTYPE_LEFT_MOVE))
	{
		return true;
	}

	return false;
}
