//===================================================
//
// キャラクターのファクトリーの処理 [factory_character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_character.h"
#include "layer_component.hpp"
#include "transform_component.hpp"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "model_manager.h"
#include "manager.h"
#include "texture_id_component.hpp"
#include "velocity_component.hpp"
#include "motion_component.hpp"
#include "tag_component.hpp"
#include "player_component.hpp"
#include "character_component.hpp"
#include "field_collision_component.hpp"
#include "factory_model.h"
#include "sphere_collider_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "player_command_component.hpp"
#include "minigame_player_component.hpp"
#include "outline_shader_component.hpp"
#include "color_constants.hpp"
#include "factory_system_entity.h"
#include "locator_component.hpp"
#include "motion_loader.h"
#include "collision_effect_wall_component.hpp"
#include "capsule_collider_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace FactoryCharacterConst
{
    constexpr const char* JET_PACK_PATH         = "JetPack.x";            // ジェットパックのモデルのパス
    constexpr float MOVE_SPEED                  = 2.5f;                   // 移動速度
    constexpr float PLAYER_CAPSULE_RADIUS       = 20.0f;                  // プレイヤーのカプセルコライダーの半径
    constexpr float PLAYER_INERTIA              = 0.25f;                  // プレイヤーの移動慣性
    constexpr float PLAYER_GRAVITY              = 0.1f;                   // プレイヤーの重力量
    constexpr float PLAYER_SPHERE_RADIUS        = 40.0f;                  // プレイヤーの球のコライダーの半径
    constexpr float BALL_SHOT_RANGE             = 70.0f;                  // ボールを打てる範囲
    constexpr float RACKET_CAPSULE_RADIUS       = 40.0f;                  // ボールのカプセルの半径
    constexpr float OUTLINE_SIZE                = 0.3f;                   // アウトラインの太さ
    const D3DXVECTOR3 BALL_SHOT_RANGE_OFFSET    = { 0.0f,70.0f,-10.0f };  // ボールを打てる範囲のオフセット
    const D3DXVECTOR3 STENCIL_SHADOW_SCALE      = { 2.0f,5.0f,2.0f };     // ステンシルの影の大きさ
    const D3DXVECTOR3 PLAYER_CAPSULE_END_POS    = { 0.0f,70.0f,0.0f };    // プレイヤーのカプセルコライダーの終点の位置
    const D3DXVECTOR3 PLAYER_CAPSULE_START_POS  = { 0.0f,0.0f,0.0f };     // プレイヤーのカプセルコライダーの始点の位置
    const D3DXVECTOR3 JET_PACK_POS              = { 0.0f,10.0f,10.0f };   // ジェットパックの位置
    const D3DXVECTOR3 PLAYER_SPHERE_OFFSET      = { 0.0f,50.0f,0.0f };    // プレイヤーの球のコライダーのオフセット
}

//===================================================
// キャラクターの生成処理
//===================================================
entt::entity FactoryCharacter::CreateCharacter(entt::registry& registry, const char* pMotionFile, const int nNumMotion, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    // 名前空間の使用
    using namespace FactoryCharacterConst;

    // entityの生成処理
    const entt::entity character = registry.create();

    auto& childrenComp = registry.emplace<ChildrenComponent>(character);

    registry.emplace<Transform3DComponent>(character, pos, rot);
    registry.emplace<LayerComponent>(character);
    registry.emplace<MotionComponent>(character, nNumMotion);
    registry.emplace<Tag::CharacterTag>(character);
    registry.emplace<Tag::HitStopTag>(character);

    auto& charactercomp = registry.emplace<CharacterComponent>(character, MOVE_SPEED);

    charactercomp.fRotDest = rot.y;
    charactercomp.shadowID = FactoryModel::CreateStencilShadow(registry, pos, STENCIL_SHADOW_SCALE);

    // モデルのロード処理
    if (FAILED(Loader::Motion::Load(character, registry, pMotionFile)))
    {
        // ロードに失敗したなら破棄する
        registry.destroy(character);
        return entt::null;
    }

    // 子供分回す
    for (auto& children : childrenComp.aChildrenID)
    {
        registry.emplace<RendererTag::CharacterModelTag>(children);
    }

    return character;
}

