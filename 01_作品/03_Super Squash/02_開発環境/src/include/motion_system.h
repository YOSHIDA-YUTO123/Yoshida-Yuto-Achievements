//===================================================
//
// モーション更新処理 [motion_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_SYSTEM_H_
#define _MOTION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct MotionComponent;

//***************************************************
// モーションのシステムの処理の取得
//***************************************************
struct MotionSystem : public SystemBase
{
	MotionSystem() = default;
	~MotionSystem() = default;

	void Update(entt::registry & registry) override;

private:
	void UpdateCurrent(entt::registry& registry, const entt::entity model, const int nCntModel, MotionComponent& motionComp);
	void UpdateBlend(entt::registry& registry, const entt::entity model, const int nCntModel, MotionComponent& motionComp);

	bool IsEnd(MotionComponent& motionComp);
	bool IsEndBlend(MotionComponent& motionComp);
	void FinishFirstBlend(MotionComponent& motionComp);
};
#endif