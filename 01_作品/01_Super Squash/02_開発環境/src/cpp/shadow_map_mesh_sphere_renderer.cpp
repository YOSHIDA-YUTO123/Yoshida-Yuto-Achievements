//===================================================
//
// 影を落とすメッシュの球体の描画処理 [shadow_map_mesh_sphere_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_map_mesh_sphere_renderer.h"
#include "manager.h"
#include "renderer.h"
#include "tag_component.hpp"
#include "mesh_sphere_component.hpp"
#include "texture_id_component.hpp"
#include "vertex_buffer_component.hpp"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "texture_manager.h"
#include "shader.h"
#include "shader_manager.h"
#include "texture_mrt_manager.h"

//===================================================
// 描画処理
//===================================================
void ShadowMapMeshSphereRenderer::Renderer(entt::registry& registry, const int nType)
{
	// 種類が違うなら処理を飛ばす
	if (nType != CTextureMRTManager::TYPE_SHADOW_MAP)
	{
		return;
	}

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshSphereRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<RendererTag::ShadowMapSphere>();

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// シェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_SHADOW_MAP);

	pShader->Begin();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

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

		// 蓋の部分の頂点の計算
		int nFanVtx = meshVtxComp.nSegmentU + 1 + 1;
		int nFanPolygon = meshVtxComp.nSegmentU;

		// 側面の部分の頂点
		int nSideVtx = (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);
		int nSidePolygon = ((meshVtxComp.nSegmentU * meshVtxComp.nSegmentV) * 2) + (4 * (meshVtxComp.nSegmentV - 1));

		pShader->BeginPass();

		pShader->Apply(&registry,entity);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLEFAN,
			0,
			0,
			nFanVtx,
			0,
			nFanPolygon);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			nSideVtx,
			meshSphereComp.nSideStartIndex,
			nSidePolygon);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLEFAN,
			0,
			0,
			nFanVtx,
			meshSphereComp.nBottomStartIndex,
			nFanPolygon);

		pShader->EndPass();
	}

	pShader->End();
}