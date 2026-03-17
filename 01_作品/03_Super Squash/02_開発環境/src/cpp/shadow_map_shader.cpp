//===================================================
//
// シャドウマップを適応するシェーダー [shadow_map_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_map_shader.h"
#include "transform_component.hpp"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CShadowMapShader::CShadowMapShader()
{
}

//===================================================
// デストラクタ
//===================================================
CShadowMapShader::~CShadowMapShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShadowMapShader::Init(const char* pShaderFile, const char* pTech)
{
	// 初期化処理
	if (FAILED(CShader::Init(pShaderFile, pTech)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 適応処理
//===================================================
void CShadowMapShader::Apply(const entt::registry* pRegistry, const entt::entity entity)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	CRenderer* pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxView, mtxProj;

	// コンポーネントの取得
	auto& transformComp = pRegistry->get<Transform3DComponent>(entity);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// マトリックスの設定
	CShader::SetMatrix("g_mtxWorld"		, transformComp.mtxWorld);
	CShader::SetMatrix("g_mtxView"		, mtxView);
	CShader::SetMatrix("g_mtxProjection", mtxProj);

	CShader::CommitChange();
}