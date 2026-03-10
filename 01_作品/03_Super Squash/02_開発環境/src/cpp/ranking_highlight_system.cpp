//===================================================
//
// ランキングのハイライト表示の更新処理の定義 [ranking_highlight_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ranking_highlight_system.h"
#include "ranking_highlight_component.hpp"
#include "score_component.hpp"
#include "color_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace RankingConst
{
	constexpr float FLASH_SPEED = 0.01f;	// 点滅速度
}

//===================================================
// 更新処理
//===================================================
void RankingHighLightSystem::Update(entt::registry& registry)
{
	// ハイライトの取得
	auto view = registry.view<RankingHighLightComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& highLightComp = registry.get<RankingHighLightComponent>(entity);
		auto& scoreComp = registry.get<ScoreComponent>(entity);

		// ナンバー分回す
		for (const auto& numberID : scoreComp.aNumberID)
		{
			// 色の取得
			auto& colorComp = registry.get<ColorComponent>(numberID);

			highLightComp.fTime += RankingConst::FLASH_SPEED;

			colorComp.col.g = fabsf(sinf(highLightComp.fTime));
			colorComp.col.b = fabsf(sinf(highLightComp.fTime));
		}
	}
}