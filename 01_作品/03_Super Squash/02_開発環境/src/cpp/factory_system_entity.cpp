//===================================================
//
// ゲーム等の処理に関わるentityの生成 [factory_system_entity.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_system_entity.h"
#include "score_area_component.hpp"
#include "box_collider_component.hpp"
#include "factory_mesh_quad.h"
#include "renderer_component.hpp"
#include "transform_component.hpp"
#include "size_component.hpp"
#include "utility_math.h"
#include "color_component.hpp"
#include "color_constants.hpp"
#include "parent_component.hpp"
#include "tag_component.hpp"
#include "menu_select_component.hpp"
#include "renderer.h"
#include "texture_mrt_manager.h"
#include "manager.h"
#include "tag_component.hpp"
#include "tutorial_system_component.hpp"
#include "wall_ui_type.hpp"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "score_area_polygon_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace FactorySystem_Const
{
    const D3DXVECTOR2 AREA_SIZE     = { 570.0f,220.0f };    // エリアの全体のサイズ
    constexpr int SEGMENT_U         = 2;                    // エリアの分割数横
    constexpr int SEGMENT_V         = 1;                    // エリアの分割数縦
}

//===================================================
// スコアのエリアの生成処理
//===================================================
entt::entity FactorySystemEntity::CreateScoreArea(entt::registry& registry, const D3DXVECTOR3& centerPos)
{
    // 名前空間の使用
    using namespace FactorySystem_Const;

    // entityの生成
    const entt::entity entity = registry.create();

    // スコアのエリア
    auto& scoreAreaComp = registry.emplace<ScoreAreaComponent>(entity, centerPos);
    scoreAreaComp.size = AREA_SIZE;
    scoreAreaComp.nSegmentU = SEGMENT_U;
    scoreAreaComp.nSegmentV = SEGMENT_V;

#if 0

    // エリアの数分回す
    for (auto& area : scoreAreaComp.data)
    {
        area.polygonID = FactoryMeshQuad::Quad(registry, centerPos, { 0.0f,0.0f,0.0f }, "");
        registry.emplace<RendererComponent>(area.polygonID, RendererComponent::TYPE_NO_DISP);
        registry.emplace<ColorComponent>(area.polygonID, Color::AQUA);
    }

    // コライダーの追加
    registry.emplace<BoxColliderComponent>(
        scoreAreaComp.colliderAABB, Const::VEC3_NULL, Const::VEC3_NULL, scoreAreaComp.data[0].polygonID);
#else
    // エリアの数分回す
    for (auto& area : scoreAreaComp.data)
    {
        area.polygonID = FactoryMeshQuad::Quad(registry, centerPos, Const::VEC3_NULL, "");
        registry.emplace<RendererComponent>(area.polygonID, RendererComponent::TYPE_NONE);
        registry.emplace<ColorComponent>(area.polygonID, Color::AQUA);
        registry.emplace<ScoreAreaPolygonComponent>(area.polygonID);
        registry.emplace<BoxColliderComponent>(area.polygonID, Const::VEC3_NULL, Const::VEC3_NULL, area.polygonID);
    }

    int nCnt = 0;

    // 一マスの大きさを求める
    D3DXVECTOR2 GridSize = Const::VEC2_NULL;
    GridSize.x = scoreAreaComp.size.x / static_cast<float>(scoreAreaComp.nSegmentU);
    GridSize.y = scoreAreaComp.size.y / static_cast<float>(scoreAreaComp.nSegmentV);

    // 全体のマスの数
    int nNumSegment = scoreAreaComp.nSegmentU * scoreAreaComp.nSegmentV;

    // マスを選出
    int nRandomArea = math::Random(0, nNumSegment - 1);

    // 前回のスコアのエリアの保存
    scoreAreaComp.nOldScoreArea = nRandomArea;

    // 縦の分割数分回す
    for (int nCntV = 0; nCntV < scoreAreaComp.nSegmentV; nCntV++)
    {
        // 横の分割数分回す
        for (int nCntU = 0; nCntU < scoreAreaComp.nSegmentU; nCntU++)
        {
            // 計算用の位置
            D3DXVECTOR3 posWk = Const::VEC3_NULL;

            // エリアの分割数の半分を設定
            float fHalfU = scoreAreaComp.nSegmentU * 0.5f;
            float fHalfV = scoreAreaComp.nSegmentV * 0.5f;

            // 中心基準にインデックスを設定  
            float fPosU = (nCntU + 0.5f) - fHalfU;
            float fPosV = (nCntV + 0.5f) - fHalfV;

            posWk.x = fPosU * GridSize.x;
            posWk.y = fPosV * GridSize.y;

            // ポリゴンのIDを格納
            auto polygonID = scoreAreaComp.data[nCnt].polygonID;

            // コンポーネントの取得
            auto& sizeComp = registry.get<Size3DComponent>(polygonID);
            auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);
            auto& colliderComp = registry.get<BoxColliderComponent>(polygonID);
            auto& colorComp = registry.get<ColorComponent>(polygonID);

            // ポリゴンの大きさを設定
            sizeComp.size = { GridSize.x * 0.5f,GridSize.y * 0.5f,0.0f };

            // 持ち主のID
            colliderComp.ownerID = polygonID;
            colliderComp.size = { GridSize.x,GridSize.y,scoreAreaComp.COLLIDER_DEPTH };

            // スコアのエリアとインデックスが一致したら
            if (nRandomArea == nCnt)
            {
                // コライダーの持ち主を変更
                colorComp.col = Color::RED;

                // スコアのエリアフラグオン
                areaPolygonComp.bScoreArea = true;
            }

            colorComp.col.a = scoreAreaComp.POLYGON_ALPHA;;

            // ポリゴンの位置を設定
            areaPolygonComp.destPos = centerPos + posWk;

            nCnt++;
        }
    }

