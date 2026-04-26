//===================================================
//
// キャラクターの更新処理 [character_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character_system.h"
#include "character_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"
#include "tag_component.hpp"
#include "game_controller.h"
#include "hitstop_controller.h"

//===================================================
// 更新処理
//===================================================
void CharacterSystem::Update(entt::registry& registry)
{
	auto view = registry.view<Tag::CharacterTag>();

	// キャラクターのコンポーネント分回す
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

		// コンポーネントを取得
		auto& characterComp = registry.get<CharacterComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& shadowTransform = registry.get<Transform3DComponent>(characterComp.shadowID);

		D3DXQUATERNION qDestRot;

		// 目的の向きを求める
		D3DXQuaternionRotationAxis(&qDestRot, &Const::AXIS_Y, characterComp.fRotDest);

		// 補間する
		D3DXQuaternionSlerp(&transformComp.quaternion, &transformComp.quaternion, &qDestRot, characterComp.fRotSpeed);
		D3DXQuaternionNormalize(&transformComp.quaternion, &transformComp.quaternion);

		// 影の位置を設定
		shadowTransform.pos = transformComp.pos;
	}
}
