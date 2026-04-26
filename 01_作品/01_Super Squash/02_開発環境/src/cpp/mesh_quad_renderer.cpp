//===================================================
//
// 3Dポリゴンの描画処理 [mesh_quad_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_quad_renderer.h"
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"
#include "transform_component.hpp"
#include "entity_manager.h"
#include "texture_id_component.hpp"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "renderer_component.hpp"
#include "renderer_manager.h"
#include "texture_mt_component.hpp"

//===================================================
// 描画処理
//===================================================
void MeshQuadRenderer::Renderer(entt::registry& registry)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshQuadRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// レイヤー順にソートしたenityの取得
	auto EntityList = EntityManager::GetSortLayerEntity<RendererTag::MeshQuadTag> (registry);

	// コンポーネント分回す
	for (auto entity : EntityList)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto pRendererComp = registry.try_get<RendererComponent>(entity);
		auto pTextureMTComp = registry.try_get<TextureMTComponent>(entity);
		auto pTextureIDComp = registry.try_get<TextureIDComponent>(entity);

		// ワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 描画しないなら
		if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_NO_DISP)
		{
			// レンダーステートのリセット
			RendererManager::ResetRenderState(pDevice, pRendererComp);

			// 処理を飛ばす
			continue;
		}

		if (pTextureIDComp != nullptr)
		{
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTextureManager->GetAdress(pTextureIDComp->nTextureID));
		}
		else if (pTextureMTComp != nullptr)
		{
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D_MULT));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3DMT);

			// マルチテクスチャのレンダーステートの設定
			RendererManager::SetRendererStateTextureMT(pDevice);

			// テクスチャの設定
			pDevice->SetTexture(0, pTextureManager->GetAdress(pTextureMTComp->aTextureID[0]));

			// テクスチャの設定
			pDevice->SetTexture(1, pTextureManager->GetAdress(pTextureMTComp->aTextureID[1]));
		}

		// レンダーステートの設定
		RendererManager::SetRenderState(pDevice, pRendererComp);

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// マルチテクスチャのレンダーステートのリセット
		RendererManager::ResetRendererStateTextureMT(pDevice);

		// レンダーステートのリセット
		RendererManager::ResetRenderState(pDevice, pRendererComp);
	}
}