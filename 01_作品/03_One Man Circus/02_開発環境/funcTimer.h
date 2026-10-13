//**************************************************
// 
// 機能タイマー
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_TIMER_H_
#define _FANCTION_TIMER_H_

#include "functionBase.h"

// 機能基底クラス
class CFuncTimer : public CFunctionBase
{
public:
	CFuncTimer() {}
	~CFuncTimer() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
	bool m_bLimit;
};

#endif