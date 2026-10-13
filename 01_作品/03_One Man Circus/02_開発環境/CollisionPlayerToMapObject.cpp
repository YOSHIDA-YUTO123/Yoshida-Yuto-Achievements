//===================================================
//
// プレイヤーとマップオブジェクトの当たり判定処理 [CollisionPlayerToMapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "CollisionPlayerToMapObject.h"
#include "game.h"
#include "CollisionMapObjManager.h"
#include "Collider.h"
#include "player.h"
#include "Collision.h"

//===================================================
// コンストラクタ
//===================================================
CCollisionPlayerToMapObject::CCollisionPlayerToMapObject(CCollisionMapObjManager* pMapObjectManager) : 
	m_pMapObjectManagerRef(pMapObjectManager)
{
}

//===================================================
// デストラクタ
//===================================================
CCollisionPlayerToMapObject::~CCollisionPlayerToMapObject()
{
	m_pMapObjectManagerRef = nullptr;
}

//===================================================
// 更新処理
//===================================================
void CCollisionPlayerToMapObject::Update(void)
{
#if 0
	// プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	bool bHit = false;

	// プレイヤーのコライダーの取得
	for (auto& colliders : pPlayer->GetCollision()->GetCollider())
	{
		//// 矩形じゃないなら
		//if (colliders->GetType() != CCollider::TYPE_AABB) continue;

		// 当たった面
		int nFace = -1;

		if (m_pMapObjectManagerRef->Collision(colliders,&nFace))
		{
			bHit = true;

			if (nFace == CCollisionAABB::FACE_TOP)
			{
				pPlayer->SetLandingObject(true,false);
			}
			else if (nFace == CCollisionAABB::FACE_BUTTOM)
			{
				pPlayer->SetLandingObject(false,true);
			}
		}
		else if (!bHit)
		{
			pPlayer->SetLandingObject(false, true);
		}
	}
#endif // 0
}
