//===================================================
//
// スプライトシートの更新処理 [update_sprite_sheet_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UPDATE_SPRITE_SHEET_SYSTEM_H_
#define _UPDATE_SPRITE_SHEET_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// スプライトシートの更新処理
//***************************************************
struct UpdateSpriteSheetSystem : SystemBase
{
	UpdateSpriteSheetSystem() = default;
	~UpdateSpriteSheetSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif