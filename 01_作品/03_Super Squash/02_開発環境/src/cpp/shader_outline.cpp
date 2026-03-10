//===================================================
//
// アウトラインシェーダーを適応するクラス [shader_outline.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader_outline.h"
#include "manager.h"
#include "renderer.h"
#include "transform_component.hpp"
#include "color_constants.hpp"
#include "outline_shader_component.hpp"

//===================================================
// コンストラクタ
//===================================================
CShaderOutLine::CShaderOutLine()
{
}

//===================================================
// デストラクタ
//===================================================
CShaderOutLine::~CShaderOutLine()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShaderOutLine::Init(const char* pShaderFile, const char* pTech)
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
void CShaderOutLine::Apply(const entt::registry& registry, const entt::entity entity)
{
    // マネージャーの取得
    CManager* pManager = CManager::GetInstance();

    // レンダラーの取得
    CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
    assert(pRenderer != nullptr && "ShaderManager");
#endif // _DEBUG

    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    D3DXMATRIX mtxProj, mtxView;

    // ビューマトリックス、プロジェクションマトリックスの取得
    pDevice->GetTransform(D3DTS_VIEW, &mtxView);
    pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

    // コンポーネントの取得
    auto& transformComp = registry.get<Transform3DComponent>(entity);
    auto& shaderComp = registry.get<OutLineShaderComponent>(entity);

    // ワールドマトリックス
    D3DXMATRIX worldViewProjMatrix;
    worldViewProjMatrix = transformComp.mtxWorld * mtxView * mtxProj;

    // マトリックスの設定
    CShader::SetMatrix("g_WorldViewProj", worldViewProjMatrix);

    // 色の設定
    D3DXCOLOR col = shaderComp.col;
    D3DXVECTOR4 color = { col.r,col.g, col.b,col.a };

    // エッジの太さを設定
    CShader::SetFloat("g_OutLineWidth", shaderComp.fSize);

    // エッジの色を設定
    CShader::SetVector("g_OutLineColor", color);
}