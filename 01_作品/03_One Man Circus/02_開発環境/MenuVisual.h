//===================================================
//
// メニューの見た目の処理 [MenuVisual.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_VISUAL_H_
#define _MENU_VISUAL_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "MenuVisualBase.h"

//***************************************************
// メニューハイライトの見た目の作成
//***************************************************
class CMenuHighLightVisual : public CMenuVisualBase
{
public:
	CMenuHighLightVisual() = default;
	~CMenuHighLightVisual() = default;

	void Update(const bool bSelect, const D3DXVECTOR3& pos, const D3DXVECTOR2& BaseSize, D3DXCOLOR& col, float& fScal) override;
};

//***************************************************
// メニューハイライト点滅の見た目の作成
//***************************************************
class CMenuHighLightFlashVisual : public CMenuVisualBase
{
public:
	CMenuHighLightFlashVisual();
	~CMenuHighLightFlashVisual() = default;

	void Update(const bool bSelect, const D3DXVECTOR3& pos, const D3DXVECTOR2& BaseSize, D3DXCOLOR& col, float& fScal) override;
private:
	float m_fCounter; // 点滅のカウンター
};

#endif