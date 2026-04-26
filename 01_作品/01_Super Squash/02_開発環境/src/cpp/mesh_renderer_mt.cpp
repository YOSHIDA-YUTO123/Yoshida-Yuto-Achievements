//===================================================
//
// マルチテクスチャのメッシュ描画処理 [MeshRendererMT.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_renderer_mt.h"
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"
#include "renderer_manager.h"
#include "entity_manager.h"
#include "mesh_vtx_component.hpp"
#include "transform_component.hpp"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "texture_mt_component.hpp"
#include "renderer_component.hpp"
#include "texture_mrt_manager.h"
#include "texture_id_component.hpp"
#include "shader.h"
#include "shader_manager.h"

//===================================================
// 描画処理
//===================================================
void MeshRendererMT::Renderer(entt::registry& registry)
{
	CManager* pManager = CManager::GetInstance();

	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 指定したコンポーネントを持ってるエンティティのリストの取得
	auto entityList = EntityManager::GetSortLayerEntity<RendererTag::MeshMTTag>(registry);

	// エンティティ分回す
	for (auto entity : entityList)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& textureMTComp = registry.get<TextureMTComponent>(entity);
		auto pRendererComp = registry.try_get<RendererComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D_MULT));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		// テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3DMT);
		
		// テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(textureMTComp.aTextureID[0]));

		// レンダーステーとの設定
		RendererManager::SetRenderState(pDevice, pRendererComp);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, meshVtxComp.nNumVertex, 0, meshVtxComp.nNumPolygon);

		// レンダーステートのリセット
		RendererManager::ResetRenderState(pDevice, pRendererComp);

		// マルチテクスチャのリセット
		RendererManager::ResetRendererStateTextureMT(pDevice);
	}
}