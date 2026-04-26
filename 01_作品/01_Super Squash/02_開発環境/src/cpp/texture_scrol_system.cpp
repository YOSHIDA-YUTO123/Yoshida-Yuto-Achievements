//===================================================
//
// テクスチャのスクロール処理 [TextureScrolSystem.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "texture_scrol_system.h"
#include "texture_scrol_component.hpp"
#include "sheet_component.hpp"

//===================================================
// 更新処理
//===================================================
void TextureScrolSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントをもつエンティティの取得
	auto view = registry.view<TextureScrolComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& scrolComp = registry.get<TextureScrolComponent>(entity);
		auto& sheetComp = registry.get<SheetComponent>(entity);

		// スクロールさせる
		sheetComp.aPosUV[0] += scrolComp.speed;
		sheetComp.aPosUV[1] += scrolComp.speed;
		sheetComp.aPosUV[2] += scrolComp.speed;
		sheetComp.aPosUV[3] += scrolComp.speed;
	}
}