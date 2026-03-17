//===================================================
//
// アウトラインを適応するモデルの描画処理 [outline_model_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "outline_model_renderer.h"
#include "shader.h"
#include "model_manager.h"
#include "manager.h"
#include "renderer.h"
#include "shader_manager.h"
#include "tag_component.hpp"
#include "model_component.hpp"
#include "transform_component.hpp"

//===================================================
// 描画処理
//===================================================
void OutLineModelRenderer::Renderer(entt::registry& registry)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || pTextureManager == nullptr)
	{
		return;
	}

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
	
	// 指定したentityの取得
	auto view = registry.view<RendererTag::OutLineModelTag>();

	// カリングを裏面描画に変更
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// コンポーネントを調べる
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& modelComp = registry.get<ModelComponent>(entity);

		// 取得できなかったら処理しない
		if (modelComp.nModelID == CModelManager::INVALID_ID)
		{
			continue;
		}

		// モデルマネージャーの取得
		CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			pShader->Apply(&registry, entity);

			pShader->BeginPass();

			//モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);

			pShader->EndPass();
		}
	}

	// カリングを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// エフェクト終了
	pShader->End();
}