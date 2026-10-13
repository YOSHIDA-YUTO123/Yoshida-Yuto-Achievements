//==============================================
//
// 当たり判定マップオブジェクトのオーナー [CollisionMapObjManager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _COLLISION_MAPOBJ_MANAGER_H_
#define _COLLISION_MAPOBJ_MANAGER_H_

//**********************************************
// インクルードファイル
//**********************************************
#include <memory>
#include <vector>

//**********************************************
// 前方宣言
//**********************************************
class CCollisionMapObject;
class CCollider;

//**********************************************
// 当たり判定用マップオブジェクトのオーナークラスの定義
//**********************************************
class CCollisionMapObjManager
{
public:
	CCollisionMapObjManager();
	~CCollisionMapObjManager();

	void AddList(CCollisionMapObject* pObject) { m_pList.push_back(pObject); }

	bool Collision(CCollisionMapObject** pCollObject, std::string& XFilePath,CCollider* pAABB, D3DXVECTOR3* pPush, int* pFace, bool* pGravity);

	std::vector<CCollisionMapObject*>& GetList(void) { return m_pList; }
private:
	std::vector<CCollisionMapObject*> m_pList; // マップオブジェクトのリスト
};
#endif