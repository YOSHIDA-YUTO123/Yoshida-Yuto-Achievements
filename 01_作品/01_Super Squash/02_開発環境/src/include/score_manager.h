//===================================================
//
// スコアの加算を楽にする [score_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

//***************************************************
// スコアマネージャーの名前空間の定義
//***************************************************
namespace ScoreManager
{
	// 状態
	enum class State
	{
		Normal = 0, // 通常
		Growth,		// 成長中
		Advanced,	// 発展
		Elite,		// 高い
		Legendary,	// もっと高い
		Max
	};

	constexpr int BASE_SCORE = 100; // 計算の基準となるスコアの値

	void AddScore(entt::registry& registry, const int nScore);
	State GetState(entt::registry& registry);
}
#endif