//===================================================
//
// キャラクターの当たり判定の条件クラス [character_collision_spec.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character_collision_spec.h"
#include "motion_component.hpp"
#include "motion_manager.h"
#include "character_motion_spec.h"
#include "tag_component.hpp"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "collision.h"
#include "parent_component.hpp"
#include "character_model_component.hpp"

//===================================================
// 判定処理(ボールとラケットが当たったかどうか)
//===================================================
bool  CharacterSpec::Collision::CBallRacket::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// モーションのコンポーネントの取得
	auto& MotionComp = registry.get<MotionComponent>(character);

	// 当たり判定をするフレーム内かどうか
	const bool is_InQuickShotFrame = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT, 0);

	// 当たり判定をするフレーム内かどうか
	const bool is_InShotFrame = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_SHOT, 0);

	// 当たり判定をするフレーム内かどうか
	const bool is_InQuick_Shot_LeftFrame = MotionManager::Check::EventFrame(MotionComp, MotionComponent::MOTIONTYPE_QUICK_SHOT_LEFT, 0);

	// ジャンプショットかどうか
	CharacterSpec::Motion::CJumpShotInAttackFrame JumpShotInAttackFrame;

	// 当たり判定をするフレーム内かどうか
	const bool is_InJump_Shot_Frame = JumpShotInAttackFrame.IsSatisfiedBy(registry, character);

	// 結果
	const bool is_result = is_InQuickShotFrame || is_InShotFrame || is_InQuick_Shot_LeftFrame || is_InJump_Shot_Frame;

	return is_result;
}

//===================================================
// 判定処理(ボールと体が当たったかどうか)
//===================================================
bool CharacterSpec::Collision::CBallHitBody::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// ボールの取得
	auto ball_view = registry.view<Tag::BallTag>();

	// カプセルコライダーの取得
	auto& capsuleColliderComp = registry.get<CapsuleColliderComponent>(character);

	// ボール分回す
	for (auto ball : ball_view)
	{
		// ボールのコライダーの取得
		auto& ballSphereColliderComp = registry.get<SphereColliderComponent>(ball);

		// カプセルと球の当たり判定
		if (::Collision::Capsule::CollisionSphere(registry, capsuleColliderComp, ballSphereColliderComp))
		{
			return true;
		}
	}
	return false;
}

//===================================================
// プレイヤーがボールを打つ範囲に入ったかどうか
//===================================================
bool CharacterSpec::Collision::CShotRange::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// 使用できないなら
	if (!registry.valid(character))
	{
		return false;
	}

	// ボールの取得
	auto ballView = registry.view<ColliderTag::BallSphere>();

	// ボールを打つ範囲のコライダーの取得
	auto ballShotRangeColliderID = registry.view<ColliderTag::BallShotRange>().front();

	auto& ballShotRange = registry.get<SphereColliderComponent>(ballShotRangeColliderID);

	// ボール分回す
	for (auto ball : ballView)
	{
		// ボールのコライダーの取得
		auto& ballSphereColliderComp = registry.get<SphereColliderComponent>(ball);

		// カプセルと球の当たり判定
		if (::Collision::Sphere::Collision(registry, ballShotRange, ballSphereColliderComp))
		{
			return true;
		}
	}
	return false;
}
