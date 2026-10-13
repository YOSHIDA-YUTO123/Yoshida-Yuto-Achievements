//================================================
//
// マップのオブジェクトクラス [MapObject.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

//************************************************
// インクルードファイル
//************************************************
#include "objectX.h"

//************************************************
// マップのオブジェクトのクラス
//************************************************
class CMapObject : public CObjectX
{
public:
	CMapObject();
	~CMapObject();

	static CMapObject* Create(const D3DXVECTOR3 pos, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:

};
#endif