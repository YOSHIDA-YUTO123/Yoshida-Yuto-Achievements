//**************************************************
// 
// 機能カメラの追従処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionForrowCamera.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "transform.h"

using namespace std;

//**************************************************
// 機能カメラの追従処理更新
//**************************************************
void CFuncForrowCamera::Update(void)
{
    // プレイヤー、カメラ取得
    CPlayer* pPlayer = CGame::GetPlayer();
    CCamera* pCamera = CManager::GetCamera();

    // 当たり判定の取得
    CCollisionObject3D* pCollision = pPlayer->GetCollision();

    CTransform* pTransform = pCollision->GetTransform();

    // プレイヤー位置取得
    D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
    D3DXVECTOR3 rotPlayer = pPlayer->GetRotationDest();
    D3DXVECTOR3 posRNow = pCamera->GetPosR();
    D3DXVECTOR3 posVNow = pCamera->GetPosV();
    D3DXVECTOR3 posR = posPlayer;
    D3DXVECTOR3 posV;
    posR.x += sinf(rotPlayer.y) * CAMERADISTANCE_PTOR;
    posR.z += cosf(rotPlayer.y) * CAMERADISTANCE_PTOR;
    posR.y += CAMERAHEIGHT;
    posV = posR;
    posV.z -= sinf(CAMERAANGLE) * CAMERADISTANCE;
    posV.y += cosf(CAMERAANGLE) * CAMERADISTANCE;
    D3DXVECTOR3 vecR = posR - posRNow;
    D3DXVECTOR3 vecV = posV - posVNow;
    D3DXVECTOR3 ansR = posVNow + vecV * PALAM;
    D3DXVECTOR3 ansV = posRNow + vecR * PALAM;

    // カメラの限界位置
    if (ansR.x <= LIMITXA)ansR.x = LIMITXA;
    if (ansR.x >= LIMITXB)ansR.x = LIMITXB;
    if (ansR.z <= LIMITZ)ansR.z = LIMITZ;
    if (ansV.x <= LIMITXA)ansV.x = LIMITXA;
    if (ansV.x >= LIMITXB)ansV.x = LIMITXB;
    if (ansV.z <= LIMITZ)ansV.z = LIMITZ;

    pCamera->SetPosV(ansR);
    pCamera->SetPosR(ansV);
}