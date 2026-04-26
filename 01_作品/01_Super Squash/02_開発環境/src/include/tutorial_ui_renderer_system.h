//===================================================
//
// チュートリアル時に投影するMRTのテクスチャの作成処理 [tutorial_ui_renderer_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TUTORIAL_UI_RENDERER_SYSTEM_H_
#define _TUTORIAL_UI_RENDERER_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// チュートリアル時のUIの描画処理の定義
//***************************************************
struct TutorialUIRendererSystem : RendererSystemBase
{
	TutorialUIRendererSystem() = default;
	~TutorialUIRendererSystem() = default;

	void Renderer(entt::registry& registry) override;
};
#endif