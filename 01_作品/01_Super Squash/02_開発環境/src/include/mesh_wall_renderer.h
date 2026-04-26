//===================================================
//
// メッシュの壁の描画処理 [mesh_wall_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WALL_RENDERER_H_
#define _MESH_WALL_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// メッシュの壁の描画処理の定義
//***************************************************
struct MeshWallRenderer : RendererSystemBase
{
	MeshWallRenderer() = default;
	~MeshWallRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif