//===================================================
//
// 輝度抽出のシェーダー [luminance_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LUMINANCE_SHADER_H_
#define _LUMINANCE_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// 輝度抽出のシェーダーのクラスの定義
//***************************************************
class CLuminanceShader : public CShader
{
public:
	CLuminanceShader();
	~CLuminanceShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif