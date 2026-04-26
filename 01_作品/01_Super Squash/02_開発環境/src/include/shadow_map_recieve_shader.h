//===================================================
//
// シャドウマップを受けるモデルシェーダー [shadow_map_recieve_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_RECIEVE_SHADER_H_
#define _SHADOW_MAP_RECIEVE_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"

//***************************************************
// シャドウマップを受けるモデルのシェーダーのクラスの定義
//***************************************************
class CShadowMapRecieverShader : public CShader
{
public:
	CShadowMapRecieverShader();
	virtual ~CShadowMapRecieverShader();

	HRESULT Init(const char* pShaderFile, const char* pTech) override;
	virtual void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif