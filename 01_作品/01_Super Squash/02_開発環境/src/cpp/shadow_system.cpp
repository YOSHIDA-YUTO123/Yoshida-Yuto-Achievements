//===================================================
//
// 影の更新処理 [shadow_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadow_system.h"
#include "transform_component.hpp"
#include "tag_component.hpp"
#include "size_component.hpp"
#include "shadow_component.hpp"
#include "color_component.hpp"
#include "utility_math.h"

//===================================================
// 影の更新処理
//===================================================
void ShadowSystem::Update(entt::registry& registry)
{
	// 影のコンポーネントの取得
	auto view = registry.view<ShadowComponent>();

	// 影の分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& shadowComp = registry.get<ShadowComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& sizeComp = registry.get<Size3DComponent>(entity);
		auto& colorComp = registry.get<ColorComponent>(entity);

		// オーナーのIDが無いなら処理を飛ばす
		if (shadowComp.ownerID == entt::null)
		{
			continue;
		}

		// オーナーの位置の取得
		auto& ownerTransform = registry.get<Transform3DComponent>(shadowComp.ownerID);

		// 高さの割合
		float fRate = (ownerTransform.pos.y / shadowComp.fMaxHeight);

		// 範囲内に収める
		fRate = math::Clamp(fRate, 0.0f, 1.0f);

		colorComp.col.a = 1.0f - fRate;

		// 割合の半径を求める
		float fRateRadius = shadowComp.fRadiusBase * fRate;

		// 大きさを設定
		sizeComp.size = D3DXVECTOR3(shadowComp.fRadiusBase + fRateRadius, 0.0f, shadowComp.fRadiusBase + fRateRadius);

		// 位置の設定
		transformComp.pos.x = ownerTransform.pos.x;
		transformComp.pos.y = 1.0f;
		transformComp.pos.z = ownerTransform.pos.z;
	}
}