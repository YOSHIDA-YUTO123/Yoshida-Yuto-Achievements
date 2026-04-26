//===================================================
//
// スコアのエリアの更新処理 [score_area_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "score_area_system.h"
#include "tag_component.hpp"
#include "score_area_component.hpp"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "box_collider_component.hpp"
#include "renderer_component.hpp"
#include "collision.h"
#include "sphere_collider_component.hpp"
#include "utility_math.h"
#include "color_constants.hpp"
#include "color_component.hpp"
#include "minigame_manager.h"
#include "mesh_circle_system.h"
#include "factory_billboard.h"
#include "factory_mesh.h"
#include "score_component.hpp"
#include "utility_math.h"
#include "timer_component.hpp"
#include "minigame_state.h"
#include "velocity_component.hpp"
#include "score_manager.h"
#include "manager.h"
#include "sound.h"
#include "particle_helper.h"
#include "effect_component.hpp"
#include "effect_constants.hpp"
#include "factory_billboard.h"
#include "score_area_polygon_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace ScoreArea_Const
{
    constexpr int PHASE_ONE             = 100;	        // 第一フェーズ開始時間
    constexpr int PHASE_TWO             = 70;	        // 第二フェーズ開始時間
    constexpr int PHASE_THREE           = 40;	        // 第三フェーズ開始時間
                                                        
    constexpr int PHASE_ONE_SCORE       = 1224;         // 第一フェーズのスコア
    constexpr int PHASE_TWO_SCORE       = 5123;         // 第二フェーズのスコア
    constexpr int PHASE_THREE_SCORE     = 11920;        // 第三フェーズのスコア
    constexpr int PHASE_FINAL_SCORE     = 50000;        // 最終フェーズのスコア

    constexpr int INTERVAL              = 100;          // 次当たり判定を取るまでの時間

    constexpr float AREA_POS_RANGE_MIN  = -1500.0f;     // スコアのエリアの発生位置の範囲最小
    constexpr float AREA_POS_RANGE_MAX  = -250.0f;      // スコアのエリアの発生位置の範囲最大

    constexpr float FADE_OUT_SPEED      = 0.01f;        // フェードアウトの速さ
    constexpr float RED_AREA_SHAKE_MIN  = -50.0f;       // 赤いエリアの揺れ幅最小
    constexpr float RED_AREA_SHAKE_MAX  = 50.0f;        // 赤いエリアの揺れ幅最大

    constexpr float BLUE_AREA_SHAKE_MIN = -20.0f;       // 青いエリアの揺れ幅最小
    constexpr float BLUE_AREA_SHAKE_MAX = 20.0f;        // 青いエリアの揺れ幅最大

    constexpr int RED_SHAKE_TIME        = 180;          // 赤いエリアの揺れる時間
    constexpr int BLUE_SHAKE_TIME       = 30;           // 青いエリアの揺れる時間
}

//===================================================
// 更新処理
//===================================================
void ScoreAreaSystem::Update(entt::registry& registry)
{
	// スコアのエリアの取得
	auto view = registry.view<ScoreAreaComponent>();

	// スコアのエリア分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& scoreAreaComp = registry.get<ScoreAreaComponent>(entity);

        // ポリゴンの位置の補間処理
        UpdatePolygon(registry, scoreAreaComp);

        // インターバルがあるなら処理を飛ばす
        if (scoreAreaComp.nInterval >= 0)
        {
            scoreAreaComp.nInterval--;

            continue;
        }

        // 当たって無いなら処理を飛ばす
        if (!CollisionBall(registry, scoreAreaComp))
        {
            continue;
        }

        // フェードアウトの開始
        BeginFadeOut(registry, scoreAreaComp);
	}
}

