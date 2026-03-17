//===================================================
//
// 球体メッシュの描画処理 [mesh_sphere_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_sphere_renderer.h"
#include "tag_component.hpp"
#include "transform_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"
#include "texture_id_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "mesh_sphere_component.hpp"
#include "renderer_component.hpp"
#include "renderer_manager.h"
#include "motion_blur_component.hpp"
#include "color_constants.hpp"
#include "utility_math.h"

//===================================================
// 描画処理
//===================================================
void MeshSphereRenderer::Renderer(entt::registry& registry)
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

	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::MeshSphereTag>();

	for (auto entity : view)
	{
		// モーションブラーの描画	
		DrawMotionBlur(registry, entity);

		// コンポーネントの取得
		auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto pRendererComp = registry.try_get<RendererComponent>(entity);

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

//===================================================
// モーションブラーの描画
//===================================================
void MeshSphereRenderer::DrawMotionBlur(entt::registry& registry, const entt::entity entity)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントの取得
	auto pMotionBlurComp = registry.try_get<MotionBlurComponent>(entity);

	// モーションブラーを持っていないなら
	if (pMotionBlurComp == nullptr)
	{
		return;
	}

	// 表示しないなら
	if (!pMotionBlurComp->bShow)
	{
		return;
	}

	// コンポーネントの取得
	auto& meshSphereComp	= registry.get<MeshSphereComponent>(entity);
	auto& vertexBufferComp	= registry.get<VertexBufferComponent>(entity);
	auto& transformComp		= registry.get<Transform3DComponent>(entity);
	auto& meshVtxComp		= registry.get<MeshVtxComponent>(entity);

	// ZWrite無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 蓋の部分の頂点の計算
	int nFanVtx			= meshVtxComp.nSegmentU + 1 + 1;
	int nFanPolygon		= meshVtxComp.nSegmentU;

	// 側面の部分の頂点
	int nSideVtx		= (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);
	int nSidePolygon	= ((meshVtxComp.nSegmentU * meshVtxComp.nSegmentV) * 2) + (4 * (meshVtxComp.nSegmentV - 1));

	// 発生させるブラーの数分回す
	for (int nCnt = 0; nCnt < pMotionBlurComp->nStrength; nCnt++)
	{
		// 割合を求める
		float fRate = nCnt / static_cast<float>(pMotionBlurComp->nStrength - 1);

		// 移動した量を求める
		D3DXVECTOR3 moveLength = transformComp.pos - transformComp.posOld;

		// ブラーの発生位置を求める
		D3DXVECTOR3 blurPos = transformComp.posOld + (moveLength * fRate);

		// 計算用マトリックス
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans, mtxScal;

		// マトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 回転・平行移動を求める
		D3DXMatrixRotationQuaternion(&mtxRot, &transformComp.quaternion);
		D3DXMatrixTranslation(&mtxTrans, blurPos.x, blurPos.y, blurPos.z);

		// 行列を掛け合わせる
		mtxWorld = (mtxRot * mtxTrans);

		// ワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < meshVtxComp.nNumVertex; nCntVtx++)
		{
			// 色の取得
			D3DXCOLOR col = pVtx[nCntVtx].col;

			col = Color::WHITE;
			col.a = 0.1f + 0.7f * fRate;

			pVtx[nCntVtx].col = col;
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(vertexBufferComp.pIdxBuffer);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);

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
	}

	// ZWrite無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}