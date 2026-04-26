//===================================================
//
// 3Dポリゴンの描画処理 [mesh_quad_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHQUAD_RENDERER_H_
#define _MESHQUAD_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 描画system2Dクラスの定義
//***************************************************
struct MeshQuadRenderer : RendererSystemBase
{
	MeshQuadRenderer() = default;
	~MeshQuadRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif