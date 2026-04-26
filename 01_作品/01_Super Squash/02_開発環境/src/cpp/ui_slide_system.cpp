//===================================================
//
// UIのスライドイン、アウトのアニメーションの処理 [ui_slide_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ui_slide_system.h"
#include "ui_slide_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"

//===================================================
// 更新処理
//===================================================
void UISlideSystem::Update(entt::registry& registry)
{
	// UIのスライドコンポーネントの取得
	auto view = registry.view<UISlideComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& slideComp = registry.get<UISlideComponent>(entity);
		auto& transformComp = registry.get<Transform2DComponent>(entity);

		// 範囲制限
		slideComp.nCounter = math::Clamp(slideComp.nCounter, 0, slideComp.nTime);

		// スライド時間の割合を求める
		float fRate = slideComp.nCounter / static_cast<float>(slideComp.nTime);

		slideComp.nCounter++;

		// スライドアウトするなら
		if (slideComp.bOut)
		{
			// 目的の位置に近づける
			transformComp.pos = slideComp.destPosIn + (slideComp.destPosOut - slideComp.destPosIn) * fRate;
		}
		else
		{
			// 目的の位置に近づける
			transformComp.pos = slideComp.firstPos + (slideComp.destPosIn - slideComp.firstPos) * fRate;
		}
	}
}