#endif // 0

    return entity;
}

//===================================================
// ロケーターの生成処理
//===================================================
entt::entity FactorySystemEntity::CreateLocator(entt::registry& registry, const D3DXVECTOR3& offSetPos, const entt::entity parentID)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<Tag::LocatorTag>(entity);
    registry.emplace<Transform3DComponent>(entity, offSetPos);

    // 親子関係の作成
    registry.emplace<ParentComponent>(entity, parentID, offSetPos);

    return entity;
}

//===================================================
// メニュー選択の生成
//===================================================
entt::entity FactorySystemEntity::CreateMenuSelect(entt::registry& registry, const int nMaxMenu, const int nNextKey, const int nNextJoykey, const bool bWrap)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<MenuSelectComponent>(entity, nMaxMenu, nNextKey, nNextJoykey,-1,-1, bWrap);
    

    return entity;
}

//===================================================
// チュートリアルのMRTの生成
//===================================================
entt::entity FactorySystemEntity::CreateTutorialMRT(entt::registry& registry)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<Tag::TutorialMRTTag>(entity);
    

    return entity;
}

//===================================================
// リザルトのMRTの生成
//===================================================
entt::entity FactorySystemEntity::CreateResultMRT(entt::registry& registry)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<Tag::ResultMRTTag>(entity);
    

    return entity;
}

//===================================================
// チュートリアルの処理の生成
//===================================================
entt::entity FactorySystemEntity::CreateTutorial(entt::registry& registry)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<TutorialSystemComponent>(entity);
    

    return entity;
}

//===================================================
// タイトルのMRTの生成
//===================================================
entt::entity FactorySystemEntity::CreateTitleMRT(entt::registry& registry)
{
    // entityの生成
    const entt::entity entity = registry.create();
    registry.emplace<Tag::TitleMRTTag>(entity);
    

    return entity;
}

//===================================================
// 球のコライダーの生成
//===================================================
entt::entity FactorySystemEntity::CreateSphereCollider(entt::registry& registry, const entt::entity parent, const float fRadius, const D3DXVECTOR3& pos)
{
    // 生成処理
    const entt::entity entity = registry.create();

    registry.emplace<SphereColliderComponent>(entity, fRadius, parent);
    registry.emplace<ParentComponent>(entity, parent, pos);
    registry.emplace<Transform3DComponent>(entity, pos);

    return entity;
}

//===================================================
// カプセルコライダーの生成
//===================================================
entt::entity FactorySystemEntity::CreateCapsuleCollider(entt::registry& registry, const entt::entity parent, const float fRadius, const D3DXVECTOR3& pos, const D3DXVECTOR3& start, const D3DXVECTOR3& end)
{
    // 生成処理
    const entt::entity entity = registry.create();

    registry.emplace<CapsuleColliderComponent>(entity, start, end, fRadius, parent);
    registry.emplace<ParentComponent>(entity, parent);
    registry.emplace<Transform3DComponent>(entity, pos);

    return entity;
}

//===================================================
// メニュー選択の構築処理
//===================================================
void FactorySystemEntity::BuildMenuSelect(entt::registry& registry, const entt::entity entity, const int nPrevKey, const int nPrevJoyKey)
{
    // メニュー選択の取得
    auto& menuSelectComp = registry.get<MenuSelectComponent>(entity);

    menuSelectComp.nPrevKey = nPrevKey;
    menuSelectComp.nPrevJoyKey = nPrevJoyKey;
}
