//===================================================
//
//メッシュの球体の描画(MRT)の描画処理 [mesh_sphere_renderer_mrt.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_SPHERE_RENDERER_HPP_
#define _MESH_SPHERE_RENDERER_HPP_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_mrt_system_base.h"

//***************************************************
// メッシュの球体の描画(MRT)の描画処理の定義
//***************************************************
struct MeshSphereRendererMRT : RendererMRTSystemBase
{
	MeshSphereRendererMRT() = default;
	~MeshSphereRendererMRT() = default;

	void Renderer(entt::registry& registry, const int nType) override;
};
#endif