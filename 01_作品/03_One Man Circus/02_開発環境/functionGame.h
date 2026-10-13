//**************************************************
// 
// 機能ルール管理処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTIONGAME_H_
#define _FANCTIONGAME_H_

#include "functionBase.h"

// 機能ルール管理処理クラス
class CFuncRulu : public CFunctionBase
{
public:
	CFuncRulu() {}
	~CFuncRulu() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

// 機能プレイヤーステート管理処理クラス
class CFuncPlayerState : public CFunctionBase
{
public:
	CFuncPlayerState() {}
	~CFuncPlayerState() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif
