//===================================================
//
// メッシュの初期化処理 [init_mesh.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _INIT_MESH_H_
#define _INIT_MESH_H_

//***************************************************
// メッシュの初期化名前空間の定義
//***************************************************
namespace InitMesh
{
	HRESULT Cylinder(entt::registry& registry, entt::entity entity);
	HRESULT Wall(entt::registry& registry, entt::entity entity);
	HRESULT WallMT(entt::registry& registry, entt::entity entity, const D3DXVECTOR2& offPosUVLeft, const D3DXVECTOR2& offPosUVRight);
	HRESULT Dome(entt::registry& registry, entt::entity entity);
	HRESULT Circle(entt::registry& registry, entt::entity entity);
	HRESULT Orbit(entt::registry& registry, entt::entity entity);
	HRESULT Sphere(entt::registry& registry, entt::entity entity);
}

#endif