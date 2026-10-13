//**************************************************
// 
// 機能プレイヤーとドラム缶の当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_COLLISION_PLAYER_TO_DRUMCAN_H_
#define _FANCTION_COLLISION_PLAYER_TO_DRUMCAN_H_

#include "functionBase.h"

// 機能基底クラス
class CFuncCollPlayerToDrumcan : public CFunctionBase
{
public:
	CFuncCollPlayerToDrumcan() {}
	~CFuncCollPlayerToDrumcan() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif