//===================================================
//
// 影を受けるフィールドの描画 [shadowmap_recieve_field_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadowmap_recieve_field_renderer.h"
#include "texture_manager.h"
#include "texture_mrt_manager.h"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "renderer.h"
#include "entity_manager.h"
#include "manager.h"
#include "shader.h"
#include "shader_manager.h"
#include "texture_id_component.hpp"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"

//===================================================
// 描画処理
//===================================================
void ShadowMapRecieveFIeldRenderer::Renderer(entt::registry& registry)
{
	// マネージャーの取得
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
	auto entityList = EntityManager::GetSortLayerEntity<RendererTag::ShadowMapRecieveField>(registry);

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// シェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_SHADOW_MAP_RECIEVE);

	CTextureMRTManager* pTextureMRTManager = pManager->GetTextureMRTManager();

	pShader->Begin();

	// エンティティ分回す
	for (auto entity : entityList)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		// テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pShader->BeginPass();

		pShader->Apply(&registry,entity);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(textureIDComp.nTextureID));
		pDevice->SetTexture(1, pTextureMRTManager->GetAdress(CTextureMRTManager::TYPE_SHADOW_MAP));

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, meshVtxComp.nNumVertex, 0, meshVtxComp.nNumPolygon);

		pShader->EndPass();
	}
	pShader->End();
}