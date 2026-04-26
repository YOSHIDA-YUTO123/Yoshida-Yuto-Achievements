//===================================================
//
// モデルの描画systemの処理 [model_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MODEL_RENDERER_H_
#define _MODEL_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// モデルの描画処理の定義
//***************************************************
struct ModelRenderer : RendererSystemBase
{
	ModelRenderer() = default;
	~ModelRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif