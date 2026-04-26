//===================================================
//
// キャラクターの描画処理 [character_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_RENDERER_H_
#define _CHARACTER_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// キャラクターの描画処理の定義
//***************************************************
struct CharacterRenderer : RendererSystemBase
{
	CharacterRenderer() = default;
	~CharacterRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif