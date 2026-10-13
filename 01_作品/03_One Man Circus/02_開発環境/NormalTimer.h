//================================================
//
// 普通のタイマーを表示する処理 [NormalTimer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _NORMALTIMER_H_
#define _NORMALTIMER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "timer.h"

//*************************************************
// 普通のタイマークラスの定義
//*************************************************
class CNormalTimer : public CTimer
{
public:
	CNormalTimer();
	~CNormalTimer();

	static CNormalTimer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nTime);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	int m_nCounter;	// カウンター
};

#endif
