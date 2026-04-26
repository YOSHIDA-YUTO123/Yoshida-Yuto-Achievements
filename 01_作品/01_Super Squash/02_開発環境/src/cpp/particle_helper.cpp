//===================================================
//
// パーティクルのヘルパークラス [particle_helper.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 前方宣言
//***************************************************
#include "particle_helper.h"
#include "fstream"
#include "json_manager.h"

//===================================================
// コンストラクタ
//===================================================
CParticleHelper::CParticleHelper() :
    m_param()
{
}

//===================================================
// デストラクタ
//===================================================
CParticleHelper::~CParticleHelper()
{
    // 要素のクリア
    m_param.clear();
}

//===================================================
// 読み込み処理
//===================================================
HRESULT CParticleHelper::Load(const char* pJsonFileName)
{
    // 名前空間の使用
    using namespace nlohmann;

    json config;

    // 読み込み処理
    if (FAILED(JsonManager::LoadJson(pJsonFileName, config)))
    {
        return E_FAIL;
    }
    
    // 読み込んだ分回す
    for (auto& particle : config["config"])
    {
        ParticleComponent::Param param;
        std::string key = particle["key"];
        param.texturePath = particle["texture"];
        param.move = JsonManager::LoadVector3(particle["move"]);
        param.col = JsonManager::LoadRBGA(particle["col"]);

        if (particle.contains("angle_max"))
        {
            param.fAngleMax = particle["angle_max"];
        }
        else if(particle.contains("angle_min"))
        {
            param.fAngleMin = particle["angle_min"];
        }

        param.fRadius = particle["radius"];
        param.nNum = particle["num"];
        param.nLife = particle["life"];
        param.nSpawnTime = particle["spawn_time"];

        // 要素の登録
        m_param.emplace(key, param);
    }

    return S_OK;
}

//===================================================
// パラメータの取得
//===================================================
void CParticleHelper::GetParam(ParticleComponent::Param& param, const D3DXVECTOR3& pos, const std::string& key, const unsigned int nFlag)
{
    // 指定されたキーを探す
    auto itr = m_param.find(key);

    // 取得出来たら
    if (itr != m_param.end())
    {
        param = itr->second;
        param.pos = pos;
        param.nEffectFlag = nFlag;
    }
}