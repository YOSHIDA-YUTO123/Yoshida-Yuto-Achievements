//===================================================
//
// アウトラインを適応するキャラクターの描画処理 [outline_character_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "outline_character_renderer.h"
#include "shader.h"
#include "shader_manager.h"
#include "manager.h"
#include "renderer.h"
#include "model_manager.h"
#include "shader_outline.h"
#include "character_model_component.hpp"
#include "transform_component.hpp"
#include "outline_shader_component.hpp"
#include "tag_component.hpp"

//===================================================
// 描画処理
//===================================================
void OutLineCharacterRenderer::Renderer(entt::registry& registry)
{
	// モデルマネージャーの取得
	CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "OutLineCharacterRenderer_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = CManager::GetInstance()->GetShaderManager();

	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	// 取得できないなら処理しない
	if (pShaderManager == nullptr) return;

	// アウトラインシェーダーの取得
	CShader* pShader = pShaderManager->Get(CShaderManager::TYPE_OUTLINE);

	// 取得できないなら処理しない
	if (pShader == nullptr) return;

	// エフェクト開始
	pShader->Begin();

	// キャラクターモデルの取得
	auto view = registry.view<RendererTag::OutLineCharacterTag>();

	// コンポーネント分回す
	for (auto entity : view)
	{
		auto& modelComp = registry.get<CharacterModelComponent>(entity);

		if (modelComp.nModelID == CModelManager::INVALID_ID)
		{
			continue;
		}

		// モデルの情報の取得
		CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(modelComp.nModelID);

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			// カリングを裏面描画に変更
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

			// シェーダーの適応処理
			pShader->Apply(&registry, entity);

			pShader->BeginPass();

			pDevice->SetTexture(0, nullptr);

			// モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);

			pShader->EndPass();

			// カリングを元に戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
		}
	}

	// エフェクト終了
	pShader->End();
}