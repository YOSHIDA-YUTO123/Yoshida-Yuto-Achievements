//===================================================
//
// ガウスブラーシェーダーを適応するクラス [gaus_blur_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "gaus_blur_shader.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CGausBlurShader::CGausBlurShader() : 
	m_aWeight()
{
}

//===================================================
// デストラクタ
//===================================================
CGausBlurShader::~CGausBlurShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CGausBlurShader::Init(const char* pShaderFile, const char* pTech)
{
	// 初期化処理
	if (FAILED(CShader::Init(pShaderFile, pTech)))
	{
		return E_FAIL;
	}

	// ブラーの重みを設定する関数
	//CalcGaussianWeights(10.0f);

	return S_OK;
}

//===================================================
// 適応処理
//===================================================
void CGausBlurShader::Apply(const entt::registry* pRegistry, const entt::entity entity)
{
	// 使用しない
	(void)pRegistry;
	(void)entity;

	// ブラーの強さを設定
	CShader::SetFloat("BLUR_STRENGTH", 2.0f);

	// 画面の大きさを設定
	CShader::SetFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	CShader::SetFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);

	//CManager::GetInstance()->GetRenderer()->GetDevice()->SetPixelShaderConstantF(0, m_aWeight.data(), 3);
}

//===================================================
// ブラーの重みを計算する関数
//===================================================
void CGausBlurShader::CalcGaussianWeights(const float fSigma)
{
	// 重みの合計を記録する変数を定義する
	float total = 0;

	// ここからガウス関数を用いて重みを計算している
	// ループ変数のxが基準テクセルからの距離
	for (int x = 0; x < MAX_WEIGHT; x++)
	{
		m_aWeight[x] = expf(-0.5f * (float)(x * x) / fSigma);
		total += 2.0f * m_aWeight[x];
	}

	// 重みの合計で除算することで、重みの合計を1にしている
	for (int i = 0; i < MAX_WEIGHT; i++)
	{
		m_aWeight[i] /= total;
	}
}