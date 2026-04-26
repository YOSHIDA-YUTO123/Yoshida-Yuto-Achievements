//===================================================
//
// フェードアウトするUIの更新処理 [ui_fadeout_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ui_fadeout_system.h"
#include "ui_fadeout_component.hpp"
#include "color_component.hpp"

//===================================================
// 更新処理
//===================================================
void UIFadeOutSystem::Update(entt::registry& registry)
{
	// フェードアウトの取得
	auto view = registry.view<UIFadeOutComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& fadeOutComp = registry.get<UIFadeOutComponent>(entity);
		auto& colorComp = registry.get<ColorComponent>(entity);

		if (!fadeOutComp.bPlay || fadeOutComp.nTime == 0)
		{
			continue;
		}

		// タイムの割合を求める
		float fRateTime = fadeOutComp.nCount / static_cast<float>(fadeOutComp.nTime);

		// アルファ値を設定
		colorComp.col.a = 1.0f - fRateTime;

		fadeOutComp.nCount++;

		// 見えなくなったら消す
		if (colorComp.col.a <= 0.0f)
		{
			registry.destroy(entity);
		}
	}
}