//===================================================
//
// キャラクターの当たり判定の条件クラス [character_collision_spec.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_COLLISION_SPEC_H_
#define _CHARACTER_COLLISION_SPEC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// キャラクターの条件(当たり判定系)の名前空間の定義
//***************************************************
namespace CharacterSpec::Collision
{
	// ラケットとボールと当たり判定
	class CBallRacket : public CSpecification
	{
	public:
		CBallRacket() = default;
		~CBallRacket() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// 体とボールと当たり判定
	class CBallHitBody : public CSpecification
	{
	public:
		CBallHitBody() = default;
		~CBallHitBody() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// プレイヤーがボールを打つ範囲に入ったかどうか判定
	class CShotRange : public CSpecification
	{
	public:
		CShotRange() = default;
		~CShotRange() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};
}

#endif