//===================================================
//
// メッシュの地面の読み込み処理 [mesh_sky_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_sky_loader.h"
#include "json_manager.h"
#include "factory_mesh.h"
#include "rotation_move_component.hpp"
#include "loader_constants.hpp"

//===================================================
// ロード処理
//===================================================
HRESULT CMeshSkyLoader::Load(entt::registry& registry)
{
    // 名前空間の使用
    using namespace nlohmann;
    using namespace LoaderConst;

    json config;

    // 読み込み処理
    if (FAILED(JsonManager::LoadJson(SKY_PATH, config)))
    {
        return E_FAIL;
    }

    // キーが存在するなら
    if (config.contains("sky"))
    {
        json skyJson = config["sky"];

        // 位置,大きさの取得
        D3DXVECTOR3 pos = JsonManager::LoadVector3(skyJson["pos"]);
        D3DXVECTOR3 rotationSpeed = JsonManager::LoadVector3(skyJson["rotation_speed"]);

        std::string texturePath = skyJson["texture"];
        float fRadius = skyJson["radius"];
        int nSegment = skyJson["segment"];
        bool bReverse = skyJson["reverse"];

        // 球体の生成
        auto skyID = FactoryMesh::Create::Sphere(registry, pos, fRadius, fRadius, nSegment, texturePath.c_str(), bReverse);
        registry.emplace<RotationMoveComponent>(skyID, rotationSpeed);
    }

    return S_OK;
}
