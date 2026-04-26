//===================================================
//
// 影を受けるフィールドの描画 [shadowmap_recieve_field_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_RECIEVE_FIELD_RENDERER_H_
#define _SHADOW_MAP_RECIEVE_FIELD_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 影を受けるフィールドの描画処理の定義
//***************************************************
struct ShadowMapRecieveFIeldRenderer : RendererSystemBase
{
	ShadowMapRecieveFIeldRenderer() = default;
	~ShadowMapRecieveFIeldRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif