//================================================
//
// 制限時間目前で表示するタイマー[LastTimer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _LAST_TIMER_H_
#define _LAST_TIMER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "object.h"

//*************************************************
// 前方宣言
//*************************************************
class CNormalTimer; // タイマー

//*************************************************
// 制限時間目前で表示するタイマーのクラス
//*************************************************
class CLastTimer : public CObject
{
public:
	CLastTimer();
	~CLastTimer();

	static CLastTimer* Create(const D3DXVECTOR2& Size, const int nTime);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	CNormalTimer* m_pTimer; // タイマー
	D3DXCOLOR m_col;		// 色
};
#endif