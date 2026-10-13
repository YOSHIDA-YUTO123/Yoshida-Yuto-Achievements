//**************************************************
// 
// 機能カメラのズーム処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "funcZoomCamera.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "transform.h"
#include "easing.h"
#include "myMath.h"

using namespace std;

CFuncZoomwCamera::CFuncZoomwCamera()
{
    Init();
}

CFuncZoomwCamera::~CFuncZoomwCamera()
{

}

//**************************************************
// 機能カメラのズーム処理初期化
//**************************************************
void CFuncZoomwCamera::Init(void)
{
    // プレイヤー、カメラ取得
    CPlayer* pPlayer = CGame::GetPlayer();
    CCamera* pCamera = CManager::GetCamera();

    // プレイヤー情報取得
    CTransform* pTransform = pPlayer->GetCollision()->GetTransform();

    // 目的位置設定
    m_posDest = pPlayer->GetPosition();
    m_posDest.y += CAMERAHEIGHT;
    m_posDest.z += CAMERADIPS;

    // 元位置設定
    m_posBace = pCamera->GetPosV();
    m_nCnt = 0;
}

//**************************************************
// 機能カメラのズーム処理更新
//**************************************************
void CFuncZoomwCamera::Update(void)
{
    // プレイヤー、カメラ取得
    CPlayer* pPlayer = CGame::GetPlayer();
    CCamera* pCamera = CManager::GetCamera();
    
    // プレイヤー情報取得
    CTransform* pTransform = pPlayer->GetCollision()->GetTransform();

    // プレイヤー位置取得
    D3DXVECTOR3 posPlayer = pTransform->GetInfo().pos;
    D3DXVECTOR3 rotPlayer = pPlayer->GetRotationDest();
    m_posDest = posPlayer; 
    m_posDest.y += CAMERAHEIGHTV;
    m_posDest.z += CAMERADIPS;
    D3DXVECTOR3 vecDiff = m_posDest - m_posBace;

    // カウントアップ
    m_nCnt++;
    m_nCnt = Clamp(m_nCnt, 0, MAXTIME);

    // 時間倍率を求める
    float fPalamTime = (float)m_nCnt / (float)MAXTIME;
    fPalamTime = SelectEasing(EASING_TYPE_OutExpo,fPalamTime);

    D3DXVECTOR3 posV = m_posBace + vecDiff * fPalamTime;
    D3DXVECTOR3 posR = pPlayer->GetPosition();
    posR.y += CAMERAHEIGHT;
    pCamera->SetPosV(posV);
    pCamera->SetPosR(posR);
    pCamera->SetDestPosV(posV);
    pCamera->SetDestPosR(posR);
}