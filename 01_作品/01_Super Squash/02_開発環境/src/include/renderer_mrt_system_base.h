//===================================================
//
// MRT描画systemの基底処理 [renderer_mrt_system_base.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_MRT_SYSTEM_BASE_H_
#define _RENDERER_MRT_SYSTEM_BASE_H_

//***************************************************
//  MRT描画systemの基底構造体の定義
//***************************************************
struct RendererMRTSystemBase
{
	RendererMRTSystemBase() = default;
	virtual ~RendererMRTSystemBase() = default;

	virtual void Renderer(entt::registry& registry, const int nType) = 0;
};
#endif