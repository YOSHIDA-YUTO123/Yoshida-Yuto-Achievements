//===================================================
//
// フィールドの当たり判定 [field_collision_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FIELD_COLLISION_SYSTEM_H_
#define _FIELD_COLLISION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// フィールドの当たり判定の処理の定義
//***************************************************
struct FieldCollisionSystem : SystemBase
{
	FieldCollisionSystem() = default;
	~FieldCollisionSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif