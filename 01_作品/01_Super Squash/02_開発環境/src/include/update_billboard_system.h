//===================================================
//
// ビルボードの更新の処理 [update_billboard_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UPDATE_BILLBOARD_SYSTEM_H_
#define _UPDATE_BILLBOARD_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// ビルボードの更新処理
//***************************************************
struct UpdateBillboardSystem : SystemBase
{
	UpdateBillboardSystem() = default;
	~UpdateBillboardSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif