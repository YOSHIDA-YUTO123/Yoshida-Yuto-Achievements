//===================================================
//
// 回転する処理 [rotation_move_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "rotation_move_system.h"
#include "transform_component.hpp"
#include "rotation_move_component.hpp"

//===================================================
// 更新処理
//===================================================
void RotationMoveSystem::Update(entt::registry& registry)
{
	// 回転の取得
	auto view = registry.view<RotationMoveComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& rotationMoveComp = registry.get<RotationMoveComponent>(entity);
		
		transformComp.quaternion *= rotationMoveComp.speed;
	}
}