//===================================================
//
// コライダーの処理 [collider_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "collider_system.h"
#include "transform_component.hpp"
#include "factory_billboard.h"
#include "line.h"
#include "color_constants.hpp"
#include "utility_math.h"
#include "box_collider_component.hpp"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"

#include "input.h"
#include "manager.h"

//===================================================
// 更新処理
//===================================================
void ColliderSystem::Update(entt::registry& registry)
{
#ifdef _DEBUG
	static bool bShow = true; // コライダーを表示するか

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	if (pKeyboard != nullptr && pKeyboard->GetTrigger(DIK_F3))
	{
		bShow = bShow ? false : true;
	}
#endif // _DEBUG

	// カプセルコライダーの取得
	auto capsuleView = registry.view<CapsuleColliderComponent>();

	// カプセルコライダー分回す
	for (auto capsule : capsuleView)
	{
		// コンポーネントの取得
		auto& capsuleComp = registry.get<CapsuleColliderComponent>(capsule);
		auto& transformComp = registry.get<Transform3DComponent>(capsuleComp.ownerID);

		// 始点,終点をローカル座標からワールド座標に変換する
		D3DXVec3TransformCoord(&capsuleComp.startPos, &capsuleComp.startPosLocal, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&capsuleComp.endPos, &capsuleComp.endPosLocal, &transformComp.mtxWorld);
	}

	// 矩形のコライダーの取得
	auto aabbView = registry.view<BoxColliderComponent>();

	// 矩形コライダー分回す
	for (auto aabb : aabbView)
	{
		// コンポーネントの取得
		auto& aabbComp = registry.get<BoxColliderComponent>(aabb);

		// オーナーが設定されていないなら処理を飛ばす
		if (!registry.valid(aabbComp.ownerID)) continue;

		auto& ownerTransform = registry.get<Transform3DComponent>(aabbComp.ownerID);

		// 世界基準の中心座標を求める
		D3DXVec3TransformCoord(&aabbComp.centerPos, &aabbComp.centerPosLocal, &ownerTransform.mtxWorld);
	}
}
