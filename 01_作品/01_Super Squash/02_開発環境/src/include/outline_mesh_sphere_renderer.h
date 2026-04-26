//===================================================
//
// アウトラインを適応する球体メッシュの描画処理 [outline_mesh_sphere_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OUTLINE_MESH_SPHERE_RENDERER_H_
#define _OUTLINE_MESH_SPHERE_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// メッシュの球体にアウトラインを適応する処理
//***************************************************
struct OutLineMeshSphereRenderer : RendererSystemBase
{
	OutLineMeshSphereRenderer() = default;
	~OutLineMeshSphereRenderer() = default;

	void Renderer(entt::registry& registry) override;
};
#endif
