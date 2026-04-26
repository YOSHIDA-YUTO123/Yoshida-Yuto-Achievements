//===================================================
//
// 3Dポリゴンの更新処理 [mesh_quad_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_quad_system.h"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "size_component.hpp"
#include "color_component.hpp"
#include "sheet_component.hpp"

//===================================================
// 更新処理
//===================================================
void MeshQuadSystem::Update(entt::registry& registry)
{
	// 3Dポリゴンのシートの取得
	auto view = registry.view<Tag::MeshQuadTag>();

	// 3Dポリゴンのシート分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& sizeComp = registry.get<Size3DComponent>(entity);
		auto pColorComp = registry.try_get<ColorComponent>(entity);
		auto pSheetComp = registry.try_get<SheetComponent>(entity);

		// 頂点情報のポインタ
		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-sizeComp.size.x, sizeComp.size.y, sizeComp.size.z);
		pVtx[1].pos = D3DXVECTOR3(sizeComp.size.x, sizeComp.size.y, sizeComp.size.z);
		pVtx[2].pos = D3DXVECTOR3(-sizeComp.size.x, -sizeComp.size.y, -sizeComp.size.z);
		pVtx[3].pos = D3DXVECTOR3(sizeComp.size.x, -sizeComp.size.y, -sizeComp.size.z);

		if (pColorComp == nullptr)
		{
			// 頂点カラーの設定
			pVtx[0].col = Const::WHITE;
			pVtx[1].col = Const::WHITE;
			pVtx[2].col = Const::WHITE;
			pVtx[3].col = Const::WHITE;
		}
		else
		{
			// 頂点カラーの設定
			pVtx[0].col = pColorComp->col;
			pVtx[1].col = pColorComp->col;
			pVtx[2].col = pColorComp->col;
			pVtx[3].col = pColorComp->col;
		}

		if (pSheetComp == nullptr)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			// テクスチャ座標の設定
			pVtx[0].tex = pSheetComp->aPosUV[0];
			pVtx[1].tex = pSheetComp->aPosUV[1];
			pVtx[2].tex = pSheetComp->aPosUV[2];
			pVtx[3].tex = pSheetComp->aPosUV[3];
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}