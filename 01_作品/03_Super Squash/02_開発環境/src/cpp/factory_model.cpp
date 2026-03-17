//===================================================
//
// モデルのエンティティのファクトリーの処理 [factory_model.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_model.h"
#include "model_component.hpp"
#include "transform_component.hpp"
#include "entity_layer.hpp"
#include "layer_component.hpp"
#include "model_manager.h"
#include "manager.h"
#include "tag_component.hpp"
#include "sphere_collider_component.hpp"
#include "jet_pack_component.hpp"
#include "parent_component.hpp"
#include "factory_system_entity.h"
#include "outline_shader_component.hpp"
#include "color_constants.hpp"
#include "factory_system_entity.h"
#include "mrt_target_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace FactoryModelConst
{
    constexpr const char* SHADOW_MODEL          = "ShadowS.x";                // 影のモデルのパス
    const D3DXVECTOR3 JETPACK_EFFECT_POS_LEFT   = { -20.0f,-10.0f,0.0f };     // ジェットパックのエフェクトの位置左
    const D3DXVECTOR3 JETPACK_EFFECT_POS_RIGHT  = { 20.0f,-10.0f,0.0f };      // ジェットパックのエフェクトの位置右
    constexpr float JETPACK_OUTLINE_SIZE        = 0.5f;                       // ジェットパックのアウトラインの色
}

//===================================================
// モデルの登録
//===================================================
const int FactoryModel::Register(const char* pModelFilePath)
{
    // モデルのマネージャークラスの取得
    CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

    int nModelID = CModelManager::INVALID_ID;

    if (pModelManager != nullptr)
    {
        // モデルのIDの登録
        nModelID = pModelManager->Register(pModelFilePath);
    }

    return nModelID;
}

//===================================================
// モデルの生成
//===================================================
entt::entity FactoryModel::CreateModel(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath)
{
    // entityの生成
    const entt::entity entity = registry.create();

    // モデルの登録
    const int nModelID = Register(pModelFilePath);

    registry.emplace<Tag::ModelTag>(entity);
    registry.emplace<Transform3DComponent>(entity, pos, rot);
    registry.emplace<ModelComponent>(entity, nModelID);
    registry.emplace<LayerComponent>(entity,static_cast<int>(EntityLayer::Model));
   
    return entity;
}

//===================================================
// アウトラインを適応するモデルの生成
//===================================================
entt::entity FactoryModel::CreateOutLineModel(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath)
{
    // entityの生成
    const entt::entity entity = registry.create();

    // モデルの登録
    const int nModelID = Register(pModelFilePath);

    registry.emplace<Tag::ModelTag>(entity);
    registry.emplace<RendererTag::OutLineModelTag>(entity);
    registry.emplace<Transform3DComponent>(entity, pos, rot);
    registry.emplace<ModelComponent>(entity, nModelID);
    registry.emplace<LayerComponent>(entity, static_cast<int>(EntityLayer::Model));
    registry.emplace<OutLineShaderComponent>(entity, Color::RED, 0.3f);

    return entity;
}

//===================================================
// マルチレンダーターゲットのモデルの生成処理
//===================================================
entt::entity FactoryModel::CreateModelMRT(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath, const int nMRTType)
{
    // entityの生成
    const entt::entity entity = registry.create();

    // モデルの登録
    const int nModelID = Register(pModelFilePath);

    registry.emplace<MRTTargetComponent>(entity,nMRTType);
    registry.emplace<Transform3DComponent>(entity, pos, rot);
    registry.emplace<ModelComponent>(entity, nModelID);
    registry.emplace<LayerComponent>(entity, static_cast<int>(EntityLayer::Model));
    registry.emplace<RendererTag::ModelMRT>(entity, nMRTType);

    return entity;
}

//===================================================
// 影の生成
//===================================================
entt::entity FactoryModel::CreateStencilShadow(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Scal)
{
    // entityの生成
    const entt::entity entity = registry.create();

    // モデルの登録
    const int nModelID = Register(FactoryModelConst::SHADOW_MODEL);

    registry.emplace<Tag::StencilTag>(entity);
    registry.emplace<Transform3DComponent>(entity, pos, Const::VEC3_NULL, Scal);
    registry.emplace<ModelComponent>(entity, nModelID);
    registry.emplace<LayerComponent>(entity, static_cast<int>(EntityLayer::Shadow));
    

    return entity;
}

//===================================================
// ジェットパックの生成
//===================================================
entt::entity FactoryModel::CreateJetPack(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath, const entt::entity ownerID, const entt::entity parentID)
{
    // 名前空間の使用
    using namespace FactoryModelConst;

    // entityの生成
    const entt::entity entity = registry.create();

    // モデルの登録
    const int nModelID = Register(pModelFilePath);

    // 親子関係の
    registry.emplace<ParentComponent>(entity, parentID);

    // ジェットパックの取得
    auto& jetPackComp = registry.emplace<JetPackComponent>(entity, ownerID);
    jetPackComp.effectLocator[0] = FactorySystemEntity::CreateLocator(registry, FactoryModelConst::JETPACK_EFFECT_POS_LEFT, entity);
    jetPackComp.effectLocator[1] = FactorySystemEntity::CreateLocator(registry, FactoryModelConst::JETPACK_EFFECT_POS_RIGHT, entity);

    registry.emplace<Tag::ModelTag>(entity);
    registry.emplace<Transform3DComponent>(entity, pos, rot);
    registry.emplace<ModelComponent>(entity, nModelID);
    registry.emplace<LayerComponent>(entity, static_cast<int>(EntityLayer::Model));    

    return entity;
}
