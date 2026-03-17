//===================================================
//
// シェーダーの初期化を行うs [shader_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader_manager.h"
#include "manager.h"
#include "renderer.h"
#include "color_constants.hpp"
#include "shader_outline.h"
#include "bloom_shader.h"
#include "luminance_shader.h"
#include "gaus_blur_shader.h"
#include "shadow_map_shader.h"
#include "shadow_map_recieve_shader.h"
#include "shadow_map_reciever_mt_shader.h"
 
//***************************************************
// 定数宣言
//***************************************************
namespace ShaderConst
{
    constexpr const char* OUTLINE_PATH                  = "data/SHADER/OutLine.hlsl";                   // アウトラインシェーダーのパス
    constexpr const char* GAUS_BLUR_PATH                = "data/SHADER/gausblur.hlsl";                  // ガウスブラーのパス
    constexpr const char* LUMINANCE_PATH                = "data/SHADER/luminance.hlsl";                 // 輝度抽出のパス
    constexpr const char* BLOOM_PATH                    = "data/SHADER/bloom.hlsl";                     // ブルームのパス
    constexpr const char* SHADOW_MAP_PATH               = "data/SHADER/shadow_map.hlsl";                // シャドウマップのパス
    constexpr const char* SHADOW_MAP_RECIEVE_PATH       = "data/SHADER/shadow_map_reciever.hlsl";       // シャドウマップを受けるオブジェクトのパス
    constexpr const char* SHADOW_MAP_RECIEVE_MT_PATH    = "data/SHADER/shadow_map_reciever_mt.hlsl";    // シャドウマップを受けるマルチテクスチャオブジェクトのパス
    constexpr const char* TECHNIQUE                     = "tech";                                       // テクニック宣言
}

//===================================================
// コンストラクタ
//===================================================
CShaderManager::CShaderManager()
{
}

//===================================================
// デストラクタ
//===================================================
CShaderManager::~CShaderManager()
{
    // シェーダー分回す
    for (auto& shader : m_apShader)
    {
        // メモリの破棄
        shader.reset();
    }
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CShaderManager> CShaderManager::Create(void)
{
    // インスタンスの生成
    auto pInstance = std::make_unique<CShaderManager>();

    // 初期化処理
    if (FAILED(pInstance->Init()))
    {
        pInstance.reset();
        return nullptr;
    }

    return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShaderManager::Init(void)
{
    // アウトラインの生成
    m_apShader[TYPE_OUTLINE] = std::make_unique<CShaderOutLine>();

    // アウトラインの初期化
    if (FAILED(m_apShader[TYPE_OUTLINE]->Init(ShaderConst::OUTLINE_PATH, ShaderConst::TECHNIQUE))) return E_FAIL;

    // ガウスブラーの生成
    m_apShader[TYPE_GAUS_BLUR] = std::make_unique<CGausBlurShader>();

    // ブラーの初期化
    if (FAILED(m_apShader[TYPE_GAUS_BLUR]->Init(ShaderConst::GAUS_BLUR_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // 輝度抽出ブラーの生成
    m_apShader[TYPE_LUMINANCE] = std::make_unique<CLuminanceShader>();

    // 輝度抽出ブラーの生成
    if (FAILED(m_apShader[TYPE_LUMINANCE]->Init(ShaderConst::LUMINANCE_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // ブルームの生成
    m_apShader[TYPE_BLOOM] = std::make_unique<CBloomShader>();

    // 輝度抽出ブラーの生成
    if (FAILED(m_apShader[TYPE_BLOOM]->Init(ShaderConst::BLOOM_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // シャドウマップの生成
    m_apShader[TYPE_SHADOW_MAP] = std::make_unique<CShadowMapShader>();

    // 輝度抽出ブラーの生成
    if (FAILED(m_apShader[TYPE_SHADOW_MAP]->Init(ShaderConst::SHADOW_MAP_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // 影を受けるシェーダーの生成
    m_apShader[TYPE_SHADOW_MAP_RECIEVE] = std::make_unique<CShadowMapRecieverShader>();

    // 輝度抽出ブラーの生成
    if (FAILED(m_apShader[TYPE_SHADOW_MAP_RECIEVE]->Init(ShaderConst::SHADOW_MAP_RECIEVE_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // 影を受けるシェーダーの生成
    m_apShader[TYPE_SHADOW_MAP_RECIEVE_MT] = std::make_unique<CShadowMapRecieverMTShader>();

    // 輝度抽出ブラーの生成
    if (FAILED(m_apShader[TYPE_SHADOW_MAP_RECIEVE_MT]->Init(ShaderConst::SHADOW_MAP_RECIEVE_MT_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    return S_OK;
}
