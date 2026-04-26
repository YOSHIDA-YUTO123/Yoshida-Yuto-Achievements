//===================================================
//
// モデルの描画systemの処理 [model_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "model_renderer.h"
#include "model_component.hpp"
#include "transform_component.hpp"
#include "texture_manager.h"
#include "model_manager.h"
#include "manager.h"
#include "renderer.h"
#include "entity_manager.h"
#include "tag_component.hpp"

//===================================================
// 描画処理
//===================================================
void ModelRenderer::Renderer(entt::registry& registry)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

		// 取得できなかったら処理しない
	if (pModelManager == nullptr || pTextureManager == nullptr)
	{
		return;
	}

	// レイヤー順にソートしたenityの取得
	auto EntityList = EntityManager::GetSortLayerEntity<Tag::ModelTag>(registry);

	// コンポーネントを調べる
	for (auto entity : EntityList)
	{
		// コンポーネントの取得
		auto& modelComp = registry.get<ModelComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// 取得できなかったら処理しない
		if (modelComp.nModelID == -1)
		{
			return;
		}

		// モデルマネージャーの取得
		CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			if (modelInfo.nTextureIdx.empty())
			{
				break;
			}

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));

			//モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);
	}
}