//===================================================
//
// タイマーの更新処理 [update_timer_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UPDAT_TIMER_SYSTEM_H_
#define _UPDAT_TIMER_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct SheetComponent; // シートのコンポーネント

//***************************************************
// タイマーの更新処理
//***************************************************
struct UpdateTimerSystem : SystemBase
{
	UpdateTimerSystem() = default;
	~UpdateTimerSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void SetLastTimer(entt::registry& registry, const entt::entity sheetID);
};
#endif