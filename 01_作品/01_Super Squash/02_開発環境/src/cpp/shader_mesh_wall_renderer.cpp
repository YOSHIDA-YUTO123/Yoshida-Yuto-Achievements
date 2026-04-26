//===================================================
//
// シェーダーを適応するメッシュの壁の描画処理 [shader_mesh_wall_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader_mesh_wall_renderer.h"
#include "tag_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "texture_mt_manager.h"
#include "entity_manager.h"
#include "renderer_component.hpp"
#include "renderer_manager.h"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"
#include "renderer_mt_key_component.hpp"
#include "shader.h"
#include "shader_manager.h"

//===================================================
// 描画処理
//===================================================
void ShaderMeshWallRenderer::Renderer(entt::registry& registry)
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

	// シェーダーマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// 取得できないなら処理しない
	if (pShaderManager == nullptr) return;

	// 壁のシェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_WALL_EFFECT);

	// 取得できないなら処理しない
	if (pShader == nullptr) return;

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 壁の取得
	auto view = registry.view<RendererTag::ShaderMeshWallTag>();
	
	pShader->Begin();

	// 取得した分回す
	for (auto entity : view)
	{
		// 使用できないなら処理を飛ばす
		if (!registry.valid(entity))
		{
			continue;
		}

		// コンポーネントの取得
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		//// ワールドマトリックスを適応
		//pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		// 頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//// テクスチャの設定
		//pDevice->SetTexture(0, pTextureMTManager->GetAdress(pRendererMTKeyComp->key.c_str()));

		//// レンダーステーとの設定
		//RendererManager::SetRenderState(pDevice, pRendererComp);

		// 適応処理
		pShader->Apply(registry, entity);

		pShader->BeginPass();

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, meshVtxComp.nNumVertex, 0, meshVtxComp.nNumPolygon);

		pShader->EndPass();

		//// レンダーステートのリセット
		//RendererManager::ResetRenderState(pDevice, pRendererComp);
	}

	pShader->End();
}