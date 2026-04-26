//===================================================
//
// チュートリアルのUIの更新処理 [tutorial_ui_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TUTORIAL_UI_SYSTEM_H_
#define _TUTORIAL_UI_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct UIFadeOutComponent;		// フェードアウト
struct TutorialSystemComponent;	// チュートリアルの処理

//***************************************************
// チュートリアルのUIの更新処理の定義
//***************************************************
struct TutorialUISystem : SystemBase
{
	TutorialUISystem() = default;
	~TutorialUISystem() = default;

	void Update(entt::registry& registry) override;
private:
	void SetTask(entt::registry& registry, const char* pTexturePath, TutorialSystemComponent& tutorialComp);
	void SetBall(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void SetSuccess(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void SetMoveTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp);
	void SetJetPackTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp);
	void SetShotTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp);
	void SetQuickTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp);
	void SetBallSpeedSmallTask(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void SetBallSpeedMediumTask(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void SetRuleUI(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void SetGameStart(entt::registry& registry, UIFadeOutComponent& fadeOutComp, TutorialSystemComponent& tutorialComp);
	void SetGameStartTask(entt::registry& registry, TutorialSystemComponent& tutorialComp);
	void Skip(entt::registry& registry, TutorialSystemComponent& tutorialComp);
};
#endif