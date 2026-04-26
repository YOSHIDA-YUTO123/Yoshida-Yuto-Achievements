//===================================================
//
// モデルの描画(MRT)の処理 [model_renderer_mrt.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MODEL_RENDERER_MRT_H_
#define _MODEL_RENDERER_MRT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_mrt_system_base.h"

//===================================================
// モデルの描画(MRT)の処理の定義
//===================================================
struct ModelRendererMRT : RendererMRTSystemBase
{
	ModelRendererMRT() = default;
	~ModelRendererMRT() = default;

	void Renderer(entt::registry& registry, const int nType) override;
};
#endif