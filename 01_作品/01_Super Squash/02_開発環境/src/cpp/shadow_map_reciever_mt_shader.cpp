//===================================================
//
// シャドウマップを受けるシェーダー(マルチテクスチャ) [shadow_map_recieve_mt_shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_map_reciever_mt_shader.h"
#include "ui_wall_component.hpp"
#include "texture_mrt_manager.h"
#include "manager.h"

//===================================================
// コンストラクタ
//===================================================
CShadowMapRecieverMTShader::CShadowMapRecieverMTShader()
{
}

//===================================================
// デストラクタ
//===================================================
CShadowMapRecieverMTShader::~CShadowMapRecieverMTShader()
{
}

//===================================================
// 適応処理
//===================================================
void CShadowMapRecieverMTShader::Apply(const entt::registry* pRegistry, const entt::entity entity)
{
	// nullなら処理しない
	if (pRegistry == nullptr)
	{
		return;
	}

	// UIの壁のコンポーネントの取得
	auto& uiWallComp = pRegistry->get<UIWallComponent>(entity);

	const bool bHasTexture = uiWallComp.pTexture != nullptr;
	const bool bHasTextureMT = uiWallComp.pTextureMT != nullptr;

	// 値の設定
	CShader::SetBool("g_bHasBaseTexture", bHasTexture);
	CShader::SetBool("g_bHasBaseTextureMT", bHasTextureMT);

	// 適応処理
	CShadowMapRecieverShader::Apply(pRegistry, entity);
}