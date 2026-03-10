//===================================================
//
// モデルの描画(MRT)の処理 [model_renderer_mrt.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "model_renderer_mrt.h"
#include "manager.h"
#include "texture_manager.h"
#include "model_manager.h"
#include "renderer.h"
#include "entity_manager.h"
#include "model_component.hpp"
#include "mrt_target_component.hpp"
#include "tag_component.hpp"
#include "transform_component.hpp"

//===================================================
// 描画処理
//===================================================
void ModelRendererMRT::Renderer(entt::registry& registry, const int nType)
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
	auto EntityList = EntityManager::GetSortLayerEntity<RendererTag::ModelMRT>(registry);

	// コンポーネントを調べる
	for (auto entity : EntityList)
	{
		// コンポーネントの取得
		auto& modelComp = registry.get<ModelComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto pMRTRendererComp = registry.try_get<MRTTargetComponent>(entity);

		// nullなら処理をしない
		if (pMRTRendererComp == nullptr)
		{
			continue;
		}

		// 種類が違うなら処理を飛ばす
		if (pMRTRendererComp != nullptr && pMRTRendererComp->target != nType)
		{
			continue;
		}

		// 取得できなかったら処理しない
		if (modelComp.nModelID == CModelManager::INVALID_ID)
		{
			return;
		}

		// モデルマネージャーの取得
		CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		// マテリアル分回す
		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			// マテリアルがないなら処理をしない(例外スロー対策)
			if (modelInfo.nTextureIdx.empty())
			{
				break;
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));

			// モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);
	}
}