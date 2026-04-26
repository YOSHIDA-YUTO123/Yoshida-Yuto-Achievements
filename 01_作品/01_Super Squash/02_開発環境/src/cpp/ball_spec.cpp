//===================================================
//
// ボールの条件をまとめた.hファイル [ball_spec.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ball_spec.h"
#include "velocity_component.hpp"
#include "ball_constants.hpp"
#include "utility_math.h"
#include "ball_component.hpp"

//===================================================
// ボールの速度がSmall以上なら
//===================================================
bool BallSpec::CSpeedLevelAboveSmall::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	// 移動量の取得
	auto& velocityComp = registry.get<VelocityComponent>(entity);

	// 速さがSmall以上なら
	if (velocityComp.fMoveLength >= BallConstants::MOVE_THRESHOLD_LEVEL_SMALL)
	{
		return true;
	}
	return false;
}

//===================================================
// ボールの速度がMedium以上なら
//===================================================
bool BallSpec::CSpeedLevelAboveMedium::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	// 移動量の取得
	auto& velocityComp = registry.get<VelocityComponent>(entity);

	// 速さがSmall以上なら
	if (velocityComp.fMoveLength >= BallConstants::MOVE_THRESHOLD_LEVEL_MEDIUM)
	{
		return true;
	}
	return false;
}

//===================================================
// インターバルがあるなら
//===================================================
bool BallSpec::CInterval::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	// ボールの取得
	auto& BallComp = registry.get<BallComponent>(entity);

	// インターバルがあるなら
	if (BallComp.nInterval >= 0)
	{
		return true;
	}
	return false;
}
