//===================================================
//
// 2Dポリゴンの描画処理 [sprite_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_SYSTEM_2D_H_
#define _RENDERER_SYSTEM_2D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 描画system2Dクラスの定義
//***************************************************
struct SpriteRenderer : RendererSystemBase
{
	SpriteRenderer() = default;
	~SpriteRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif