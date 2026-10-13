//**************************************************
// 
// 機能プレイヤーとゴールの当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionCollisionPlayerToGoal.h"
#include "game.h"
#include "player.h"
#include "goal.h"
#include "score.h"
#include "DualUnitTimer.h"
#include "manager.h"
#include "result.h"
#include "transform.h"
#include "GameEndScene.h"

using namespace std;

//**************************************************
// 機能プレイヤーとゴールの当たり判定クラス
//**************************************************
void CFuncCollPlayerToGoal::Update(void)
{
    // プレイヤー,ゴール,スコア,タイマー取得
    CPlayer* pPlayer = CGame::GetPlayer();
    CGoal* pGoal = CGame::GetGoal();
    CScore* pScore = CGame::GetScore();
    CDualUnitTimer* pTimer = CGame::GetTimer();

    // 当たり判定の取得
    CCollisionObject3D* pCollision = pPlayer->GetCollision();

    // 空間情報の取得
    CTransform* pTransform = pCollision->GetTransform();
 
    // 情報の取得
    CTransform::Info playerTransform = pTransform->GetInfo();

    // プレイヤー位置取得
    D3DXVECTOR3 posPlayer = playerTransform.pos;
    D3DXVECTOR3 posPlayerOld = playerTransform.posOld;
    D3DXVECTOR3 movePlayer = pPlayer->GetMove();

    // ゴールと判定を取る
    if (pGoal != nullptr)
    {
        // ゴールに当たっていたら画面遷移
        if (pGoal->Collision(&posPlayer, &movePlayer, posPlayer, posPlayerOld) == true)
        {
            // タイマー保存
            pTimer->Save("data\\TEXT\\NowTime.txt");
            
            pScore->AddScore(pTimer->GetTime() + pTimer->GetTimer());

            // スコア保存
            pScore->Save("data\\TEXT\\NowScore.txt");

            CManager::SetFade(make_unique<CGameEndScene>());
        }
    }

    // プレイヤー位置設定
    pTransform->SetInfo(playerTransform);
    pPlayer->SetMove(movePlayer);
}