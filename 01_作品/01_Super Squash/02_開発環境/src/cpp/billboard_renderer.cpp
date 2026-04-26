//===================================================
//
// ビルボードの描画処理 [billboard_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "billboard_renderer.h"
#include "transform_component.hpp"
#include "layer_component.hpp"
#include "entity_manager.h"
#include "renderer.h"
#include "manager.h"
#include "texture_manager.h"
#include "texture_id_component.hpp"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "renderer_component.hpp"
#include "renderer_manager.h"

//===================================================
// 描画処理
//===================================================
void BillboardRenderer::Renderer(entt::registry& registry)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "BillboardRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// entityの数分回す
	for (auto entity : registry.view<Tag::BillboardTag>())
	{
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto rendererComp = registry.try_get<RendererComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(textureIDComp.nTextureID));

		// レンダーステートの設定
		RendererManager::SetRenderState(pDevice, rendererComp);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// レンダーステートのリセット
		RendererManager::ResetRenderState(pDevice,rendererComp);
	}
}