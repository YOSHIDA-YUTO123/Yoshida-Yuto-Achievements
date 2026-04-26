//===================================================
//
// スコアの加算を楽にする [score_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "score_manager.h"
#include "score_component.hpp"
#include "utility_math.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ScoreManager_Const
{
	constexpr int SCORE_NORMAL		= 50000;	// 通常の値
	constexpr int SCORE_GROWTH		= 200000;	// 成長中の値
	constexpr int SCORE_ADVANCED	= 400000;	// 発展の値
	constexpr int SCORE_ELITE		= 600000;	// 高いの値
	constexpr int SCORE_LEGENDARY	= 800000;	// もっと高いの値
}

//===================================================
// スコアの加算
//===================================================
void ScoreManager::AddScore(entt::registry& registry, const int nScore)
{
	// スコアの取得
	auto score_ID = registry.view<ScoreComponent>().front();

	// 取得できないなら
	if (!registry.valid(score_ID)) return;

	auto& scoreComp = registry.get<ScoreComponent>(score_ID);

	scoreComp.nScore += nScore;
}

//===================================================
// スコアの状態の取得
//===================================================
ScoreManager::State ScoreManager::GetState(entt::registry& registry)
{
	// スコアの取得
	auto score_ID = registry.view<ScoreComponent>().front();

	// 取得できないなら
	if (!registry.valid(score_ID)) return State::Normal;

	auto& scoreComp = registry.get<ScoreComponent>(score_ID);

	// 定数の使用
	using namespace ScoreManager_Const;

	// 通常の範囲内だったら
	if (math::IsInRange(scoreComp.nScore,0, SCORE_NORMAL))
	{
		return State::Normal;
	}
	else if (math::IsInRange(scoreComp.nScore, SCORE_NORMAL, SCORE_GROWTH))
	{
		return State::Growth;
	}
	else if (math::IsInRange(scoreComp.nScore, SCORE_GROWTH, SCORE_ADVANCED))
	{
		return State::Advanced;
	}
	else if (math::IsInRange(scoreComp.nScore, SCORE_ADVANCED, SCORE_ELITE))
	{
		return State::Elite;
	}
	else if (math::IsInRange(scoreComp.nScore, SCORE_ELITE, SCORE_LEGENDARY))
	{
		return State::Legendary;
	}

	return State::Legendary;
}
