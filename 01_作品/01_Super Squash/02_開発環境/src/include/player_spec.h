//===================================================
//
// プレイヤーの判定処理をまとめた名前空間 [player_spec.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_SPEC_H_
#define _PLAYER_SPEC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// プレイヤーの判定処理の名前空間の定義
//***************************************************
namespace PlayerSec
{
	// 素早い移動のインターバルの判定
	class CQuickMoveInterval : public CSpecification
	{
	public:
		CQuickMoveInterval() = default;
		~CQuickMoveInterval() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const override;
	};

	// 素早い移動時間中かどうかの判定
	class CCheckQuickMoveTime : public CSpecification
	{
	public:
		CCheckQuickMoveTime() = default;
		~CCheckQuickMoveTime() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const override;
	};

	// パッドで左移動をしているかどうかの判定
	class CJoyPadMoveLeft : public CSpecification
	{
	public:
		CJoyPadMoveLeft() = default;
		~CJoyPadMoveLeft() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const override;
	};

	// パッドで右移動をしているかどうかの判定
	class CJoyPadMoveRight : public CSpecification
	{
	public:
		CJoyPadMoveRight() = default;
		~CJoyPadMoveRight() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const override;
	};

	// パッドで前方移動をしているかどうかの判定
	class CJoyPadMoveForward : public CSpecification
	{
	public:
		CJoyPadMoveForward() = default;
		~CJoyPadMoveForward() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const override;
	};
}
#endif