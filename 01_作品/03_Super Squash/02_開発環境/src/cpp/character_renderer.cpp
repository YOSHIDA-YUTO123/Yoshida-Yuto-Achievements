//===================================================
//
// キャラクターの描画処理 [character_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character_renderer.h"
#include "model_manager.h"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "layer_component.hpp"
#include "transform_component.hpp"
#include "entity_manager.h"
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"
#include "tag_component.hpp"

//===================================================
// 描画処理
//===================================================
void CharacterRenderer::Renderer(entt::registry& registry)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// モデルマネージャーの取得
	CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CharacterRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	// コンポーネント分回す
	for (auto [entity ,modelComp] : registry.view<CharacterModelComponent, RendererTag::CharacterModelTag>().each())
	{
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

		if (modelComp.nModelID == CModelManager::INVALID_ID)
		{
			continue;
		}

		// モデルの情報の取得
		CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

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