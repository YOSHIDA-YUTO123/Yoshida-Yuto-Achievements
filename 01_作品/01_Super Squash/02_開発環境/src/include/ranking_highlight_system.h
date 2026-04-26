//===================================================
//
// ランキングのハイライト表示の更新処理の定義 [ranking_highlight_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RANKING_HIGH_LIGHT_SYSTEM_H_
#define _RANKING_HIGH_LIGHT_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// ランキングのハイライト表示の更新処理の定義
//***************************************************
struct RankingHighLightSystem : SystemBase
{
	RankingHighLightSystem() = default;
	~RankingHighLightSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif