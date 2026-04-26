//===================================================
//
// タイトルメニューのアニメーションの更新処理 [titlemenu_animation_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TITLE_MENU_ANIMATION_SYSTEM_H_
#define _TITLE_MENU_ANIMATION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// タイトルメニューのアニメーションの更新処理の定義
//***************************************************
struct TitleMenuAnimationSystem : SystemBase
{
	TitleMenuAnimationSystem() = default;
	~TitleMenuAnimationSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif