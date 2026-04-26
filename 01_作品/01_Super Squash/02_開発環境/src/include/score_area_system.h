//===================================================
//
// スコアのエリアの更新処理 [score_area_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCORE_AREA_SYSTEM_H_
#define _SCORE_AREA_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct ScoreAreaComponent; // スコアのエリアのコンポーネント

//***************************************************
// スコアのエリアの更新処理
//***************************************************
struct ScoreAreaSystem : SystemBase
{
	ScoreAreaSystem() = default;
	~ScoreAreaSystem() = default;

	void Update(entt::registry& registry) override;
private:
	bool CollisionBall(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void SetArea(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void Reset(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void SetAreaSegement(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void SetScore(entt::registry& registry, const entt::entity ball);
	void SetEffect(entt::registry& registry, const entt::entity ball);
	void UpdatePolygon(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void BeginFadeOut(entt::registry& registry, ScoreAreaComponent& scoreAreaComp);
	void BeginShake(entt::registry& registry, const entt::entity polygonID, const float fRangeMin, const float fRangeMax, const int nTime);
	void UpdateShake(entt::registry& registry, const entt::entity polygonID);
};
#endif