//===================================================
//
// UIの壁の制御処理 [ui_wall_controller.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UI_WALL_CONTROLLER_H_
#define _UI_WALL_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// UIの壁の制御処理の定義
//***************************************************
namespace UIWallController
{
	void SetTitleWall(entt::registry& registry);
	void SetTutorialWall(entt::registry& registry);
	void SetTutorialTask(entt::registry& registry, const char* pTexturePath);
	void SetTutorialGameRule(entt::registry& registry);
	void SetTutotialGameStart(entt::registry& registry);
	void SetGameStart(entt::registry& registry);
	void SetResultUI(entt::registry& registry);
	void BeginResultScrol(entt::registry& registry);
	void SetRanking(entt::registry& registry);
	void SetTimeLimit60Second(entt::registry& registry);
	void SetSecondFaze(entt::registry& registry);

	void SetScrolUV(entt::registry& registry, const float fPosTexU);
	void SetNormalUV(entt::registry& registry);
};
#endif