//===================================================
//
// アウトラインを適応する球体メッシュの描画処理 [outline_mesh_SphereRenderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "outline_mesh_sphere_renderer.h"
#include "shader_manager.h"
#include "shader.h"
#include "manager.h"
#include "renderer.h"
#include "transform_component.hpp"
#include "mesh_sphere_component.hpp"
#include "tag_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"
#include "renderer_component.hpp"
#include "renderer_manager.h"
#include "outline_shader_component.hpp"

//===================================================
// 描画処理
//===================================================
void OutLineMeshSphereRenderer::Renderer(entt::registry& registry)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "OutLineMeshSphere_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = CManager::GetInstance()->GetShaderManager();

	// 取得できないなら処理しない
	if (pShaderManager == nullptr) return;

	// アウトラインシェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_OUTLINE);

	// 取得できないなら処理しない
	if (pShader == nullptr) return;

	// エフェクト開始
	pShader->Begin();

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<RendererTag::OutLineSphereTag>();

	for (auto entity : view)
	{
		auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto rendererComp = registry.try_get<RendererComponent>(entity);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// 蓋の部分の頂点の計算
		int nFanVtx = meshVtxComp.nSegmentU + 1 + 1;
		int nFanPolygon = meshVtxComp.nSegmentU;

		// 側面の部分の頂点
		int nSideVtx = (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);
		int nSidePolygon = ((meshVtxComp.nSegmentU * meshVtxComp.nSegmentV) * 2) + (4 * (meshVtxComp.nSegmentV - 1));

		// レンダーステートの設定
		RendererManager::SetRenderState(pDevice, rendererComp);

		// カリングを裏面描画に変更
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// アウトラインシェーダーの適応
		pShader->Apply(&registry, entity);

		pShader->BeginPass();

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

		// カリングを元に戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// レンダーステートの設定
		RendererManager::ResetRenderState(pDevice, rendererComp);
	}

	// エフェクト終了
	pShader->End();
}
