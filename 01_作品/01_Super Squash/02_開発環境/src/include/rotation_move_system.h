//===================================================
//
// 回転する処理 [rotation_move_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ROTATION_MOVE_SYSTEM_H_
#define _ROTATION_MOVE_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 回転する処理の定義
//***************************************************
struct RotationMoveSystem : SystemBase
{
	RotationMoveSystem() = default;
	~RotationMoveSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif