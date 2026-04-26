//===================================================
//
// 3Dポリゴンのファクトリー [factory_mesh_quad.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_MESH_QUAD_H_
#define _FACTORY_MESH_QUAD_H_

//***************************************************
// 3Dポリゴンのファクトリー
//***************************************************
namespace FactoryMeshQuad
{
	entt::entity Quad(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath);
	entt::entity Shadow(entt::registry& registry, const entt::entity owner, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const float fMaxHeight);
	entt::entity Sheet(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath, const int nSegmentU, const int nSegmentV, const bool bRendererMT = false);
}

//***************************************************
// 3Dポリゴンの初期化処理
//***************************************************
namespace InitMeshQuad
{
	static constexpr int MIN_SEGMENT = 1;	// 最小の分割数

	HRESULT Init(entt::registry& registry, entt::entity entity, const int nSegmentU = MIN_SEGMENT, const int nSegmentV = MIN_SEGMENT);
	HRESULT InitMT(entt::registry& registry, entt::entity entity, const int nSegmentU = MIN_SEGMENT, const int nSegmentV = MIN_SEGMENT);
	HRESULT InitUIWall(entt::registry& registry, entt::entity entity, const D3DXVECTOR2& offPosUVLeft, const D3DXVECTOR2& offPosUVRight);
}

#endif