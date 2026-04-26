//===================================================
//
//メッシュの球体の描画(MRT)の描画処理 [mesh_sphere_renderer_mrt.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_sphere_renderer_mrt.h"
#include "renderer.h"
#include "manager.h"
#include "tag_component.hpp"
#include "mesh_sphere_component.hpp"
#include "texture_id_component.hpp"
#include "vertex_buffer_component.hpp"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "texture_manager.h"
#include "renderer_component.hpp"
#include "mrt_target_component.hpp"
#include "renderer_manager.h"

//===================================================
// 描画処理
//===================================================
void MeshSphereRendererMRT::Renderer(entt::registry& registry, const int nType)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "MeshSphereRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<RendererTag::MeshSphereMRT>();

	for (auto entity : view)
	{
		auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto pRendererComp = registry.try_get<RendererComponent>(entity);
		auto pMRTTargetComp = registry.try_get<MRTTargetComponent>(entity);

		// nullだったら処理を飛ばす
		if (pMRTTargetComp == nullptr)
		{
			continue;
		}

		// 種類が違うなら処理を飛ばす
		if (pMRTTargetComp != nullptr && pMRTTargetComp->nTarget != nType)
		{
			continue;
		}

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

		// レンダーステートの設定
		RendererManager::SetRenderState(pDevice, pRendererComp);

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

		// レンダーステートの設定
		RendererManager::ResetRenderState(pDevice, pRendererComp);
	}
}