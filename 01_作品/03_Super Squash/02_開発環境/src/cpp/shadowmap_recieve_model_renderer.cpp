//===================================================
//
// 影のモデルの描画 [shadow_model_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadowmap_recieve_model_renderer.h"
#include "manager.h"
#include "texture_manager.h"
#include "renderer.h"
#include "model_component.hpp"
#include "entity_manager.h"
#include "tag_component.hpp"
#include "shader.h"
#include "shader_manager.h"
#include "model_manager.h"
#include "transform_component.hpp"
#include "texture_mrt_manager.h"
 
//===================================================
// 描画処理
//===================================================
void ShadowMapRecieveModelRenderer::Renderer(entt::registry& registry)
{
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = pManager->GetModelManager();

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
	auto EntityList = EntityManager::GetSortLayerEntity<RendererTag::ShadowMapRecieveField>(registry);

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// シェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_SHADOW_MAP_RECIEVE);

	CTextureMRTManager* pTextureMRTManager = pManager->GetTextureMRTManager();

	pShader->Begin();

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

			pShader->BeginPass();

			// 適応処理
			pShader->Apply(&registry, entity);

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));
			pDevice->SetTexture(1, pTextureMRTManager->GetAdress(CTextureMRTManager::TYPE_SHADOW_MAP));

			// モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);

			pShader->EndPass();
		}

		//保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);
	}

	pShader->End();
}