//===================================================
// ボールとの当たり判定
//===================================================
bool ScoreAreaSystem::CollisionBall(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    // 定数の名前空間の使用
    using namespace ScoreArea_Const;

    // ボールの取得
    auto ballColliderID = registry.view<ColliderTag::BallSphere>().front();

    // 使用できないなら
    if (!registry.valid(ballColliderID))
    {
        return false;
    }

    // ボールの球のコライダーの取得
    auto& ballSphereCollider = registry.get<SphereColliderComponent>(ballColliderID);

    // ボールのIDの取得
    const auto ballID = ballSphereCollider.ownerID;

    int nCnt = 0;

    const int nSegmentU = scoreAreaComp.nSegmentU + 1;
    const int nSegmentV = scoreAreaComp.nSegmentV + 1;

    // 縦の分割数分回す
    for (int nCntV = 0; nCntV < nSegmentV; nCntV++)
    {
        // 横の分割数分回す
        for (int nCntU = 0; nCntU < nSegmentU; nCntU++)
        {
            // ポリゴンの取得
            auto polygonID = scoreAreaComp.data[nCnt].polygonID;

            // コンポーネントの取得
            auto& colliderAABB = registry.get<BoxColliderComponent>(polygonID);
            auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);

            // 当たっていないなら
            if (!Collision::Box::CollisionSphere(registry, colliderAABB, ballSphereCollider))
            {
                nCnt++;

                continue;
            }

            // スコアのエリアじゃないなら
            if (!areaPolygonComp.bScoreArea)
            {
                // 揺れの開始
                BeginShake(registry, polygonID, BLUE_AREA_SHAKE_MIN, BLUE_AREA_SHAKE_MAX, BLUE_SHAKE_TIME);

                nCnt++;

                continue;
            }

            // スコアの設定
            SetScore(registry, ballID);

            // エフェクトの設定処理
            SetEffect(registry, ballID);
            
            // 揺れの開始
            BeginShake(registry, polygonID, RED_AREA_SHAKE_MIN, RED_AREA_SHAKE_MAX, RED_SHAKE_TIME);

            // インターバルを設定
            scoreAreaComp.nInterval = ScoreArea_Const::INTERVAL;

            nCnt++;

            return true;
        }
    }


    return false;
}

//===================================================
// エリアの設定処理
//===================================================
void ScoreAreaSystem::SetArea(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    // 定数の名前空間の使用
    using namespace ScoreArea_Const;

    // エリアの分割数の設定
    SetAreaSegement(registry, scoreAreaComp);

    int nCnt = 0;

    const int nSegmentU = scoreAreaComp.nSegmentU + 1;
    const int nSegmentV = scoreAreaComp.nSegmentV + 1;

    // ポリゴンの大きさを求める
    D3DXVECTOR2 GridSize = Const::VEC2_NULL;
    GridSize.x = scoreAreaComp.size.x / static_cast<float>(nSegmentU);
    GridSize.y = scoreAreaComp.size.y / static_cast<float>(nSegmentV);

    // 分割の総数
    int nNumSegment = nSegmentU * nSegmentV;

    // マスを選出
    int nRandomArea = -1;

    // 前回と違うなら
    while (1)
    {
        nRandomArea = math::Random(0, nNumSegment - 1);

        if(scoreAreaComp.nOldScoreArea != nRandomArea)
        {
            // 前回のスコアのエリアの保存
            scoreAreaComp.nOldScoreArea = nRandomArea;

            break;
        }
    }

    // 要素のリセット
    Reset(registry, scoreAreaComp);

    // 縦の分割数分回す
    for (int nCntV = 0; nCntV < nSegmentV; nCntV++)
    {
        // 横の分割数分回す
        for (int nCntU = 0; nCntU < nSegmentU; nCntU++)
        {
            // 描画を取得
            auto& rendererComp = registry.get<RendererComponent>(scoreAreaComp.data[nCnt].polygonID);

            // アクティブじゃないなら
            if (!scoreAreaComp.data[nCnt].bActive)
            {
                rendererComp.nFlag = RendererComponent::TYPE_NONE;
                scoreAreaComp.data[nCnt].bActive = true;
            }

            // 計算用の位置
            D3DXVECTOR3 posWk = Const::VEC3_NULL;

            // エリアの分割数の半分を設定
            float fHalfU = nSegmentU * 0.5f;
            float fHalfV = nSegmentV * 0.5f;

            // 中心基準にインデックスを設定  
            float fPosU = (nCntU + 0.5f) - fHalfU;
            float fPosV = (nCntV + 0.5f) - fHalfV;

            posWk.x = fPosU * GridSize.x;
            posWk.y = fPosV * GridSize.y;

            // ポリゴンのIDを格納
            auto polygonID = scoreAreaComp.data[nCnt].polygonID;

            // コンポーネントの取得
            auto& sizeComp = registry.get<Size3DComponent>(polygonID);
            auto& transformComp = registry.get<Transform3DComponent>(polygonID);
            auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);
            auto& colorComp = registry.get<ColorComponent>(polygonID);
            auto& colliderComp = registry.get<BoxColliderComponent>(polygonID);

            // ポリゴンの大きさを設定
            sizeComp.size = { GridSize.x * 0.5f,GridSize.y * 0.5f,0.0f };

            // 持ち主のIDの設定
            colliderComp.ownerID = polygonID;
            colliderComp.size = { GridSize.x,GridSize.y,scoreAreaComp.COLLIDER_DEPTH };

            // スコアのエリアとインデックスが一致したら
            if (nRandomArea == nCnt)
            {
                // スコアエリアフラグオン
                areaPolygonComp.bScoreArea = true;

                // コライダーの持ち主を変更
                colorComp.col.r = Color::RED.r;
                colorComp.col.g = Color::RED.g;
                colorComp.col.b = Color::RED.b;
            }
            else
            {
                areaPolygonComp.bScoreArea = false;
            }

            // ポリゴンの位置
            D3DXVECTOR3 polygonPos = scoreAreaComp.centerPos + posWk;

            // ポリゴンの位置を設定
            transformComp.pos.x = polygonPos.x;
            transformComp.pos.y = polygonPos.y;
            transformComp.pos.z = polygonPos.z + math::Random(AREA_POS_RANGE_MIN, ScoreArea_Const::AREA_POS_RANGE_MAX);

            // ポリゴンの目的の位置を設定
            areaPolygonComp.destPos = polygonPos;

            // リセット
            areaPolygonComp.nShakeTime = 0;

            nCnt++;
        }
    }
}

//===================================================
// 要素のリセット
//===================================================
void ScoreAreaSystem::Reset(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    // スコアのエリアのデータ分回す
    for (auto& scoreAreaData : scoreAreaComp.data)
    {
        scoreAreaData.bActive = false;

        // 色の取得
        auto& colorComp = registry.get<ColorComponent>(scoreAreaData.polygonID);

        // 描画を取得
        auto& rendererComp = registry.get<RendererComponent>(scoreAreaData.polygonID);
        rendererComp.nFlag = RendererComponent::TYPE_NO_DISP;

        // コライダーの持ち主を変更
        colorComp.col.r = Color::AQUA.r;
        colorComp.col.g = Color::AQUA.g;
        colorComp.col.b = Color::AQUA.b;
        colorComp.col.a = scoreAreaComp.POLYGON_ALPHA;
    }
}

//===================================================
// エリアの分割数の設定
//===================================================
void ScoreAreaSystem::SetAreaSegement(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    using namespace ScoreArea_Const;

    // タイマーの取得
    const auto timerID = registry.view<TimerComponent>().front();

    // 使用できないなら
    if (!registry.valid(timerID))
    {
        return;
    }

    // 現在のゲームの時間の取得
    int nTime = registry.get<TimerComponent>(timerID).nTime;

    // 初期フェーズなら
    if (nTime >= PHASE_ONE)
    {
        scoreAreaComp.nSegmentU = 1;
        scoreAreaComp.nSegmentV = 0;
    }
    // 第一フェーズなら
    else if (math::IsInRange(nTime, PHASE_TWO, PHASE_ONE))
    {
        scoreAreaComp.nSegmentU = 2;
        scoreAreaComp.nSegmentV = 0;
    }
    // 第二フェーズなら
    else if (math::IsInRange(nTime, PHASE_THREE, PHASE_TWO))
    {
        scoreAreaComp.nSegmentU = 2;
        scoreAreaComp.nSegmentV = 1;
    }
    // 第3フェーズなら
    else if (nTime <= PHASE_THREE)
    {
        scoreAreaComp.nSegmentU = 2;
        scoreAreaComp.nSegmentV = 2;
    }
}

//===================================================
// スコアの設定
//===================================================
void ScoreAreaSystem::SetScore(entt::registry& registry, const entt::entity ball)
{
    // ミニゲームの補助クラスの取得
    CMiniGameManager* pMinigameManager = CMinigameState::GetManager();

    if (pMinigameManager == nullptr) return;

    using namespace ScoreArea_Const;

    // タイマーの取得
    const auto timerID = registry.view<TimerComponent>().front();

    // 使用できないなら
    if (!registry.valid(timerID))
    {
        return;
    }

    // プレイヤーのコンボ数の取得
    int nPlayerComb = pMinigameManager->GetPlayerComp();

    // 現在のゲームの時間の取得
    int nTime = registry.get<TimerComponent>(timerID).nTime;

    // スコアの取得
    const auto scoreID = registry.view<ScoreComponent>().front();

    // スコアの取得
    auto& scoreComp = registry.get<ScoreComponent>(scoreID);

    // 初期フェーズなら
    if (nTime >= PHASE_ONE)
    {
        // スコアの加算
        ScoreManager::AddScore(registry, PHASE_ONE_SCORE);
    }
    // 第一フェーズなら
    else if (math::IsInRange(nTime, PHASE_TWO, PHASE_ONE))
    {
        // スコアの加算
        ScoreManager::AddScore(registry, PHASE_TWO_SCORE);
    }
    // 第二フェーズなら
    else if (math::IsInRange(nTime, PHASE_THREE, PHASE_TWO))
    {
        // スコアの加算
        ScoreManager::AddScore(registry, PHASE_THREE_SCORE);
    }
    // 第3フェーズなら
    else if (nTime <= PHASE_THREE)
    {
        // スコアの加算
        ScoreManager::AddScore(registry, PHASE_FINAL_SCORE);
    }

    // ボールの移動量の取得
    auto& ballVelocityComp = registry.get<VelocityComponent>(ball);

    int nMoveLength = static_cast<int>(ballVelocityComp.fMoveLength);

    // 移動速度でスコア加算
    ScoreManager::AddScore(registry, ScoreManager::BASE_SCORE * nMoveLength);
    ScoreManager::AddScore(registry, nMoveLength * nPlayerComb);

    // スコアの設定
    pMinigameManager->SetScore(scoreComp.nScore);
}

