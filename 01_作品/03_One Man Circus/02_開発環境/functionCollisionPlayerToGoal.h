//**************************************************
// 
// 機能プレイヤーとゴールの当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_COLLISION_PLAYER_TO_GOAL_H_
#define _FANCTION_COLLISION_PLAYER_TO_GOAL_H_

#include "functionBase.h"

// 機能プレイヤーとゴールの当たり判定クラス
class CFuncCollPlayerToGoal : public CFunctionBase
{
public:
	CFuncCollPlayerToGoal() {}
	~CFuncCollPlayerToGoal() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif
