//===================================================
//
// プレイヤーのモーションに応じた処理 [player_motion_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_MOTION_SYSTEM_H_
#define _PLAYER_MOTION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// プレイヤーのモーションのシステムの定義
//***************************************************
struct PlayerMotionSystem : SystemBase
{
	PlayerMotionSystem() = default;
	~PlayerMotionSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void SetChargeEffect(entt::registry& registry, const entt::entity player);
	void SetMoveEffect(entt::registry& registry, const entt::entity player);
};
#endif