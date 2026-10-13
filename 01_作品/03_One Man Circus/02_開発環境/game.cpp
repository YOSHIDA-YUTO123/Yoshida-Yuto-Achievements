//==============================================
//
// ゲームシーン [game.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "game.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "line.h"
#include "goal.h"
#include <fstream>
#include "json.hpp"
#include "debugproc.h"
#include "title.h"
#include "score.h"
#include "input.h"
#include "result.h"
#include "Audience.h"
#include "functionBase.h"
#include "functionCollisionPlayerToDrumcan.h"
#include "functionCollisionPlayerToGimmick.h"
#include "functionCollisionPlayerToGoal.h"
#include "functionForrowCamera.h"
#include "functionDebugCamera.h"
#include "DrumCan.h"
#include "RotationFloor.h"
#include "transform.h"
#include "RotationFloorManager.h"
#include "AudienceManager.h"
#include "functionGame.h"
#include "rulebase.h"
#include "ruleJuggling.h"
#include "ruleHula.h"
#include "CollisionMapObjManager.h"
#include "pause.h"
#include "meshfield.h"
#include "dome.h"
#include "StageLoader.h"
#include "object3D.h"
#include "CollisionPlayerToMapObject.h"
#include "cylinder.h"
#include "functionDebugRule.h"
#include "CollisionMapObject.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "fade.h"
#include "GameManager.h"
#include "DualUnitTimer.h"
#include "light.h"
#include "FunctionMapObjToMapObj.h"
#include "GameEndScene.h"
#include "functionGimickSpotLight.h"
#include "functionForrowCamera.h"
#include "funcZoomCamera.h"

using namespace nlohmann;
using namespace std;

// 静的メンバー変数宣言
CPlayer* CGame::m_pPlayer = nullptr;	    // プレイヤー
CGoal* CGame::m_pGoal = nullptr;		    // ゴール
CScore* CGame::m_pScore = nullptr;			// スコア
CDualUnitTimer* CGame::m_pTimer = nullptr;	// タイマーへのポインタ
CRotationFloorManager* CGame::m_pRotationFloorManager = nullptr; // 回転床のマネージャーへのポインタ
std::unique_ptr<CCollisionMapObjManager> CGame::m_pCollMapObjManager = nullptr;// 当たり判定するマップオブジェクトのマネージャー
CRuleBase* CGame::m_pRule = nullptr;                                // ルール基底クラス
int CGame::m_nSituation = SITUATION_NONE;	                        //状況管理変数
vector<std::unique_ptr<CFunctionBase>> CGame::m_vpFunction;	// 機能ポインタ
vector<CAudienceManager*> CGame::m_vectorAudience;             // 観客

//==============================================
// コンストラクタ
//==============================================
CGame::CGame() : CScene(MODE_GAME)
{
    m_pRule = nullptr;              // ルール基底クラス
    m_fCameraDistance = 0.0f;       // カメラの距離
    m_rotCamera = Const::VEC3_NULL;	// カメラの角度
    m_pGameManager = nullptr;
    m_pBall = nullptr;
   
}

//==============================================
// デストラクタ
//==============================================
CGame::~CGame()
{
}

//==============================================
// 初期化処理
//==============================================
HRESULT CGame::Init(void)
{
    // ライトの取得
    CLight* pLight = CManager::GetLight();

    pLight->Init();
#if 1
    pLight->SetDirectional(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.2f), { 0.0f,-0.1f,0.0f }, Const::VEC3_NULL);

    // フィールド
    CMeshField::Create(D3DXVECTOR3(0.0f, 0.5f, 0.0f), 64, 64, D3DXVECTOR2(5000.0f, 5000.0f));
#else
    // フィールド
    CMeshField::Create(D3DXVECTOR3(0.0f, 0.5f, 0.0f), 1, 1, D3DXVECTOR2(5000.0f, 2000.0f));
    pLight->SetDirectional(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-0.52f, -0.59f, 0.62f), { 0.0f,0.0f,0.0f });
#endif // 0


