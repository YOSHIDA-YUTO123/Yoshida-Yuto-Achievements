//===================================================
//
// メッシュドームの描画処理 [mesh_dome_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_dome_renderer.h"
#include "mesh_dome_component.hpp"
#include "mesh_vtx_component.hpp"
#include "tag_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "texture_id_component.hpp"
#include "vertex_buffer_component.hpp"
#include "texture_manager.h"
#include "renderer_component.hpp"
#include "transform_component.hpp"

//===================================================
// 描画処理
//===================================================
void MeshDomeRenderer::Renderer(entt::registry& registry)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshDomeRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::MeshDomeTag>();

	for (auto entity : view)
	{
		auto& domeComp = registry.get<MeshDomeComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& rendererComp = registry.get<RendererComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		auto& TopVtxComp = registry.get<MeshVtxComponent>(domeComp.vtxEntity[MeshDomeComponent::TYPE_TOP]);
		auto& SideVtxComp = registry.get<MeshVtxComponent>(domeComp.vtxEntity[MeshDomeComponent::TYPE_SIDE]);

		// ワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(textureIDComp.nTextureID));

		// フラグがonだったら
		if (rendererComp.nFlag & RendererComponent::TYPE_LIGHT_OFF)
		{
			// ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, TopVtxComp.nNumVertex, 0, TopVtxComp.nNumPolygon);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SideVtxComp.nNumVertex, TopVtxComp.nNumIndex, SideVtxComp.nNumPolygon);

		// ライトをもとに戻すにする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}