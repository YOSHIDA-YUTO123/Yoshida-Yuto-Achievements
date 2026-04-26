//===================================================
//
// ステンシルシャドウの描画処理 [stencil_shadow_renderer.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "stencil_shadow_renderer.h"
#include "transform_component.hpp"
#include "model_component.hpp"
#include "tag_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "model_manager.h"

//===================================================
// 描画処理
//===================================================
void StencilShadowRenderer::Renderer(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::StencilTag>();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// エンティティ分回す
	for (auto entity : view)
	{
		// マトリックスの設定
		SetMatrix(registry, entity, pDevice);

		// ステンシルテストを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// Zバッファへの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// カラーバッファへの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

		// ステンシルバッファの比較パラメータの設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR); // ステンシルテスト合格、zテスト合格
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT); // ステンシルテスト合格、zテスト不合格
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO); // ステンシルテスト不合格

		// 表面をカリング
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// モデルの描画処理
		Draw(registry, entity, pDevice);

		// ステンシルバッファの参照値を設定("1"にする)
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

		// ステンシルバッファの比較パラメータの設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT); // ステンシルテスト合格、zテスト合格
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);	  // ステンシルテスト合格、zテスト不合格
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO); // ステンシルテスト不合格

		// 裏面をカリング
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// モデルの描画処理
		Draw(registry, entity, pDevice);

		// カラーバッファをもとに戻す
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

		// ステンシルテストを無効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		// Zバッファへの書き込みを有効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 表面をカリング
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//===================================================
// マトリックスの設定処理
//===================================================
void StencilShadowRenderer::SetMatrix(entt::registry& registry, const entt::entity entity, LPDIRECT3DDEVICE9 pDevice)
{
	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	auto& transformComp = registry.get<Transform3DComponent>(entity);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);
}

//===================================================
// 描画処理
//===================================================
void StencilShadowRenderer::Draw(entt::registry& registry, const entt::entity entity, LPDIRECT3DDEVICE9 pDevice)
{
	// コンポーネントの取得
	auto& modelComp = registry.get<ModelComponent>(entity);

	// 取得できなかったら処理しない
	if (modelComp.nModelID == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		//モデル(パーツ)の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}
