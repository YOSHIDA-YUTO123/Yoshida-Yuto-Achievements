//**************************************************
// 
// 機能プレイヤーとギミックの当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_COLLISION_PLAYER_TO_GIMMICK_H_
#define _FANCTION_COLLISION_PLAYER_TO_GIMMICK_H_

#include "functionBase.h"

// 前方宣言
class CPlayer; // プレイヤークラス

// 機能基底クラス
class CFuncCollPlayerToGimmick : public CFunctionBase
{
public:
	CFuncCollPlayerToGimmick() {}
	~CFuncCollPlayerToGimmick() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif