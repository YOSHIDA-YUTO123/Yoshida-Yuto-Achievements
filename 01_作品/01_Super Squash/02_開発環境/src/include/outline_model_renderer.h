//===================================================
//
// アウトラインを適応するモデルの描画処理 [outline_model_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OUTLINE_MODEL_RENDERER_H_
#define _OUTLINE_MODEL_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// アウトラインシェーダーを適応するモデルの処理の定義
//***************************************************
struct OutLineModelRenderer : RendererSystemBase
{
	OutLineModelRenderer() = default;
	~OutLineModelRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif