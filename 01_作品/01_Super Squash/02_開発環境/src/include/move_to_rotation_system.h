//===================================================
//
// 移動方向に回転するの処理 [move_to_rotation_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOVE_TO_ROTATION_H_
#define _MOVE_TO_ROTATION_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 移動方向に回転する処理の定義
//***************************************************
struct MoveToRotationSystem : SystemBase
{
	MoveToRotationSystem() = default;
	~MoveToRotationSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif