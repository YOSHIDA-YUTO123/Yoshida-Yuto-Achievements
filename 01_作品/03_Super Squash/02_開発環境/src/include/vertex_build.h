//===================================================
//
// 頂点のマネージャー [vertex_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _VERTEX_MANAGER_H_
#define _VERTEX_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
struct VertexBufferComponent;	// 頂点バッファのコンポーネント
struct MeshVtxComponent;		// メッシュの頂点のコンポーネント

//***************************************************
// 頂点のマネージャー名前空間の定義
//***************************************************
namespace VertexManager
{
	HRESULT CreateMeshVtxBuffer(entt::registry& registry, entt::entity entity);
	HRESULT CreateMeshVtxBufferMT(VertexBufferComponent& vertexBufferComp, MeshVtxComponent& meshVtxComp);
	HRESULT CreateMeshVtxBuffer(entt::registry& registry, entt::entity entity, const int nNumVertex, const int nNumIndex);
	HRESULT CreateSpriteVtxBuffer(VertexBufferComponent& vertexBufferComp);
	HRESULT CreateQuadVtxBuffer(entt::registry& registry, entt::entity entity);
	HRESULT CreateQuadVtxBufferMT(VertexBufferComponent& vertexBufferComp);
	void GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV);
	int  GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV, const int nOffsetIndex, const int nOffsetVertex);
	void FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int* pOffsetIndex = nullptr);
	void FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int nOffsetVertex, int nOffsetIndex);
}
#endif