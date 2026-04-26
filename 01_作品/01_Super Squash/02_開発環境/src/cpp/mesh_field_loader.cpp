//===================================================
//
// メッシュの地面の読み込み処理 [mesh_field_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_field_loader.h"
#include "json_manager.h"
#include "factory_mesh.h"

//===================================================
// 最初のシーンのロード処理
//===================================================
HRESULT CMeshFieldLoader::FirstSceneLoad(entt::registry& registry, const char* pJsonFilePath)
{
    // 名前空間の使用
    using namespace nlohmann;

    json config;

    // ファイルを読み取る
    if (FAILED(JsonManager::LoadJson(pJsonFilePath, config)))
    {
        return E_FAIL;
    }

    // キーが存在するなら
    if (config.contains("field"))
    {
        // fieldの取得
        json fieldJson = config["field"];

        // 位置,大きさの取得
        D3DXVECTOR3 pos = JsonManager::LoadVector3(fieldJson["pos"]);
        D3DXVECTOR3 size = JsonManager::LoadVector3(fieldJson["size"]);
        std::string texturePath = fieldJson["texture"];

        // 分割数の取得
        int nSegmentU = fieldJson["segmentU"];
        int nSegmentV = fieldJson["segmentV"];

        // メッシュフィールドの生成
        entt::entity entity = FactoryMesh::Create::Field(registry, pos, size, texturePath.c_str());
        FactoryMesh::Build::Field(registry, entity, nSegmentU, nSegmentV);
    }

    return S_OK;
}
