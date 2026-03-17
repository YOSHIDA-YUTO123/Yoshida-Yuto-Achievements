//===================================================
//
// ブルームシェーダーを適応するクラス [bloom_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BLOOM_SHADER_H_
#define _BLOOM_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// ブルームのシェーダーのクラスの定義
//***************************************************
class CBloomShader : public CShader
{
public:
	CBloomShader();
	~CBloomShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif