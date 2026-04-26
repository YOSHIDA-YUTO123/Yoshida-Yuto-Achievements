//===================================================
//
// メッシュの壁の描画処理 [mesh_wall_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_wall_renderer.h"
#include "texture_manager.h"
#include "renderer.h"
#include "entity_manager.h"
#include "tag_component.hpp"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"
#include "manager.h"
#include "texture_mt_component.hpp"
#include "renderer_manager.h"
#include "renderer_component.hpp"
#include "tag_component.hpp"
#include "ui_wall_component.hpp"
#include "shader.h"
#include "shader_manager.h"
#include "texture_mrt_manager.h"
#include "wall_constants.hpp"
#include "entity_name_component.hpp"

//===================================================
// 描画処理
//===================================================
void MeshWallRenderer::Renderer(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	auto view = EntityManager::GetSortLayerEntity<Tag::MeshWallTag>(registry);

	//// マルチテクスチャのレンダーステーとの設定
	//RendererManager::SetRendererStateTextureMT(pDevice);

	// MRTテクスチャの取得
	CTextureMRTManager* pTextureMRTManager = pManager->GetTextureMRTManager();

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// シェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_SHADOW_MAP_RECIEVE_MT);

	pShader->Begin();

	// エンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& uiWallComp = registry.get<UIWallComponent>(entity);
		auto& rendererComp = registry.get<RendererComponent>(entity);
		auto& nameComp = registry.get<EntityNameComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D_MULT));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		// テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3DMT);

		pShader->BeginPass();

		pShader->Apply(&registry,entity);

		// テクスチャの設定
		pDevice->SetTexture(0, uiWallComp.pTexture);
		pDevice->SetTexture(1, uiWallComp.pTextureMT);

		if (nameComp.name != WallConst::TOP_WALL)
		{
			pDevice->SetTexture(2, pTextureMRTManager->GetAdress(CTextureMRTManager::TYPE_SHADOW_MAP));
		}

		// レンダーステーとの設定
		RendererManager::SetRenderState(pDevice, &rendererComp);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, meshVtxComp.nNumVertex, 0, meshVtxComp.nNumPolygon);

		pShader->EndPass();

		// レンダーステートのリセット
		RendererManager::ResetRenderState(pDevice, &rendererComp);
	}

	pShader->End();

	//// マルチテクスチャのリセット
	//RendererManager::ResetRendererStateTextureMT(pDevice);
}