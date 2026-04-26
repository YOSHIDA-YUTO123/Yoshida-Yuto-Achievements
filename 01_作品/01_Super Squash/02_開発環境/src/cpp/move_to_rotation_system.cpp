//===================================================
//
// 移動方向に回転するの処理 [move_to_rotation_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "move_to_rotation_system.h"
#include "move_to_rotation_component.hpp"
#include "transform_component.hpp"
#include "velocity_component.hpp"
#include "utility_math.h"
#include "game_controller.h"
#include "hitstop_controller.h"

//===================================================
// 更新処理
//===================================================
void MoveToRotationSystem::Update(entt::registry& registry)
{
	// ゲームを操作するクラスの取得
	auto pGameController = CGameController::GetInstance();

	if (pGameController != nullptr)
	{
		// ヒットストップしているなら処理を飛ばす
		if (pGameController->GetHitStop()->IsStop())
		{
			return;
		}
	}

	// 移動方向に回転するコンポーネントを持つエンティティの取得
	auto view = registry.view<MoveToRotationComponent>();

	// 移動方向に回転するコンポーネントを持つエンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& velocityComp = registry.get<VelocityComponent>(entity);
		auto& moveToRotComp = registry.get<MoveToRotationComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// 移動量を正規化するため別の変数に格納
		D3DXVECTOR3 vecMove = velocityComp.move;

		D3DXVec3Normalize(&vecMove, &vecMove);

		// 移動が無いなら
		if (D3DXVec3Length(&vecMove) == 0.0f)
		{
			return;
		}

		// 回転軸の作成
		moveToRotComp.axis = math::Cross(Const::VEC_U, vecMove);

		// クォータニオンが初期状態だったら
		if (transformComp.quaternion.x == 0.0f && transformComp.quaternion.y == 0.0f && transformComp.quaternion.z == 0.0f)
		{
			// 回転軸における指定の回転角からクォータニオンを作成
			D3DXQuaternionRotationAxis(&transformComp.quaternion, &moveToRotComp.axis, moveToRotComp.fValueRot);
		}
		else
		{
			// 計算用クォータニオン
			D3DXQUATERNION quat, quatNew;

			// 回転軸における指定の回転角からクォータニオンを作成
			D3DXQuaternionRotationAxis(&quat, &moveToRotComp.axis, moveToRotComp.fValueRot);

			// 前の姿勢と今の姿勢の合成
			D3DXQuaternionMultiply(&quatNew, &transformComp.quaternion, &quat);

			// 保存
			transformComp.quaternion = quatNew;
		}

		// 移動量から回転量を求める
		moveToRotComp.fValueRot = (velocityComp.fMoveLength / moveToRotComp.fCircumference) * Const::TWO_PI;
	}
}