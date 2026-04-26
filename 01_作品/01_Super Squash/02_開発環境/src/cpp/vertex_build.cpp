//===================================================
//
// 頂点のマネージャー [vertex_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 頂点のマネージャー名前空間の定義
//***************************************************
#include "vertex_build.h"
#include "mesh_vtx_component.hpp"
#include "renderer.h"
#include "manager.h"
#include "vertex_buffer_component.hpp"

//===================================================
// 頂点バッファの生成
//===================================================
HRESULT VertexManager::CreateMeshVtxBuffer(entt::registry& registry, entt::entity entity)
{
	// コンポーネントの取得
	auto& MeshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 頂点数の設定
	MeshVtx.nNumVertex = (MeshVtx.nSegmentU + 1) * (MeshVtx.nSegmentV + 1);

	// ポリゴン数の設定
	MeshVtx.nNumPolygon = ((MeshVtx.nSegmentU * MeshVtx.nSegmentV) * 2) + (4 * (MeshVtx.nSegmentV - 1));

	// インデックス数の設定
	MeshVtx.nNumIndex = MeshVtx.nNumPolygon + 2;

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateMeshVertexBuffer_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MeshVtx.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * MeshVtx.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// メッシュの頂点の生成(マルチテクスチャ)
//===================================================
HRESULT VertexManager::CreateMeshVtxBufferMT(VertexBufferComponent& vertexBufferComp, MeshVtxComponent& meshVtxComp)
{
	// 頂点数の設定
	meshVtxComp.nNumVertex = (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);

	// ポリゴン数の設定
	meshVtxComp.nNumPolygon = ((meshVtxComp.nSegmentU * meshVtxComp.nSegmentV) * 2) + (4 * (meshVtxComp.nSegmentV - 1));

	// インデックス数の設定
	meshVtxComp.nNumIndex = meshVtxComp.nNumPolygon + 2;

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateMeshVertexBuffer_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULT) * meshVtxComp.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3DMT,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * meshVtxComp.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 頂点バッファの生成
//===================================================
HRESULT VertexManager::CreateMeshVtxBuffer(entt::registry& registry, entt::entity entity, const int nNumVertex, const int nNumIndex)
{
	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateMesVertexBuffer_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// スプライトの頂点バッファの生成
//===================================================
HRESULT VertexManager::CreateSpriteVtxBuffer(VertexBufferComponent& vertexBufferComp)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateSpriteVertexBuffer_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 3Dポリゴンの頂点バッファの生成
//===================================================
HRESULT VertexManager::CreateQuadVtxBuffer(entt::registry& registry, entt::entity entity)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateQuadVertexBuffer_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// マルチテクスチャ3Dの頂点バッファの生成
//===================================================
HRESULT VertexManager::CreateQuadVtxBufferMT(VertexBufferComponent& vertexBufferComp)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CreateQuadVertexBufferMT_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULT) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3DMT,
		D3DPOOL_MANAGED,
		&vertexBufferComp.pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// インデックスの設定(グリッド)
//===================================================
void VertexManager::GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIndxNum = nSegmentU + 1;//X

	int nIdxCnt = 0;//配列

	int nNum = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nSegmentV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegmentU; IndxCount2++, nIndxNum++, nNum++)
		{
			// インデックスバッファの設定
			pIdx[nIdxCnt] = static_cast<WORD>(nIndxNum);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nNum);
			nIdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < nSegmentV - 1)
		{
			pIdx[nIdxCnt] = static_cast<WORD>(nNum - 1);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nIndxNum);
			nIdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();
}

//===================================================
// グリッドのインデックスの設定(オフセット指定)
//===================================================
int VertexManager::GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV, const int nOffsetIndex, const int nOffsetVertex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIndxNum = nSegmentU + 1;//X

	int nIdxCnt = nOffsetIndex;//配列

	int nNum = 0;//

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nSegmentV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegmentU; IndxCount2++, nIndxNum++, nNum++)
		{
			// インデックスバッファの設定
			pIdx[nIdxCnt] = static_cast<WORD>(nIndxNum + nOffsetVertex);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nNum + nOffsetVertex);
			nIdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < nSegmentV - 1)
		{
			pIdx[nIdxCnt] = static_cast<WORD>(nNum - 1 + nOffsetVertex);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nIndxNum + nOffsetVertex);
			nIdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();

	return nIdxCnt;
}

//===================================================
// 扇形のインデックス
//===================================================
void VertexManager::FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int* pOffsetIndex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIdx = 0;

	for (int nCnt = 0; nCnt <= nSegmentU; nCnt++)
	{
		pIdx[nIdx++] = static_cast<WORD>(nCnt);
	}

	pIdx[nIdx++] = 1;

	if (pOffsetIndex != nullptr)
	{
		// インデックスのオフセットを設定
		*pOffsetIndex = nIdx;
	}
	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();
}

//===================================================
// 扇形のインデックス(オフセット指定)
//===================================================
void VertexManager::FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int nOffsetVertex, int nOffsetIndex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIdx = nOffsetIndex;

	for (int nCnt = nSegmentU + 1; nCnt >= 0; nCnt--)
	{
		pIdx[nIdx++] = static_cast<WORD>(nCnt + nOffsetVertex);
	}

	pIdx[nIdx++] = static_cast<WORD>(nOffsetVertex + nSegmentU);

	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();
}