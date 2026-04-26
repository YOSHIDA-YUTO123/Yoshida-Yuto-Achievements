//===================================================
//
// アウトラインシェーダーを適応するクラス [shader_outline.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADER_OUTLINE_H_
#define _SHADER_OUTLINE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// アウトラインシェーダーを適応するクラスの定義
//***************************************************
class CShaderOutLine : public CShader
{
public:
	CShaderOutLine();
	~CShaderOutLine();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif