//===================================================
//
//  プレイヤーの読み込み処理 [player_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player_loader.h"
#include "json_manager.h"
#include "factory_character.h"
#include "motion_component.hpp"
#include "loader_constants.hpp"

//===================================================
// 読み込み処理
//===================================================
HRESULT CPlayerLoader::Load(entt::registry& registry)
{
    // 名前空間の使用
    using namespace nlohmann;
    using namespace LoaderConst;

    json config;

    // 読み込み処理
    if (FAILED(JsonManager::LoadJson(PLAYER_PATH, config)))
    {
        return E_FAIL;
    }

    // キーが存在するなら
    if (config.contains("player"))
    {
        // プレイヤーの情報の取得
        json playerJson = config["player"];

        // モーションのファイルパスの取得
        std::string motionFilePath = playerJson["motion"];

        // 空間情報の読み込み
        D3DXVECTOR3 pos = JsonManager::LoadVector3(playerJson["pos"]);
        D3DXVECTOR3 rot = JsonManager::LoadVector3(playerJson["rot"]);

        // プレイヤーの生成
        entt::entity playerID = FactoryCharacter::CreateCharacter(registry, motionFilePath.c_str(), MotionComponent::MOTIONTYPE_MAX, pos, rot);
        FactoryCharacter::BuildPlayer(registry, playerID);
    }

    return S_OK;
}
