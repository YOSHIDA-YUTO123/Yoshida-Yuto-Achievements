//===================================================
//
// マップのオブジェクト同士の判定[FunctionMapObjToMapObh.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FUNCTION_MAPOBJ_TO_MAPOBJ_H_
#define _FUNCTION_MAPOBJ_TO_MAPOBJ_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "functionBase.h"

//***************************************************
// 前方宣言
//***************************************************
class CCollisionMapObjManager; // マップオブジェトのマネージャー
class CCollisionMapObject; // マップオブジェトのマネージャー

//***************************************************
// マップのオブジェクト同士のあたり判定
//***************************************************
class CFunctionMapObjToMapObj : public CFunctionBase
{
public:
	CFunctionMapObjToMapObj(CCollisionMapObjManager* pMapObjectRef);
	~CFunctionMapObjToMapObj() = default;

	void Init(void) override {};
	void Uninit(void) override {};
	void Update(void) override ;
private:
	bool Collision(CCollisionMapObject* pMapObj_A, CCollisionMapObject* pMapObj_B);
private:
	CCollisionMapObjManager* m_pMapObjectRef; // 参照用マップオブジェクト
};

#endif