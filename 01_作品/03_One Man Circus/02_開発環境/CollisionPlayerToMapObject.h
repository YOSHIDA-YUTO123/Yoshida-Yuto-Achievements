//===================================================
//
// プレイヤーとマップオブジェクトの当たり判定処理 [CollisionPlayerToMapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISON_PLAYER_TO_MAPOBJECT_H_
#define _COLLISON_PLAYER_TO_MAPOBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "functionBase.h"

//***************************************************
// 前方宣言
//***************************************************
class CCollisionMapObjManager; // マップオブジェクトのマネージャー

//***************************************************
// プレイヤーとマップオブジェクトの当たり判定クラスの定義
//***************************************************
class CCollisionPlayerToMapObject : public CFunctionBase
{
public:
	CCollisionPlayerToMapObject(CCollisionMapObjManager* pMapObjectManager);
	~CCollisionPlayerToMapObject();

	void Init(void) override {};
	void Uninit(void) override {};
	void Update(void) override;
private:
	CCollisionMapObjManager* m_pMapObjectManagerRef; // マップオブジェクトのマネージャーの参照
};
#endif