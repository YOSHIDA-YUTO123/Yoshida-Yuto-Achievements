//================================================
//
// UIの点滅処理 [ui_flash_system.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "ui_flash_system.h"
#include "ui_flash_component.hpp"
#include "color_component.hpp"

//=================================================
// 更新処理
//=================================================
void UIFlashSystem::Update(entt::registry& registry)
{
	// 点滅の取得
	auto view = registry.view<UIFlashComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& colorComp = registry.get<ColorComponent>(entity);
		auto& UIFlashComp = registry.get<UIFlashComponent>(entity);

		if (UIFlashComp.bPlay)
		{
			// 時間を加算
			UIFlashComp.fTime += UIFlashComp.fSpeed;

			// 透明度を設定
			colorComp.col.a = fabsf(sinf(UIFlashComp.fTime) * UIFlashComp.fMaxAlpha);

			UIFlashComp.nFlashTime--;

			// 点滅時間が終わったら
			if (UIFlashComp.nFlashTime <= 0)
			{
				UIFlashComp.bPlay = false;
			}
			continue;
		}

		UIFlashComp.fTime = 0.0f;

		// 透明度を設定
		colorComp.col.a = 1.0f;
	}
}