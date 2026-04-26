//===================================================
//
// シャドウマップを適応するシェーダー [shadow_map_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_map_recieve_shader.h"
#include "transform_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "texture_mrt_manager.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CShadowMapRecieverShader::CShadowMapRecieverShader()
{
}

//===================================================
// デストラクタ
//===================================================
CShadowMapRecieverShader::~CShadowMapRecieverShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShadowMapRecieverShader::Init(const char* pShaderFile, const char* pTech)
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
void CShadowMapRecieverShader::Apply(const entt::registry* pRegistry, const entt::entity entity)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	CTextureMRTManager* pTextureMRTManager = pManager->GetTextureMRTManager();

	CRenderer* pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxView, mtxProj;

	// コンポーネントの取得
	auto& transformComp = pRegistry->get<Transform3DComponent>(entity);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// マトリックスの設定
	CShader::SetMatrix("g_mtxWorld", transformComp.mtxWorld);
	CShader::SetMatrix("g_mtxView", mtxView);
	CShader::SetMatrix("g_mtxProjection", mtxProj);

	D3DXMATRIX lightViewMatrix;
	D3DXVECTOR3 lightPos = pTextureMRTManager->CREATE_TABLE[CTextureMRTManager::TYPE_SHADOW_MAP].posV;	// 視点
	D3DXVECTOR3 target = pTextureMRTManager->CREATE_TABLE[CTextureMRTManager::TYPE_SHADOW_MAP].posR;	// 注視点

	D3DXMatrixLookAtLH(
		&lightViewMatrix,
		&lightPos, 
		&target, 
		&Const::VEC_U);

	D3DXMATRIX lightMtxProj;

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&lightMtxProj,
		D3DXToRadian(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		1.0f,
		lightPos.y + lightPos.y * 0.5f);


	D3DXMATRIX lightVP;
	D3DXMatrixMultiply(&lightVP, &lightViewMatrix, &lightMtxProj); // proj * view

	CShader::SetMatrix("g_mtxLVP", lightVP);

	D3DXVECTOR3 lightDir = math::GetVector(target , lightPos);
	D3DXVECTOR4 presentLightDir = { lightDir.x,lightDir.y,lightDir.z,0.0f };
	CShader::SetVector("g_lightDir", presentLightDir);

	CShader::CommitChange();
}