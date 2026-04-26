//===================================================
//
// ステンシル用のポリゴンの処理 [stencil_polygon.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _STENCIL_POLYGON_H_
#define _STENCIL_POLYGON_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// ステンシル用のポリゴンのクラスの定義
//***************************************************
struct StencilPolygonRenderer : RendererSystemBase
{
	StencilPolygonRenderer() = default;
	~StencilPolygonRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif