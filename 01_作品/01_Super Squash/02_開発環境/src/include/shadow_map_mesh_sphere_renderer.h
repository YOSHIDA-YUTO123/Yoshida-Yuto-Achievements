//===================================================
//
// 影を落とすメッシュの球体の描画処理 [shadow_map_mesh_sphere_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_MESH_SPHERE_RENDERE_H_
#define _SHADOW_MAP_MESH_SPHERE_RENDERE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_mrt_system_base.h"

//***************************************************
// メッシュの壁の描画処理の定義
//***************************************************
struct ShadowMapMeshSphereRenderer : RendererMRTSystemBase
{
	ShadowMapMeshSphereRenderer() = default;
	~ShadowMapMeshSphereRenderer() = default;

	void Renderer(entt::registry& registry, const int nType) override;
};
#endif