//===================================================
//
// シャドウマップを受けるシェーダー(マルチテクスチャ) [shadow_map_recieve_mt_shader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_MAP_RECIEVE_MT_SHADER_H_
#define _SHADOW_MAP_RECIEVE_MT_SHADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_map_recieve_shader.h"

//***************************************************
// シャドウマップを受けるモデルのシェーダーのクラスの定義
//***************************************************
class CShadowMapRecieverMTShader : public CShadowMapRecieverShader
{
public:
	CShadowMapRecieverMTShader();
	~CShadowMapRecieverMTShader();

	void Apply(const entt::registry* pRegistry, const entt::entity entity) override;
private:
};
#endif