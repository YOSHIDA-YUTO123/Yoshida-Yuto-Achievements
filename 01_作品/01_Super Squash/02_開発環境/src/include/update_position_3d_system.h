//===================================================
//
// 位置の更新処理 [update_position_3d_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UPDATE_POSITION3D_SYSTEM_H_
#define _UPDATE_POSITION3D_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 位置の更新処理3Dの定義
//***************************************************
struct UpdatePosition3DSystem : SystemBase
{
	UpdatePosition3DSystem() = default;
	~UpdatePosition3DSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif