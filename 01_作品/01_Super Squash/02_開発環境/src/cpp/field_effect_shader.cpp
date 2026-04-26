//===================================================
//
// フィールドのエフェクトのシェーダーを適応する [field_effect_shader.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "field_effect_shader.h"
#include "field_effect_component.hpp"
#include "texture_id_component.hpp"

//===================================================
// コンストラクタ
//===================================================
CFieldEffectShader::CFieldEffectShader()
{
}

//===================================================
// デストラクタ
//===================================================
CFieldEffectShader::~CFieldEffectShader()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CFieldEffectShader::Init(const char* pShaderFile, const char* pTech)
{
	// 初期化処理
	if (FAILED(CShader::Init(pShaderFile, pTech)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 適応処理
//===================================================
void CFieldEffectShader::Apply(const entt::registry& registry, const entt::entity entity)
{
	// 地面のエフェクトの取得
	auto& fieldEffectComp = registry.get<FieldEffectComponent>(entity);
	auto pTextureIDComp = registry.try_get<TextureIDComponent>(entity);

	// 値の設定
	CShader::SetValue("g_UVPos", &fieldEffectComp.uvPos, sizeof(D3DXVECTOR2));

	// 無効なテクスチャのID
	int nTextureID = TextureIDComponent::INVALID_ID;

	if (pTextureIDComp != nullptr)
	{
		nTextureID = pTextureIDComp->nTextureID;
	}

	// テクスチャの設定
	CShader::SetTexture("g_BaseTexture", nTextureID);

	// タイムカウンターの設定
	CShader::SetFloat("g_Time", fieldEffectComp.fTime);
}