//===================================================
//
// 影のモデルの描画 [shadow_model_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_RECIEVE_MODEL_RENDERER_H_
#define _SHADOW_MAP_RECIEVE_MODEL_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 影モデルの描画処理の定義
//***************************************************
struct ShadowMapRecieveModelRenderer : RendererSystemBase
{
	ShadowMapRecieveModelRenderer() = default;
	~ShadowMapRecieveModelRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif