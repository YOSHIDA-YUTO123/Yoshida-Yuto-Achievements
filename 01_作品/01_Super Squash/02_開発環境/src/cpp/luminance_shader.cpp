//===================================================
//
// 輝度抽出のシェーダー [luminance_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "luminance_shader.h"

//===================================================
// コンストラクタ
//===================================================
CLuminanceShader::CLuminanceShader()
{
}

//===================================================
// デストラクタ
//===================================================
CLuminanceShader::~CLuminanceShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CLuminanceShader::Init(const char* pShaderFile, const char* pTech)
{
	// 初期化処理
	if (FAILED(CShader::Init(pShaderFile, pTech)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CLuminanceShader::Apply(const entt::registry* pRegistry, const entt::entity entity)
{
	(void)pRegistry;
	(void)entity;
}