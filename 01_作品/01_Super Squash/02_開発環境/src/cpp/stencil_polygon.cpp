//===================================================
//
// 画面のポリゴンの処理 [stencil_polygon.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "stencil_polygon.h"
#include "renderer.h"
#include "manager.h"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"

//===================================================
// 描画処理
//===================================================
void StencilPolygonRenderer::Renderer(entt::registry& registry)
{
	// ステンシルポリゴンの取得
	auto stencilView = registry.view<Tag::StencilPolygonTag>();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 取得した分回す
	for (auto entity : stencilView)
	{
		// コンポーネントの取得
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		// ステンシルテストを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// Zバッファへの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// カラーバッファへの書き込みを有効にする
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

		// ステンシルバッファの参照値を設定("2"にする)
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

		// ステンシルバッファの比較パラメータの設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); // ステンシルテスト合格、zテスト合格
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP); // ステンシルテスト合格、zテスト不合格
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // ステンシルテスト不合格

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャがない
		pDevice->SetTexture(0, NULL);

		// プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類	

		// ステンシルテストを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		// Zバッファへの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// カラーバッファをもとに戻す
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

		// 表面をカリング
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}
