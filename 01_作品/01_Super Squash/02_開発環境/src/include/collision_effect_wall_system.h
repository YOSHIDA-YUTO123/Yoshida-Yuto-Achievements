//===================================================
//
// エフェクトの壁の当たり判定の処理 [collision_effect_wall_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISION_EFFECT_WALL_SYSTEM_H_
#define _COLLISION_EFFECT_WALL_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// エフェクトの壁の当たり判定の処理の定義
//***************************************************
struct CollisionEffectWallSystem : SystemBase
{
	CollisionEffectWallSystem() = default;
	~CollisionEffectWallSystem() = default;

	void Update(entt::registry& registry) override;
private:
	bool Collision(entt::registry& registry, const entt::entity wall);
};
#endif