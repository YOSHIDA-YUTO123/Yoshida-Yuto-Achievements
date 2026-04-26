//===================================================
//
// 球体メッシュの描画処理 [mesh_sphere_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_SPHERE_RENDERER_H_
#define _MESH_SPHERE_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 球体メッシュの描画システムの定義
//***************************************************
struct MeshSphereRenderer : RendererSystemBase
{
	MeshSphereRenderer() = default;
	~MeshSphereRenderer() = default;

	void Renderer(entt::registry& registry) override;
private:
	void DrawMotionBlur(entt::registry& registry, const entt::entity entity);
};
#endif