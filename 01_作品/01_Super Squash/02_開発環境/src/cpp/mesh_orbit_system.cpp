//===================================================
//
// メッシュの軌跡の処理 [mesh_orbit_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_orbit_system.h"
#include "mesh_orbit_component.hpp"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "transform_component.hpp"
#include "factory_billboard.h"

//===================================================
// 更新処理
//===================================================
void MeshOrbitSystem::Update(entt::registry& registry)
{
	VERTEX_3D* pVtx = nullptr;

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<MeshOrbitComponent>();

	// エンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshOrbitComp = registry.get<MeshOrbitComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

		if (meshOrbitComp.ownerID == entt::null)
		{
			continue;
		}

		auto& OwnerTransformComp = registry.get<Transform3DComponent>(meshOrbitComp.ownerID);

		D3DXMATRIX mtxTrans = OwnerTransformComp.mtxWorld;

		// ワールド座標に変換
		D3DXVec3TransformCoord(&meshOrbitComp.top, &meshOrbitComp.topLocal, &OwnerTransformComp.mtxWorld);
		D3DXVec3TransformCoord(&meshOrbitComp.bottom, &meshOrbitComp.bottomLocal, &OwnerTransformComp.mtxWorld);

		// 頂点バッファをロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 四個前から始める
		for (int nCnt = meshVtxComp.nNumVertex - 4; nCnt >= 0; nCnt -= 2)
		{
			// 前の頂点座標を入れる
			D3DXVECTOR3 OldVtx0 = pVtx[nCnt].pos;
			D3DXVECTOR3 OldVtx1 = pVtx[nCnt + 1].pos;

			// 頂点座標の設定
			pVtx[nCnt + 2].pos = OldVtx0;
			pVtx[nCnt + 3].pos = OldVtx1;

		}

		// 新しい頂点を追加（先頭に入れる）
		pVtx[0].pos = meshOrbitComp.bottom;
		pVtx[1].pos = meshOrbitComp.top;

		// 頂点数分回す
		for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
		{
			// アルファ値の設定
			float fAlv = meshVtxComp.col.a * (1.0f - (float)nCnt / meshVtxComp.nNumVertex);

			// 色の変更
			pVtx[nCnt].col = D3DXCOLOR(meshVtxComp.col.r, meshVtxComp.col.g, meshVtxComp.col.b, fAlv);
		}

		// 頂点バッファをアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}
