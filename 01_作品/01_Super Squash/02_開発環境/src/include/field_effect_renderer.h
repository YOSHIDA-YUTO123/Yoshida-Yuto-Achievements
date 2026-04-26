//===================================================
//
// 地面のエフェクト(シェーダー)を適応する描画処理 [field_effect_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FIELD_EFFECT_RENDERER_H_
#define _FIELD_EFFECT_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 地面のエフェクトを適応する描画処理
//***************************************************
struct FieldEffectRenderer : RendererSystemBase
{
	FieldEffectRenderer() = default;
	~FieldEffectRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif