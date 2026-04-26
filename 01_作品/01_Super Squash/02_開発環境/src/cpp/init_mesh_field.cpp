//===================================================
//
// メッシュフィールドの初期化処理 [init_mesh_field.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "init_mesh_field.h"
#include "vertex_buffer_component.hpp"
#include "vertex_build.h"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"

//===================================================
// メッシュフィールドの初期化処理
//===================================================
HRESULT InitMeshField::Field(entt::registry& registry, entt::entity entity)
{
	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& sizeComp = registry.get<Size3DComponent>(entity);
	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / meshVtx.nSegmentU;
	float fTexPosY = 1.0f / meshVtx.nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= meshVtx.nSegmentV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= meshVtx.nSegmentU; nCntX++)
		{
			// 位置の設定
			posWk.x = ((sizeComp.size.x / meshVtx.nSegmentU) * nCntX) - (sizeComp.size.x * 0.5f);
			posWk.y = sizeComp.size.y;
			posWk.z = sizeComp.size.z - ((sizeComp.size.z / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.z * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = Const::VEC_U;

			//頂点カラーの設定
			pVtx[nCntVtx].col = meshVtx.col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}

//===================================================
// マルチテクスチャのメッシュフィールド
//===================================================
HRESULT InitMeshField::FieldMT(entt::registry& registry, entt::entity entity)
{
	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBufferMT(vertexBufferComp, meshVtx)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& sizeComp = registry.get<Size3DComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / meshVtx.nSegmentU;
	float fTexPosY = 1.0f / meshVtx.nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D_MULT* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 縦の分割数の設定
	for (int nCntZ = 0; nCntZ <= meshVtx.nSegmentV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= meshVtx.nSegmentU; nCntX++)
		{
			// 位置の設定
			posWk.x = ((sizeComp.size.x / meshVtx.nSegmentU) * nCntX) - (sizeComp.size.x * 0.5f);
			posWk.y = sizeComp.size.y;
			posWk.z = sizeComp.size.z - ((sizeComp.size.z / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.z * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntVtx].col = meshVtx.col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}
