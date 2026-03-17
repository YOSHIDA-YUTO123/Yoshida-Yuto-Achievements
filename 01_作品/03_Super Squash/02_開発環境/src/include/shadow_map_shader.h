//===================================================
//
// シャドウマップを適応するシェーダー [shadow_map_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_SHADER_H_
#define _SHADOW_MAP_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// シャドウマップのシェーダーのクラスの定義
//***************************************************
class CShadowMapShader : public CShader
{
public:
	CShadowMapShader();
	~CShadowMapShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif