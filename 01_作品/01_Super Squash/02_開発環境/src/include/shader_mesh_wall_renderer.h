//===================================================
//
// シェーダーを適応するメッシュの壁の描画処理 [shader_mesh_wall_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADER_MESH_WALL_RENDERER_H_
#define _SHADER_MESH_WALL_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// シェーダーを適応するメッシュの壁の描画処理
//***************************************************
struct ShaderMeshWallRenderer : RendererSystemBase
{
	ShaderMeshWallRenderer() = default;
	~ShaderMeshWallRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif