//===================================================
//
// リザルトの更新処理 [result_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RESULT_SYSTEM_H_
#define _RESULT_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct ResultSystemComponent;	// リザルトの処理のコンポーネント
struct ScoreComponent;			// スコアのコンポーネント

//***************************************************
// リザルトの更新処理の定義
//***************************************************
struct ResultSystem : SystemBase
{
	ResultSystem() = default;
	~ResultSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void CreateScore(entt::registry& registry, const entt::entity ballID, ScoreComponent& scoreComp, ResultSystemComponent& resultSystemComp);
	void ShotBall(entt::registry& registry, ResultSystemComponent& resultSystemComp, ScoreComponent& scoreComp);
	void CreateSpriteSheet(entt::registry& registry, ScoreComponent& scoreComp, ResultSystemComponent& resultSystemComp);
	void BeginScoreSlideOut(entt::registry& registry, ScoreComponent& scoreComp);
	void SetRanking(entt::registry& registry, ResultSystemComponent& resultSystemComp);
	void AdvanceResult(entt::registry& registry, ResultSystemComponent& resultSystemComp, ScoreComponent& scoreComp);
};

#endif