#ifdef _DEBUG
    //float fWidth = 10.0f;
    //float fLength = 500.0f;
    //D3DXCOLOR col = { 1.0f,1.0f,1.0f,0.4f };

    CLine::Create(Const::VEC3_NULL, D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
    CLine::Create(Const::VEC3_NULL, D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
    CLine::Create(Const::VEC3_NULL, D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif

    // マップオブジェクトのマネージャー
    m_pCollMapObjManager = std::make_unique<CCollisionMapObjManager>();
    CStageLoader::Load(m_pCollMapObjManager.get(), "stage000.json");

    // カメラ取得
    CCamera* pCamera = CManager::GetCamera();

    // プレイヤーの生成
    if (m_pPlayer == nullptr)
    {
        m_pPlayer = CPlayer::Create();
    }

    // ゴール生成
    if (m_pGoal == nullptr)
    {
        m_pGoal = CGoal::Create();
        m_pGoal->SetRadius(20.0f);
    }

    // ゲームマネージャーの生成
    m_pGameManager = std::make_unique<CGameManager>();

    // スコア生成
    if (m_pScore == nullptr)
    {
        m_pScore = CScore::Create(D3DXVECTOR3(600.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 50.0f), 10000000, false, m_pGameManager.get());
    }

    // タイマー生成
    if (m_pTimer == nullptr)
    {
        m_pTimer = CDualUnitTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.6f, 55.0f, 0.0f), D3DXVECTOR2(70.0f, 45.0f), 180, m_pGameManager.get());
    }

    // 壁
    CObject3D* pWall = CObject3D::Create(D3DXVECTOR3(0.0f, 200.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3000.0f, 200.0f, 0.0f), "gameback.jpg");
    pWall->SetTexture(0.0f, 0.0f, 10.0f, 1.0f);

    //// 客席
    //CObject3D* pWallAudience = CObject3D::Create(D3DXVECTOR3(0.0f, 15.0f, 240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3000.0f, 30.0f, 0.0f), "gamefloor01.jpg");
    //pWallAudience->SetTexture(0.0f, 0.0f, 25.0f, 1.0f);

    // 観客
    m_vectorAudience.push_back(CAudienceManager::Create(D3DXVECTOR3(-660.0f, 30.0f, 250.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f));
    m_vectorAudience.push_back(CAudienceManager::Create(D3DXVECTOR3(-680.0f, 40.0f, 280.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f));
    m_vectorAudience.push_back(CAudienceManager::Create(D3DXVECTOR3(-660.0f, 60.0f, 320.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f));

    // ゲームのシステムファイルを読み込む
    ReadJsonSystemFile();

    pCamera->SetCamera(D3DXVECTOR3(-400.0f,120.0f,-280.0f),D3DXVECTOR3(-400.0f,20.0f,50.0f), m_rotCamera);
    //pCamera->SetDestPosV(D3DXVECTOR3(-400.0f, 120.0f, -280.0f));
    //pCamera->SetDestPosR(D3DXVECTOR3(-400.0f, 20.0f, 50.0f));
    // カメラの追従処理
    //pCamera->SetDistance(m_fCameraDistance*1.5f);
    //pCamera->SetRot(m_rotCamera);

    AddFuncTion(make_unique<CFuncRulu>());                // ルール管理
    AddFuncTion(make_unique<CFuncCollPlayerToGimmick>()); // ギミック判定処理
    AddFuncTion(make_unique<CFuncCollPlayerToGoal>());    // ゴール判定処理
    AddFuncTion(make_unique<CFuncCollPlayerToDrumcan>()); // ドラム缶判定処理
    AddFuncTion(make_unique<CFuncForrowCamera>());        // 追従カメラ
    AddFuncTion(make_unique<CFuncDebCamera>());           // デバッグ用カメラ操作
    AddFuncTion(make_unique<CFuncDebRule>());             // デバッグ用ルール
    AddFuncTion(make_unique<CCollisionPlayerToMapObject>(m_pCollMapObjManager.get()));   // プレイヤーとマップオブジェクトの当たり判定
    AddFuncTion(make_unique<CFunctionMapObjToMapObj>(m_pCollMapObjManager.get()));      // マップオブジェクトとマップオブジェクトの当たり判定
    AddFuncTion(make_unique<CFunctionGimickSpotLight>(m_pCollMapObjManager.get()));      // 近いギミックにスポットライトを当てる処理

    m_pBall = CObjectX::Create(D3DXVECTOR3(-620.0f, 40.0f, 0.0f), "MapObjectFile\\ball.x", D3DXVECTOR3(0.0f, 0.0f, -0.1f));

    // 回転床のマネージャーの生成
    m_pRotationFloorManager = CRotationFloorManager::Create();

    // 回転床の生成
    m_pRotationFloorManager->CreateFloor(D3DXVECTOR3(-20.0f, 90.0f, -90.0f), 1.5f, 0.05f);
    
    // 回転床の生成
    m_pRotationFloorManager->CreateFloor(D3DXVECTOR3(330.0f, 110.0f, -130.0f), 1.5f, 0.1f);

    // 回転床の生成
    m_pRotationFloorManager->CreateFloor(D3DXVECTOR3(734.0f,80.0f, -130.0f), 0.5f, 0.15f);

    // 回転床の生成
    m_pRotationFloorManager->CreateFloor(D3DXVECTOR3(1500.0f, 70.0f, 150.0f), 2.5f, 0.25f);

    //m_pRotationFloorManager->CreateFloor(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 0.0f,0.1f);

    CSound* pSound = CManager::GetSound();
    if (pSound != nullptr)
    {
        pSound->Play(CSound::SOUND_LABEL_GAMEBGM);
        pSound->Play(CSound::SOUND_LABEL_SE_NORMAL,0.3f);
    }

    return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CGame::Uninit(void)
{
    CPauseManager::GetInstance()->Uninit();

    m_pGameManager.reset();

    // ボール破棄
    if (m_pBall != nullptr)
    {
        m_pBall = nullptr;
    }

    // マップオブジェクトのマネージャーの破棄
    if (m_pCollMapObjManager != nullptr)
    {
        m_pCollMapObjManager.reset();
    }

    if (m_pRotationFloorManager != nullptr)
    {
        m_pRotationFloorManager->Uninit();
        m_pRotationFloorManager = nullptr;
    }

    //  観客削除
    m_vectorAudience.clear();

    // プレイヤーの破棄
    if (m_pPlayer != nullptr)
    {
        //m_pPlayer->Uninit();
        m_pPlayer = nullptr;
    }

    // ゴールの破棄
    if (m_pGoal != nullptr)
    {
        //m_pGoal->Uninit();
        m_pGoal = nullptr;
    }

    // スコア破棄
    if (m_pScore != nullptr)
    {
        m_pScore = nullptr;
    }

    // タイマー破棄
    if (m_pTimer != nullptr)
    {
        m_pTimer = nullptr;
    }

    // ルール基底クラス破棄
    if (m_pRule != nullptr)
    {
        // メモリの破棄
        delete m_pRule;
        m_pRule = nullptr;
    }

    // 機能を削除
    m_vpFunction.clear();
}

//==============================================
// 更新処理
//==============================================
void CGame::Update(void)
{
    //// カメラの追従処理
    //CManager::GetCamera()->SetDistance(m_fCameraDistance);

    if (!CPauseManager::GetPause())// ポーズ状態じゃないなら
    {
        // 機能を回す
        for (auto& iterFunc : m_vpFunction)
        {
            iterFunc->Update();
        }
    }

    // ゲームマネージャーの更新処理
    if (m_pGameManager != nullptr)
    {
        m_pGameManager->Update();
    }

    // キーボード取得
    CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

    // ポーズの切り替え
    CPauseManager::GetInstance()->EnablePause();

    // ポーズの選択
    CPauseManager::GetInstance()->SelectMenu();

#ifdef _DEBUG
    // フェードの取得
    CFade* pFade = CManager::GetFade();


    if (pInputKeyBoard->GetPress(DIK_G))
    {
        auto p = CParticle::Create({ 0.0f,10.0f,0.0f }, Const::WHITE, 10.0f, 1, CParticle::TYPE_SMOKE);
        p->SetParam({ 0.0f,3.0f,0.0f }, 1, 60);
        p->SetAnim(4, 3, 10, true);
    }

    if (pInputKeyBoard->GetTrigger(DIK_F8))
    {
        pFade->SetFade(std::make_unique<CGameEndScene>());
    }
    if (pInputKeyBoard->GetTrigger(DIK_F9))
    {
        CManager::GetFade()->SetFade(std::make_unique<CResultWin>());
    }
    if (pInputKeyBoard->GetTrigger(DIK_F10))
    {
        CManager::GetFade()->SetFade(std::make_unique<CResultLose>());
    }

    CDebugProc::Print("score:%d\n", m_pScore->GetScore());

#endif // _DEBUG

    //if (pInputKeyBoard->GetTrigger(DIK_J))
    //{
    //    auto p = CParticle::Create({ -200.0f,30.0f,0.0f }, Const::WHITE, 10.0f, 10);
    //    p->SetParam({ 15.0f,5.0f,15.0f }, 1, 60);
    //    p->SetParam(0.25f,-0.2f);
    //}
}

//==============================================
// 描画処理
//==============================================
void CGame::Draw(void)
{
}

//==============================================
// 機能追加処理処理
//==============================================
void CGame::AddFuncTion(std::unique_ptr<CFunctionBase> function)
{
    m_vpFunction.push_back(std::move(function));
}

//==============================================
// ルールの破棄
//==============================================
void CGame::DestoroyRule(void)
{
    if (m_pRule != nullptr)
    {
        // 終了処理
        m_pRule->Uninit();

        // メモリの破棄
        delete m_pRule;
        m_pRule = nullptr;
    }
}

//==============================================
// ルール変更関数
//==============================================
void CGame::ChangeRule(CRuleBase* pNewRule)
{
    if (m_pRule != nullptr)
    {
        // 現在のルールの破棄
        DestoroyRule();
    }

    if (pNewRule != nullptr && m_pRule == nullptr)
    {
        m_pRule = pNewRule;

        // 初期化処理
        m_pRule->Init();
    }
}

//==============================================
// ゲームシーンプレイヤー状態管理処理
//==============================================
void CGame::PlayerStateManagement(void)
{

}

//==============================================
// ゲームシーンシステムファイル読込処理
//==============================================
void CGame::ReadJsonSystemFile(void)
{
    // ファイルを読込
    ifstream ifs("data\\SYSTEM\\system_game.json");

    if (ifs.is_open() == false)
    {
        MessageBox(NULL, "CGame::ReadJsonSystemFile() > ファイルが開けませんでした", "Error", MB_OK);
        return;
    }

    // ジェイソンオブジェクト生成
    json json;

    // jsonの情報を読み込む
    ifs >> json;

    // 情報を移し替える
    m_fCameraDistance = json["CAMERA_DISTANCE"];
    m_rotCamera.x = json["CAMERA_ANGLE_X"];
    m_rotCamera.y = json["CAMERA_ANGLE_Y"];
    m_rotCamera.z = json["CAMERA_ANGLE_Z"];

    // プレイヤーの情報取得し設定
    if (m_pPlayer != nullptr)
    {
        m_pPlayer->SetMovement(json["FLOAT_PLAYER_MOVEMENT"]);      // 移動量
        m_pPlayer->SetJumpment(json["FLOAT_PLAYER_JUMPMENT"]);      // ジャンプ量
        m_pPlayer->SetFriction(json["FLOAT_PLAYER_FRICTION"]);      // 摩擦係数
        m_pPlayer->SetFrictionMemo(json["FLOAT_PLAYER_FRICTION"]);      // 摩擦係数
        m_pPlayer->SetGravity(json["FLOAT_PLAYER_GRAVITY"]);        // 重力量
        m_pPlayer->SetLowLimit(json["FLOAT_PLAYER_LOWLIMIT"]);      // 下限値
        m_pPlayer->SetXLimit(json["FLOAT_PLAYER_XLIMIT"]);          // Xの限界地
        m_pPlayer->SetZLimit(json["FLOAT_PLAYER_ZLIMIT"]);          // Zの限界地
        m_pPlayer->SetCountMax(json["INT_PLAYER_COUNTMAX"]);        // カウンターの最大値
        m_pPlayer->SetRebuildTime(json["INT_PLAYER_REBUILDTIME"]);  // カウンターの最大値

        // 初期位置設定
        D3DXVECTOR3 posApp;
        posApp.x = json["FLOAT_PLAYER_APPPOSX"];
        posApp.y = json["FLOAT_PLAYER_APPPOSY"];
        posApp.z = json["FLOAT_PLAYER_APPPOSZ"];

        // 当たり判定の取得
        CCollisionObject3D* pCollision = m_pPlayer->GetCollision();

        CTransform* pTransform = pCollision->GetTransform();

        CTransform::Info transform = pTransform->GetInfo();

        transform.pos = posApp;

        pTransform->SetInfo(transform);
    }

    // ゴール情報取得し設定
    if (m_pGoal != nullptr)
    {
        D3DXVECTOR3 pos;
        pos.x = json["FLOAT_GOAL_POS_X"];
        pos.y = json["FLOAT_GOAL_POS_Y"];
        pos.z = json["FLOAT_GOAL_POS_Z"];
        m_pGoal->SetPosition(pos);
        string str = json["FILEPATH_GOAL_MODEL"];
        m_pGoal->LoadModel(str.c_str());
    }

    ifs.close();
}

//******************************************************************
// 観客の設定
//******************************************************************
void CGame::SetAnimation(CAudience::ANIMETYPE animetype,int nNum)
{
    if (nNum >= (int)m_vectorAudience.size() || nNum < 1)
    {
        nNum = (int)m_vectorAudience.size();
    }

    for (int nCnt = 0;nCnt < nNum;nCnt++)
    {
        m_vectorAudience[nCnt]->SetAnimation(animetype);
    }
}