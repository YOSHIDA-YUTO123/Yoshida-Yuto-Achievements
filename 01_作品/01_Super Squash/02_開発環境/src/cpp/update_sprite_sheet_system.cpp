//===================================================
//
// スプライトシートの更新処理 [update_sprite_sheet_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_sprite_sheet_system.h"
#include "sheet_component.hpp"
#include "vertex_buffer_component.hpp"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "color_component.hpp"
#include "tag_component.hpp"

//===================================================
// 更新処理
//===================================================
void UpdateSpriteSheetSystem::Update(entt::registry& registry)
{
	// スプライトシートの取得
	auto view = registry.view<Tag::SpriteSheetTag>();

	// スプライトシート分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& sizeComp = registry.get<Size2DComponent>(entity);
		auto& TransformComp = registry.get<Transform2DComponent>(entity);
		auto& colorComp = registry.get<ColorComponent>(entity);
		auto& spriteSheetComp = registry.get<SheetComponent>(entity);

		// 頂点情報のポインタ
		VERTEX_2D* pVtx;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 対角線の長さを求める
		float fLength = sqrtf((sizeComp.size.x * sizeComp.size.x) + (sizeComp.size.y * sizeComp.size.y));

		// 傾きを計算
		float fAngle = atan2f(sizeComp.size.x, sizeComp.size.y);

		// 位置の取得
		D3DXVECTOR2 pos = TransformComp.pos;

		// 頂点座標の設定
		pVtx[0].pos.x = pos.x + sinf(TransformComp.rot.z - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf(TransformComp.rot.z - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + sinf(TransformComp.rot.z + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf(TransformComp.rot.z + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + sinf(TransformComp.rot.z - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf(TransformComp.rot.z - fAngle) * fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + sinf(TransformComp.rot.z + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf(TransformComp.rot.z + fAngle) * fLength;
		pVtx[3].pos.z = 0.0f;

		//// 頂点座標の設定
		//pVtx[0].pos = D3DXVECTOR3(pos.x - sizeComp.size.x, pos.y - sizeComp.size.y, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(pos.x + sizeComp.size.x, pos.y - sizeComp.size.y, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(pos.x - sizeComp.size.x, pos.y + sizeComp.size.y, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(pos.x + sizeComp.size.x, pos.y + sizeComp.size.y, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = colorComp.col;
		pVtx[1].col = colorComp.col;
		pVtx[2].col = colorComp.col;
		pVtx[3].col = colorComp.col;

		// テクスチャ座標の設定
		pVtx[0].tex = spriteSheetComp.aPosUV[0];
		pVtx[1].tex = spriteSheetComp.aPosUV[1];
		pVtx[2].tex = spriteSheetComp.aPosUV[2];
		pVtx[3].tex = spriteSheetComp.aPosUV[3];

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}