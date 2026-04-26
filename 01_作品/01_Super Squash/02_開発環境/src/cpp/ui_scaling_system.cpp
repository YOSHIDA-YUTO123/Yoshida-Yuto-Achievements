//===================================================
//
// UIの拡大縮小アニメーションの更新処理 [ui_scaling_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ui_scaling_system.h"
#include "ui_scaling_component.hpp"
#include "size_component.hpp"

//===================================================
// 更新処理
//===================================================
void UIScalingSystem::Update(entt::registry& registry)
{
	// コンポーネントの取得
	auto view = registry.view<UIScalingComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& UIScalingComp = registry.get<UIScalingComponent>(entity);
		auto& sizeComp = registry.get<Size2DComponent>(entity);

		// 再生できるなら
		if (UIScalingComp.bPlay)
		{
			// 拡大した大きさを求める
			D3DXVECTOR2 destSize = UIScalingComp.baseSize * UIScalingComp.fScal;

			// カウンターを加算
			UIScalingComp.fCounter += UIScalingComp.fSpeed;

			// サインカーブ
			float sin = fabsf(sinf(UIScalingComp.fCounter));

			// 大きさを設定
			sizeComp.size = (destSize * sin) + UIScalingComp.baseSize;
		}
		else
		{
			UIScalingComp.fCounter = 0;

			// 元の大きさに段々戻す
			sizeComp.size += (UIScalingComp.baseSize - sizeComp.size) * UIScalingComp.fScalCoef;
		}
	}
}