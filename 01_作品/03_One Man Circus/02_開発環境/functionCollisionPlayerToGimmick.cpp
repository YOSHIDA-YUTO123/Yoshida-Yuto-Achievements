//**************************************************
// 
// 機能プレイヤーとギミックの当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionCollisionPlayerToGimmick.h"
#include "game.h"
#include "transform.h"
#include "Collider.h"
#include "RotationFloorManager.h"
#include "RotationFloor.h"
#include "player.h"
#include "statePlayer.h"
#include "Collision.h"

//**************************************************
// 機能プレイヤーとギミックの当たり判定更新
//**************************************************
void  CFuncCollPlayerToGimmick::Update(void)
{
	// Author : YUTO YOSHIDA

	// プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// 回転床のマネージャーの取得
	CRotationFloorManager* pRotationFloorManager = CGame::GetRotationFloorManager();

	// 取得できなかったら処理しない
	if (pPlayer == nullptr || pRotationFloorManager == nullptr) return;

	// 当たり判定の取得
	CCollisionObject3D* pCollision = pPlayer->GetCollision();

	// プレイヤーのコライダー要素を調べる
	for (auto& playerCollider : pCollision->GetCollider())
	{
		// AABBじゃないなら処理を飛ばす
		if (playerCollider->GetType() != CCollider::TYPE_AABB) continue;

		D3DXMATRIX mtxParent;

		int nFace = -1;
		bool bRanding = false;

		// 当たり判定処理
		if (pRotationFloorManager->Collision(playerCollider, &mtxParent, &nFace, &bRanding))
		{
			if (nFace == CCollisionOBB::FACE_TOP)
			{
				// まだ乗ってなかったらバランス状態に移行
				if (bRanding == false)
				{
					pPlayer->ChangeState(std::make_shared<CStatePlayerMovingFloor>());
					pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
				}
				pPlayer->SetRotationFloorLanding(true,false, mtxParent);
				pPlayer->SetIsRanding(true);
			}
			else if (nFace == CCollisionOBB::FACE_BUTTOM)
			{
				pPlayer->SetIsRanding(false);
				pPlayer->SetRotationFloorLanding(false, false, mtxParent);
			}
			else
			{
				pPlayer->SetIsRanding(false);
			}
		}
		else
		{
			//pPlayer->SetIsRanding(false);
			pPlayer->SetRotationFloorLanding(false,true, mtxParent);
		}
	}
}