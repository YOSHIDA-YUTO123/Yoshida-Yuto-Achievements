//===================================================
//
// ステンシルシャドウの描画処理 [stencil_shadow_renderer.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// 二重インクルード防止
//***************************************************
#ifndef _STENCIL_SHADOW_RENDERER_H_
#define _STENCIL_SHADOW_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// ステンシルシャドウの描画処理の定義
//***************************************************
struct StencilShadowRenderer : RendererSystemBase
{
	StencilShadowRenderer() = default;
	~StencilShadowRenderer() = default;

	void Renderer(entt::registry& registry) override;
private:
	void SetMatrix(entt::registry& registry, const entt::entity entity, LPDIRECT3DDEVICE9 pDevice);
	void Draw(entt::registry& registry, const entt::entity entity, LPDIRECT3DDEVICE9 pDevice);
};

#endif