//===================================================
//
// ボールと地面の当たり判定 [ball_to_field_collision_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ball_to_field_collision_system.h"
#include "ball_component.hpp"
#include "tag_component.hpp"
#include "mesh_vtx_component.hpp"
#include "size_component.hpp"
#include "transform_component.hpp"
#include "vertex_buffer_component.hpp"
#include "field_collision_component.hpp"
#include "sphere_component.hpp"
#include "ball_field_collision_component.hpp"
#include "debug_proc.h"
#include "factory_billboard.h"
#include "utility_math.h"

//===================================================
// 更新処理
//===================================================
void BallToCollisionSystem::Update(entt::registry& registry)
{
	// 地面の当たり判定のコンポーネントを持つエンティティの取得
	auto CollisionEntity = registry.view<BallComponent>();

	// 地面のコンポーネントを持つエンティティの取得(front())
	auto fieldEntity = registry.view<Tag::MeshFieldTag>().front();

	// 当たり判定
	for (auto ballEntity : CollisionEntity)
	{
		// コンポーネントの取得
		auto& balltransformComp = registry.get<Transform3DComponent>(ballEntity);
		auto& ballComp = registry.get<BallFieldCollisionComponent>(ballEntity);
		auto& sphereColliderComp = registry.get<SphereComponent>(ballEntity);

		// 着地したら
		if ((balltransformComp.pos.y - sphereColliderComp.fRadius) <= 0.0f)
		{
			ballComp.bLanding = true;
			ballComp.entity = fieldEntity;
			ballComp.normal = Const::VEC_U;

			// 押し戻す
			balltransformComp.pos.y = sphereColliderComp.fRadius;
		}
	}
}