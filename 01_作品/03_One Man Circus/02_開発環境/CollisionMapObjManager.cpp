//==============================================
//
// 当たり判定マップオブジェクトのオーナー [CollisionMapObjManager.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "CollisionMapObjManager.h"
#include "CollisionMapObject.h"
#include "Collider.h"
#include "Collision.h"
#include "CollisionObject3D.h"
#include "player.h"
#include "game.h"

//==============================================
// コンストラクタ
//==============================================
CCollisionMapObjManager::CCollisionMapObjManager() :
	m_pList()
{
}

//==============================================
// デストラクタ
//==============================================
CCollisionMapObjManager::~CCollisionMapObjManager()
{
	m_pList.clear();
}

//==============================================
// 当たり判定
//==============================================
bool CCollisionMapObjManager::Collision(CCollisionMapObject** pCollObject,std::string& XFilePath,CCollider* pAABB, D3DXVECTOR3* pPush, int* pFace, bool* pGravity)
{
	// 判定結果
	bool bResult = false;

	// マップオブジェクトのリスト
	for (auto& mapobjects : m_pList)
	{
		// 当たり判定の取得
		CCollisionObject3D* pCollision = mapobjects->GetCollision();

		for (auto& colliders : pCollision->GetCollider())
		{
			// OBBじゃないまたは、AABBじゃないならスキップ
			//if (colliders->GetType() != CCollider::TYPE_OBB || pAABB->GetType() != CCollider::TYPE_AABB) continue;

			if (CCollisionAABB::CollisionUnderPos(pAABB, colliders, pPush, pFace))
			{
				// ポインター設定
				(*pCollObject) = mapobjects;

				// ファイルパス設定
				XFilePath = mapobjects->GetXFilePath();

				if (*pFace == CCollisionAABB::FACE_TOP)
				{
					*pGravity = false;
				}
				bResult = true;
			}
		}
	}

	return bResult;
}
