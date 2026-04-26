//===================================================
//
// シーンのentityの生成処理 [scene_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "scene_loader.h"
#include "motion_component.hpp"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "fstream"
#include "string"
#include "sstream"
#include "model_manager.h"
#include "manager.h"
#include "transform_component.hpp"
#include "light.h"
#include "factory_character.h"
#include "player_component.hpp"
#include "json_manager.h"
#include "factory_mesh.h"
#include "renderer_component.hpp"
#include "camera.h"
#include "mesh_wall_component.hpp"
#include "color_constants.hpp"
#include "factory_2d.h"
#include "entity_name_component.hpp"
#include "wall_constants.hpp"
#include "title_wall_component.hpp"
#include "ui_wall_component.hpp"
#include "mesh_wall_loader.h"
#include "mesh_field_loader.h"
#include "mesh_sky_loader.h"
#include "player_loader.h"
#include "loader_constants.hpp"

//===================================================
// 基本のシーンの生成
//===================================================
HRESULT Loader::Scene::FirstScene(entt::registry& registry)
{
    // 名前空間の使用
    using namespace LoaderConst;

    // メッシュの壁の読み込み
    if (FAILED(CMeshWallLoader::FirstSceneLoad(registry)))
    {
        return E_FAIL;
    }

    // メッシュフィールドの読み込み
    if (FAILED(CMeshFieldLoader::FirstSceneLoad(registry, FIRST_SCENE_FIELD_PATH)))
    {
        return E_FAIL;
    }

    // 空の読み込み処理
    if (FAILED(CMeshSkyLoader::Load(registry)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//===================================================
// ゲームのロード
//===================================================
HRESULT Loader::Scene::Minigame(entt::registry& registry)
{
    // 読み込み処理
    if (FAILED(CPlayerLoader::Load(registry)))
    {
        return E_FAIL;
    }

    return S_OK;
}

//===================================================
// 基盤のシーンのロード処理
//===================================================
HRESULT Loader::Scene::BaseScene(entt::registry& registry)
{
    // 名前空間の使用
    using namespace LoaderConst;

    // メッシュの壁の読み込み処理
    if (FAILED(CMeshWallLoader::GameSceneLoad(registry)))
    {
        return E_FAIL;
    }

    // メッシュフィールドの読み込み
    if (FAILED(CMeshFieldLoader::FirstSceneLoad(registry, SCENE_BASE_FIELD_PATH)))
    {
        return E_FAIL;
    }

    // 空の読み込み処理
    if (FAILED(CMeshSkyLoader::Load(registry)))
    {
        return E_FAIL;
    }

    return S_OK;
}