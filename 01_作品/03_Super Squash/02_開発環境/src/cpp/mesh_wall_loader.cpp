//===================================================
//
// メッシュの壁の読み込み処理 [mesh_wall_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// シーンのentitiyの生成処理の定義
//***************************************************
#include "mesh_wall_loader.h"
#include "fstream"
#include "json_manager.h"
#include "wall_constants.hpp"
#include "factory_mesh.h"
#include "entity_name_component.hpp"
#include "title_wall_component.hpp"
#include "mesh_vtx_component.hpp"
#include "layer_component.hpp"
#include "mesh_wall_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace MeshWallLoaderConst
{
    constexpr float OFFPOS_TEX_U            = 0.4f;                                     // テクスチャ座標U

    constexpr const char* FIRST_SCENE       = "WALL/first_scene_wall.json";             // 読み込み用ファイルパス
    constexpr const char* GAME_SCENE        = "WALL/game_wall.json";                    // 読み込み用ファイルパス
    constexpr const char* TEXTURE_BG_PATH   = "data/TEXTURE/wall_backgrond_000.jpg";    // 背景のテクスチャパス
}

//===================================================
// 最初のシーンの壁の読み込み処理
//===================================================
HRESULT CMeshWallLoader::FirstSceneLoad(entt::registry& registry)
{
	// 名前空間の使用
	using namespace nlohmann;
    using namespace MeshWallLoaderConst;

    // 読み取り用変数
    json config;

    // ファイルを読み込む
    if (FAILED(JsonManager::LoadJson(FIRST_SCENE, config)))
    {
        return E_FAIL;
    }

    // キーが存在しないなら
    if (!config.contains("walls"))
    {
        return E_FAIL;
    }

    int nCnt = 0;

    // 壁の数分回す
    for (auto& walls : config["walls"])
    {
        // 位置を取得
        D3DXVECTOR3 pos = JsonManager::LoadVector3(walls["pos"]);

        // 向きの取得
        D3DXVECTOR3 rot = JsonManager::LoadVector3(walls["rot"]);

        // 大きさ
        D3DXVECTOR3 size = JsonManager::LoadVector3(walls["size"]);

        // 目的の位置、向きの取得
        D3DXVECTOR3 destPos = JsonManager::LoadVector3(walls["dest_pos"]);
        D3DXVECTOR3 destRot = JsonManager::LoadVector3(walls["dest_rot"]);

        // y軸回転だけ設定
        destRot.y = rot.y;

        int nLayer = 0;

        // テクスチャパス
        std::string texturePath = walls["texture"];

        // 名前の取得
        std::string name = walls["entity_name"];

        // 分割数取得
        int nSegmentU = walls["segmentU"];
        int nSegmentV = walls["segmentV"];

        // メッシュウォールの生成処理
        entt::entity entity = FactoryMesh::Create::WallMT(registry, pos, size, texturePath.c_str(), nullptr, rot);

        // 壁の構築処理
        FactoryMesh::Build::WallMT(registry, entity, nSegmentU, nSegmentV, D3DXVECTOR2(OFFPOS_TEX_U * nCnt, 0.0f), D3DXVECTOR2(OFFPOS_TEX_U * (nCnt + 1), 1.0f));
        registry.emplace<EntityNameComponent>(entity, name);
        registry.emplace<TitleWallComponent>(entity, destPos, destRot);
        registry.emplace<LayerComponent>(entity, nLayer);


        nCnt++;
    }

	return S_OK;
}

//===================================================
// ゲームシーンで使う壁の読み込み処理
//===================================================
HRESULT CMeshWallLoader::GameSceneLoad(entt::registry& registry)
{
    // 名前空間の使用
    using namespace nlohmann;
    using namespace MeshWallLoaderConst;

    // 読み取り用変数
    json config;

    // ファイルを読み込む
    if (FAILED(JsonManager::LoadJson(GAME_SCENE, config)))
    {
        return E_FAIL;
    }

    // キーが存在しないなら
    if (!config.contains("walls"))
    {
        return E_FAIL;
    }

    int nCnt = 0;

    // 壁の数分回す
    for (auto& walls : config["walls"])
    {
        // 位置を取得
        D3DXVECTOR3 pos = JsonManager::LoadVector3(walls["pos"]);
                                                  
        // 向きの取得                             
        D3DXVECTOR3 rot = JsonManager::LoadVector3(walls["rot"]);
                                                  
        // 大きさ                                 
        D3DXVECTOR3 size = JsonManager::LoadVector3(walls["size"]);

        // テクスチャパス
        std::string texturePath = walls["texture"];

        // 名前の取得
        std::string name = walls["entity_name"];

        // 分割数取得
        int nSegmentU = walls["segmentU"];
        int nSegmentV = walls["segmentV"];

        // 天井だったら
        const bool bTopWall = name == WallConst::TOP_WALL;

        int nLayer = 0;
        bool bMult = false;

        // メッシュウォールの生成処理
        entt::entity entity = entt::null;

        if (bTopWall)
        {
            // 天井の生成処理
            entity = FactoryMesh::Create::WallMT(registry, pos, size, texturePath.c_str(), nullptr, rot);

            nLayer = 1;
        }
        else
        {
            bMult = true;
            // 壁の生成処理
            entity = FactoryMesh::Create::WallMT(registry, pos, size, texturePath.c_str(), TEXTURE_BG_PATH, rot);
        }

        // 壁の構築処理
        FactoryMesh::Build::WallMT(registry, entity, nSegmentU, nSegmentV, D3DXVECTOR2(OFFPOS_TEX_U * nCnt, 0.0f), D3DXVECTOR2(OFFPOS_TEX_U * (nCnt + 1), 1.0f));
        registry.emplace<EntityNameComponent>(entity, name);
        registry.emplace<LayerComponent>(entity, nLayer);

        nCnt++;
    }

    return S_OK;
}
