//===================================================
//
// オープニングのUIの処理 [Opening_UI.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OPENING_UI_H_
#define _OPENING_UI_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object2D.h"

//***************************************************
// オープニングのUI
//***************************************************
class COpening_UI : public CObject2D
{
public:

	COpening_UI();
	~COpening_UI();

	static COpening_UI* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& Size, const char* pTexturePath);

	void SetShow(const bool bShow) { m_bShow = bShow; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	D3DXCOLOR m_col;		// 色
	float m_fFlashCounter;	// 点滅カウンター
	bool m_bShow;			// 表示するかどうか
};
#endif