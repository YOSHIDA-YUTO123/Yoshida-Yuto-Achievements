//===================================================
//
// ボールと地面の当たり判定 [ball_to_field_collision_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_TO_FIELD_COLLISION_SYSTEM_H_
#define _BALL_TO_FIELD_COLLISION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// ボールと地面の当たり判定の処理の定義
//***************************************************
struct BallToCollisionSystem : SystemBase
{
	BallToCollisionSystem() = default;
	~BallToCollisionSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif