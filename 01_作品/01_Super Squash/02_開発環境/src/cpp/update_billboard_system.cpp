//===================================================
//
// ビルボードの更新の処理 [update_billboard_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_billboard_system.h"
#include "tag_component.hpp"
#include "mesh_vtx_component.hpp"
#include "size_component.hpp"
#include "vertex_buffer_component.hpp"
#include "color_component.hpp"
#include "sheet_component.hpp"

//===================================================
// 更新処理
//===================================================
void UpdateBillboardSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::BillboardTag>();

	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& sizeComp = registry.get<Size2DComponent>(entity);
		auto& colorComp = registry.get<ColorComponent>(entity);
		auto pSheetComp = registry.try_get<SheetComponent>(entity);

		// 頂点情報のポインタ
		VERTEX_3D* pVtx;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 大きさの取得
		D3DXVECTOR2 Size = sizeComp.size;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = colorComp.col;
		pVtx[1].col = colorComp.col;
		pVtx[2].col = colorComp.col;
		pVtx[3].col = colorComp.col;

		// 通常なら
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