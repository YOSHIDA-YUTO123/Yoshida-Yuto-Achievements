//===================================================
//
// メニューの見た目の基底クラス [MenuVisual.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_VISUALBASE_H_
#define _MENU_VISUALBASE_H_

//***************************************************
// メニューの見た目の操作インターフェースの定義
//***************************************************
class CMenuVisualBase
{
public:
	CMenuVisualBase() = default;
	virtual ~CMenuVisualBase() = default;

	virtual void Update(const bool bSelect, const D3DXVECTOR3& pos, const D3DXVECTOR2& BaseSize, D3DXCOLOR& col, float& fScal) = 0;
private:
};
#endif