//===================================================
// エフェクトの設定処理
//===================================================
void ScoreAreaSystem::SetEffect(entt::registry& registry, const entt::entity ball)
{
    // コライダーの位置の取得
    const auto ownerTransformComp = registry.get<Transform3DComponent>(ball);

    // ワールド座標の取得
    D3DXVECTOR3 worldPos = math::GetPositionFromMatrix(ownerTransformComp.mtxWorld);

    // スコアマネージャーの使用
    using namespace ScoreManager;

    // スコアの状態の取得
    State state = GetState(registry);

    // マネージャーの取得
    CManager* pManager = CManager::GetInstance();

    // 音の取得
    CSound* pSound = pManager->GetSound();

    if (pSound != nullptr)
    {
        pSound->Play(CSound::LABEL_SE_SCORE_AREA_HIT);
    }

    ParticleComponent::Param param;

    // パラメータの設定
    param.pos = worldPos;
    param.col = Color::RED;
    param.nEffectFlag = EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS | EffectComponent::FLAG_GRAVITY;
    param.move.x = 10.0f;
    param.move.y = 10.0f;
    param.move.z = 10.0f;
    param.fRadius = 20.0f;
    param.nLife = 120;
    param.nNum = 100;

    // パーティクルの生成
    FactoryBillboard::Create::Particle(registry, param);

    if (state == State::Normal)
    {
        auto circleID = FactoryMesh::Create::Circle(registry, worldPos, { D3DX_PI * 0.5f,0.0f,0.0f }, { Color::CORAL.r,Color::CORAL.g,Color::CORAL.b, 0.3f }, "");
        FactoryMesh::Build::Circle(registry, circleID, 2.0f, 0.0f, 150.0f, 30);
        FactoryMesh::Build::Circle(registry, circleID, 0.0f);
    }
    else if (state == State::Advanced)
    {
        // パラメータの設定
        param.pos = worldPos;
        param.col = Color::RED;
        param.texturePath = EffectConstants::TEXTURE_NORMAL;
        param.nEffectFlag = EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS | EffectComponent::FLAG_GRAVITY;
        param.move.x = 0.0f;
        param.move.y = 10.0f;
        param.move.z = 10.0f;
        param.fRadius = 10.0f;
        param.nLife = 120;
        param.nNum = 10;

        auto circleID = FactoryMesh::Create::Circle(registry, worldPos, { D3DX_PI * 0.5f,0.0f,0.0f }, Color::CORAL, "");
        FactoryMesh::Build::Circle(registry, circleID, 2.0f, 0.0f, 150.0f, 30);
        FactoryMesh::Build::Circle(registry, circleID, 0.0f);

        FactoryBillboard::Create::Particle(registry, param);
    }

    if (state != State::Normal)
    {
        auto circleID = FactoryMesh::Create::Circle(registry, worldPos, { D3DX_PI * 0.5f,0.0f,0.0f }, Color::CORAL, "");
        FactoryMesh::Build::Circle(registry, circleID, 2.0f, 0.0f, 150.0f, 30);
        FactoryMesh::Build::Circle(registry, circleID, 0.0f);

        circleID = FactoryMesh::Create::Circle(registry, worldPos, { D3DX_PI * 0.5f,0.0f,0.0f }, Color::CORAL, "");
        FactoryMesh::Build::Circle(registry, circleID, 2.0f, 0.0f, 100.0f, 30);
        FactoryMesh::Build::Circle(registry, circleID, 0.0f);

        circleID = FactoryMesh::Create::Circle(registry, worldPos, { D3DX_PI * 0.5f,0.0f,0.0f }, Color::CORAL, "");
        FactoryMesh::Build::Circle(registry, circleID, 2.0f, 0.0f, 50.0f, 30);
        FactoryMesh::Build::Circle(registry, circleID, 0.0f);
    }
}

