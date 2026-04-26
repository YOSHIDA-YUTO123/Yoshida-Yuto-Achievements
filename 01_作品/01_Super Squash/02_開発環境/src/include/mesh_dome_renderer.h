//===================================================
//
// メッシュドームの描画処理 [mesh_dome_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHDOME_RENDERER_H_
#define _MESHDOME_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// メッシュドームの描画処理
//***************************************************
struct MeshDomeRenderer : RendererSystemBase
{
	MeshDomeRenderer() = default;
	~MeshDomeRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif