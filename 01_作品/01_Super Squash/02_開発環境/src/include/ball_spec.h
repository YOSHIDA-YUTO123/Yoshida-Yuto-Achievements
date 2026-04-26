//===================================================
//
// ボールの条件をまとめた.hファイル [ball_spec.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_SPEC_H_
#define _BALL_SPEC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// ボールの条件の名前空間の定義
//***************************************************
namespace BallSpec
{
	// ボールの速度がSMall以上か判定
	class CSpeedLevelAboveSmall : public CSpecification
	{
	public:
		CSpeedLevelAboveSmall() = default;
		~CSpeedLevelAboveSmall() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
	};

	// ボールの速度がMedium以上か判定
	class CSpeedLevelAboveMedium : public CSpecification
	{
	public:
		CSpeedLevelAboveMedium() = default;
		~CSpeedLevelAboveMedium() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
	};

	// ボールがあるかどうか判定
	class CInterval : public CSpecification
	{
	public:
		CInterval() = default;
		~CInterval() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
	};
};
#endif