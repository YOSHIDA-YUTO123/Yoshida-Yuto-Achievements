//===================================================
//
//  リザルトのボールの更新処理 [result_ball_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "result_ball_system.h"
#include "result_ball_component.hpp"
#include "velocity_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "ball_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"

//===================================================
// 更新処理
//===================================================
void ResultBallSystem::Update(entt::registry& registry)
{
	// リザルトのボールの取得
	auto view = registry.view<ResultBallComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& resultBallComp = registry.get<ResultBallComponent>(entity);
		auto& wallCollisionComp = registry.get<MeshWallCollisionComponent>(entity);
		auto& velocityComp = registry.get<VelocityComponent>(entity);

		// 着地していたら
		if (wallCollisionComp.bCollision)
		{
			// 反射方向の移動量を取得
			velocityComp.move = math::Bound(velocityComp.move, wallCollisionComp.normal, 1.0f);
		}

		// 寿命を減らす
		resultBallComp.nLife--;

		// 寿命が尽きたら	
		if (resultBallComp.nLife <= 0)
		{
			// 破棄
			registry.destroy(entity);
		}
	}
}