//===================================================
// プレイヤーの構成処理
//===================================================
void FactoryCharacter::BuildPlayer(entt::registry& registry, entt::entity player)
{
    // 名前空間の使用
    using namespace FactoryCharacterConst;

    registry.emplace<PlayerComponent>(player);
    registry.emplace<VelocityComponent>(player, PLAYER_INERTIA, PLAYER_GRAVITY);
    registry.emplace<Tag::PlayerTag>(player);
    registry.emplace<FieldCollisionComponent>(player, player);
    registry.emplace<PlayerCommandComponent>(player);
    registry.emplace<MeshWallCollisionComponent>(player, player);
    registry.emplace<CapsuleColliderComponent>(player, PLAYER_CAPSULE_START_POS, PLAYER_CAPSULE_END_POS, PLAYER_CAPSULE_RADIUS, player);
    registry.emplace<CollisionEffectWallComponent>(player);

    // 球のコライダーの生成
    auto sphereID = FactorySystemEntity::CreateSphereCollider(registry, player, BALL_SHOT_RANGE, BALL_SHOT_RANGE_OFFSET);
    registry.emplace<ColliderTag::BallShotRange>(sphereID);

    // 地面の判定するコライダーの生成
    sphereID = FactorySystemEntity::CreateSphereCollider(registry, player, PLAYER_SPHERE_RADIUS, FactoryCharacterConst::PLAYER_SPHERE_OFFSET);
    registry.emplace<ColliderTag::Wall>(sphereID);
    registry.emplace<ColliderTag::Field>(sphereID);

    auto& childrenComp = registry.get<ChildrenComponent>(player);

    // 胸のIDの取得
    auto bodyID = childrenComp.aChildrenID[CharacterModelComponent::MODEL_CHEST];
    FactoryModel::CreateJetPack(registry, JET_PACK_POS, Const::VEC3_NULL, JET_PACK_PATH, player, bodyID);

    // ラケットモデルのIDの取得
    auto racketID = childrenComp.aChildrenID[CharacterModelComponent::MODEL_RACKET];
    
    // ラケットの設定
    {
        registry.emplace<Tag::BallCollisionTag>(racketID);
        auto& characterModelComp = registry.get<CharacterModelComponent>(racketID);

        // モデルマネージャーの取得
        CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

        if (pModelManager == nullptr) return;

        // モデルの情報の取得
        CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(characterModelComp.nModelID);

        // ロケーターの配置
        auto Locator = FactorySystemEntity::CreateLocator(registry, { 0.0f,modelInfo.size.y ,0.0f }, racketID);
        registry.emplace<LocatorComponent>(racketID, Locator);

        // コライダーの生成
        registry.emplace<CapsuleColliderComponent>(racketID, Const::VEC3_NULL, D3DXVECTOR3(0.0f, modelInfo.size.y, 0.0f), RACKET_CAPSULE_RADIUS, racketID);
    }

    // モデルの数分回す
    for (const auto& modelID : childrenComp.aChildrenID)
    {
        // アウトラインシェーダーを適応
        registry.emplace<OutLineShaderComponent>(modelID, Color::AQUA, OUTLINE_SIZE);
        registry.emplace<RendererTag::OutLineCharacterTag>(modelID);
    }

    // ミニゲームのプレイヤーの構築処理
    BuildMinigamePlayer(registry, player);
}

//===================================================
// ミニゲームのプレイヤーの構築処理
//===================================================
void FactoryCharacter::BuildMinigamePlayer(entt::registry& registry, entt::entity player)
{
    // ミニゲームのプレイヤーのデータ
    registry.emplace<MinigamePlayerComponent>(player);
}