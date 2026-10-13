//===================================================
//
// マップのオブジェクト同士の判定[FunctionMapObjToMapObh.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "FunctionMapObjToMapObj.h"
#include "CollisionMapObjManager.h"
#include "Collision.h"
#include "CollisionMapObject.h"
#include "CollisionObject3D.h"

//===================================================
// コンストラクタ
//===================================================
CFunctionMapObjToMapObj::CFunctionMapObjToMapObj(CCollisionMapObjManager* pMapObjectRef) : 
	m_pMapObjectRef(pMapObjectRef)
{
}

//===================================================
// 更新処理
//===================================================
void CFunctionMapObjToMapObj::Update(void)
{
	// 取得できないなら処理しない
	if (m_pMapObjectRef == nullptr) return;

	// すべてのマップオブジェクトを判定するため二つリストを用意する
	
	// リストの取得
	auto& mapObj_A = m_pMapObjectRef->GetList();

	// リストの取得
	auto& mapObj_B = m_pMapObjectRef->GetList();

	// mapObjectA分回す
	for (auto& obj_A : mapObj_A)
	{
		// mapObjectB分回す
		for (auto& obj_B : mapObj_B)
		{
			Collision(obj_A, obj_B);
		}
	}
}

//===================================================
// 当たり判定
//===================================================
bool CFunctionMapObjToMapObj::Collision(CCollisionMapObject* pMapObj_A, CCollisionMapObject* pMapObj_B)
{
	// MapObject_Aのコライダー分回す
	for (auto& collider_A : pMapObj_A->GetCollision()->GetCollider())
	{
		// MapObject_Bのコライダー分回す
		for (auto& collider_B : pMapObj_B->GetCollision()->GetCollider())
		{
			// 相手が自分だったら判定しない
			if (collider_A == collider_B) continue;

			if (CCollisionAABB::Collision(collider_A, collider_B))
			{
				return true;
			}
		}
	}
	return false;
}
