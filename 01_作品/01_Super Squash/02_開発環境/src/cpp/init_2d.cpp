//===================================================
//
// メッシュの初期化処理 [init_2d.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "init_2d.h"
#include "vertex_build.h"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "color_component.hpp"
#include "vertex_build.h"
#include "vertex_buffer_component.hpp"

//===================================================
// スプライトの初期化処理
//===================================================
HRESULT Init2D::Sprite(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// スプライトの生成処理
	if (FAILED(VertexManager::CreateSpriteVtxBuffer(vertexBufferComp)))
	{
		return E_FAIL;
	}
	auto& sizeComp = registry.get<Size2DComponent>(entity);
	auto& TransformComp = registry.get<Transform2DComponent>(entity);
	auto& colorComp = registry.get<ColorComponent>(entity);

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

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = colorComp.col;
	pVtx[1].col = colorComp.col;
	pVtx[2].col = colorComp.col;
	pVtx[3].col = colorComp.col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / nSegmentU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / nSegmentV);
	pVtx[3].tex = D3DXVECTOR2(1.0f / nSegmentU, 1.0f / nSegmentV);

	// 頂点バッファのアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}