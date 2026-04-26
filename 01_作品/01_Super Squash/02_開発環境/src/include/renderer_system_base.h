//===================================================
//
// 描画systemの基底処理 [renderer_system_base.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_SYSTEM_BASE_H_
#define _RENDERER_SYSTEM_BASE_H_

//***************************************************
// RendererSystemの基底構造体の定義
//***************************************************
struct RendererSystemBase
{
	RendererSystemBase() = default;
	virtual ~RendererSystemBase() = default;

	virtual void Renderer(entt::registry& registry) = 0;
};
#endif