//===================================================
// ポリゴンの更新処理
//===================================================
void ScoreAreaSystem::UpdatePolygon(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    // 定数の名前空間の使用
    using namespace ScoreArea_Const;

    int nCnt = 0;

    const int nSegmentU = scoreAreaComp.nSegmentU + 1;
    const int nSegmentV = scoreAreaComp.nSegmentV + 1;

    // エリアの設定可能フラグ
    bool bSetArea = false;

    // 縦の分割数分回す
    for (int nCntV = 0; nCntV < nSegmentV; nCntV++)
    {
        // 横の分割数分回す
        for (int nCntU = 0; nCntU < nSegmentU; nCntU++, nCnt++)
        {
            // ポリゴンのIDを格納
            auto polygonID = scoreAreaComp.data[nCnt].polygonID;

            // コンポーネントの取得
            auto& transformComp = registry.get<Transform3DComponent>(polygonID);
            auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);
            auto& colorComp = registry.get<ColorComponent>(polygonID);

            // 目的の位置に近づける
            transformComp.pos += (areaPolygonComp.destPos - transformComp.pos) * 0.1f;

            // 揺れの更新処理
            UpdateShake(registry, polygonID);

            // フェードアウトできるなら
            if (!areaPolygonComp.bFadeOut)
            {
                continue;
            }

            // 色を減らす
            colorComp.col.a -= FADE_OUT_SPEED;

            // フェードアウトが終わったら
            if (colorComp.col.a <= 0.0f)
            {
                colorComp.col.a = 0.0f;

                bSetArea = true;
                areaPolygonComp.bFadeOut = false;
            }
        }
    }

    // スコアのエリアを設定できるなら
    if (bSetArea)
    {
        // エリアの設定
        SetArea(registry, scoreAreaComp);
    }

}

//===================================================
// フェードアウトの開始
//===================================================
void ScoreAreaSystem::BeginFadeOut(entt::registry& registry, ScoreAreaComponent& scoreAreaComp)
{
    int nCnt = 0;

    const int nSegmentU = scoreAreaComp.nSegmentU + 1;
    const int nSegmentV = scoreAreaComp.nSegmentV + 1;

    // 縦の分割数分回す
    for (int nCntV = 0; nCntV < nSegmentV; nCntV++)
    {
        // 横の分割数分回す
        for (int nCntU = 0; nCntU < nSegmentU; nCntU++)
        {
            // スコアのエリアのポリゴンの取得
            auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(scoreAreaComp.data[nCnt].polygonID);

            // フェードアウトフラグが立ってないなら
            if (!areaPolygonComp.bFadeOut)
            {
                areaPolygonComp.bFadeOut = true;
            }

            nCnt++;
        }
    }
}

//===================================================
// 揺れの開始
//===================================================
void ScoreAreaSystem::BeginShake(entt::registry& registry, const entt::entity polygonID, const float fRangeMin, const float fRangeMax, const int nTime)
{
    // コンポーネントの取得
    auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);

    // 揺れのパラメータの設定
    areaPolygonComp.fShakeRangeMax = fRangeMax;
    areaPolygonComp.fShakeRangeMin = fRangeMin;
    areaPolygonComp.nShakeTime = nTime;
}

//===================================================
// エリアの揺れの処理
//===================================================
void ScoreAreaSystem::UpdateShake(entt::registry& registry, const entt::entity polygonID)
{
    // 定数の名前空間の使用
    using namespace ScoreArea_Const;

    // コンポーネントの取得
    auto& transformComp = registry.get<Transform3DComponent>(polygonID);
    auto& areaPolygonComp = registry.get<ScoreAreaPolygonComponent>(polygonID);

    // 揺れの時間を減らす
    areaPolygonComp.nShakeTime--;

    // 揺れの時間が終わったなら
    if (areaPolygonComp.nShakeTime <= 0)
    {
        return;
    }

    // 目的の位置に近づける
    transformComp.pos.x = areaPolygonComp.destPos.x + math::Random(areaPolygonComp.fShakeRangeMin, areaPolygonComp.fShakeRangeMax);
    transformComp.pos.y = areaPolygonComp.destPos.y + math::Random(areaPolygonComp.fShakeRangeMin, areaPolygonComp.fShakeRangeMax);
}