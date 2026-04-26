//===================================================
//
// キャラクターのモーションの条件クラス [character_motion_spec.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_MOTION_SPEC_H_
#define _CHARACTER_MOTION_SPEC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// キャラクターの条件(モーション系)の名前空間の定義
//***************************************************
namespace CharacterSpec::Motion
{
	// ショットの判定
	class CShotMotion : public CSpecification
	{
	public:
		CShotMotion() = default;
		~CShotMotion() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

// 素早いショットの判定
	class CQuickShot : public CSpecification
	{
	public:
		CQuickShot() = default;
		~CQuickShot() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

// ショット(左側)の判定
	class CLeftQuickShot : public CSpecification
	{
	public:
		CLeftQuickShot() = default;
		~CLeftQuickShot() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

// ジャンプショットの判定
	class CJumpShot : public CSpecification
	{
	public:
		CJumpShot() = default;
		~CJumpShot() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ジャンプショットのアタックフレーム内か判定
	class CJumpShotInAttackFrame : public CSpecification
	{
	public:
		CJumpShotInAttackFrame() = default;
		~CJumpShotInAttackFrame() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// チャージモーションの判定
	class CCharge : public CSpecification
	{
	public:
		CCharge() = default;
		~CCharge() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// チャージモーションが終了の判定
	class CFinishCharge : public CSpecification
	{
	public:
		CFinishCharge() = default;
		~CFinishCharge() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// 吹き飛びモーションの判定
	class CBlowOff : public CSpecification
	{
	public:
		CBlowOff() = default;
		~CBlowOff() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ジェットパックモーションの判定
	class CJetPack : public CSpecification
	{
	public:
		CJetPack() = default;
		~CJetPack() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ショットモーションかどうかの判定
	class CCheckShot : public CSpecification
	{
	public:
		CCheckShot() = default;
		~CCheckShot() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ジェットパックの素早い左移動かどうかの判定
	class CJetPackMoveLeft : public CSpecification
	{
	public:
		CJetPackMoveLeft() = default;
		~CJetPackMoveLeft() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ジェットパックの素早い左移動のEventFrameの判定
	class CCheckJetLeftMoveFrame : public CSpecification
	{
	public:
		CCheckJetLeftMoveFrame() = default;
		~CCheckJetLeftMoveFrame() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// ジェットパックの素早い右移動かどうかの判定
	class CJetPackMoveRight : public CSpecification
	{
	public:
		CJetPackMoveRight() = default;
		~CJetPackMoveRight() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// 移動からニュートラルモーションに変更できるかどうかの判定
	class CMoveToNeutral : public CSpecification
	{
	public:
		CMoveToNeutral() = default;
		~CMoveToNeutral() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	};

	// 移動モーションかどうかの判定
	class CMove : public CSpecification
	{
	public:
		explicit CMove(const bool bBlend = true) : m_bBlend(bBlend) {}

		~CMove() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	private:
		bool m_bBlend;
	};

	// 右移動モーションかどうかの判定
	class CRightMove : public CSpecification
	{
	public:
		explicit CRightMove(const bool bBlend = true) : m_bBlend(bBlend) {}

		~CRightMove() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	private:
		bool m_bBlend;
	};

	// 左移動モーションかどうかの判定
	class CLeftMove : public CSpecification
	{
	public:
		explicit CLeftMove(const bool bBlend = true) : m_bBlend(bBlend) {}

		~CLeftMove() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	private:
		bool m_bBlend;
	};
}

#endif