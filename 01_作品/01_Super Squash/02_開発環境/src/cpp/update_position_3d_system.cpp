//===================================================
//
// 位置の更新処理 [update_position_3d_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_position_3d_system.h"
#include "transform_component.hpp"
#include "velocity_component.hpp"
#include "game_controller.h"
#include "hitstop_controller.h"
#include "tag_component.hpp"

//===================================================
// 更新処理
//===================================================
void UpdatePosition3DSystem::Update(entt::registry& registry)
{
	auto view = registry.view<VelocityComponent>();

	// コンポーネント分回す
	for (auto entity : view)
	{
		// ゲームを操作するクラスの取得
		auto pGameController = CGameController::GetInstance();

		// ヒットストップしているときにHitStop対象のentityだったら
		if (pGameController != nullptr && registry.any_of<Tag::HitStopTag>(entity))
		{
			// ヒットストップしているなら処理を飛ばす
			if (pGameController->GetHitStop()->IsStop())
			{
				return;
			}
		}

		// 移動量の取得
		auto& velocityComp = registry.get<VelocityComponent>(entity);

		// 空間情報の取得の取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// 移動量の減衰処理
		velocityComp.move.x += (0.0f - velocityComp.move.x) * velocityComp.fInertia;
		velocityComp.move.z += (0.0f - velocityComp.move.z) * velocityComp.fInertia;

		// 前のフレームの位置の取得
		transformComp.posOld = transformComp.pos;

		// 重力を設定
		velocityComp.move.y += -velocityComp.fGravity;

		// 移動量の更新処理
		transformComp.pos += velocityComp.move;

		// 移動した分の長さを求める
		D3DXVECTOR3 vecMove = transformComp.posOld - transformComp.pos;

		// 移動した長さを設定
		velocityComp.fMoveLength = D3DXVec3Length(&vecMove);
	}
}
