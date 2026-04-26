//===================================================
//
// ビルボードの描画処理 [billboard_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BILLBOARD_RENDERER_H_
#define _BILLBOARD_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// ビルボードの描画処理
//***************************************************
struct BillboardRenderer : RendererSystemBase
{
	BillboardRenderer() = default;
	~BillboardRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif