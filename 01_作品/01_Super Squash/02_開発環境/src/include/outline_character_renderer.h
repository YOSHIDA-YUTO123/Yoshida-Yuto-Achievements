//===================================================
//
// アウトラインを適応するキャラクターの描画処理 [outline_character_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OUTLINE_CHARACTER_RENDERER_H_
#define _OUTLINE_CHARACTER_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// アウトラインを適応するキャラクターの描画処理
//***************************************************
struct OutLineCharacterRenderer : RendererSystemBase
{
	OutLineCharacterRenderer() = default;
	~OutLineCharacterRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif