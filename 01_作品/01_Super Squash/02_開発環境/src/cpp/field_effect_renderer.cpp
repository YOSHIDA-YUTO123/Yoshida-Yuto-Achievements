//===================================================
//
// 地面のエフェクト(シェーダー)を適応する描画処理 [field_effect_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "field_effect_renderer.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "shader_manager.h"
#include "field_effect_component.hpp"
#include "vertex_buffer_component.hpp"
#include "renderer_manager.h"

//===================================================
// 描画処理
//===================================================
void FieldEffectRenderer::Renderer(entt::registry& registry)
{
	(void)registry;
}