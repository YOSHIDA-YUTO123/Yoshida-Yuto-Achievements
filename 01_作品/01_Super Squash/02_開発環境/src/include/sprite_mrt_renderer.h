//===================================================
//
// マルチレンダーターゲットのスプライト [sprite_mrt_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SPRITE_MRT_RENDERER_H_
#define _SPRITE_MRT_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_mrt_system_base.h"

//***************************************************
// マルチレンダーターゲットのスプライトの描画処理
//***************************************************
struct SpriteMRTRenderer : RendererMRTSystemBase
{
	SpriteMRTRenderer() = default;
	~SpriteMRTRenderer() = default;

	void Renderer(entt::registry& registry, const int nType) override;
};
#endif