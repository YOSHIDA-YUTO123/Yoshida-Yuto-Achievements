//===================================================
//
// ガウスブラーシェーダーを適応するクラス [gaus_blur_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _GAUS_BLUR_SHADER_H_
#define _GAUS_BLUR_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// ガウスブラーのシェーダーを適応するクラスの定義
//***************************************************
class CGausBlurShader : public CShader
{
public:
	static constexpr int MAX_WEIGHT = 9;	// ブラーの重み

	CGausBlurShader();
	~CGausBlurShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
	void CalcGaussianWeights(const float fSigma);
private:
	std::array<float, MAX_WEIGHT> m_aWeight;	// ブラーの重み
};
#endif