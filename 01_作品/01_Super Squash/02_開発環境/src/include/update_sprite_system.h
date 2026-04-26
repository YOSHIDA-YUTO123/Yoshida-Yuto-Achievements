//===================================================
//
// 2Dの更新の処理 [update_sprite_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UPDATE_SPRITE_SYSTEM_H_
#define _UPDATE_SPRITE_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 2Dの更新処理
//***************************************************
struct UpdateSpriteSystem : SystemBase
{
	UpdateSpriteSystem() = default;
	~UpdateSpriteSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif