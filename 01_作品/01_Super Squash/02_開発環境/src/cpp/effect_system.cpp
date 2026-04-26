//===================================================
//
// エフェクトの更新処理 [effect_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "effect_system.h"
#include "tag_component.hpp"
#include "effect_component.hpp"
#include "vector"
#include "vertex_buffer_component.hpp"
#include "size_component.hpp"
#include "manager.h"
#include "color_component.hpp"

//===================================================
// 更新処理
//===================================================
void EffectSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::EffectTag>();

	// エフェクト分回す
	for (auto entity : view)
	{
		auto& sizeComp = registry.get<Size2DComponent>(entity);
		auto& effectComp = registry.get<EffectComponent>(entity);
		auto& colorComp = registry.get<ColorComponent>(entity);

		// 半径の減少
		if (effectComp.nFlag & EffectComponent::FLAG_DEC_RADIUS)
		{
			// 半径を減らす
			sizeComp.size.x -= effectComp.fDecRadius;
			sizeComp.size.y -= effectComp.fDecRadius;
		}

		// αの減少
		if (effectComp.nFlag & EffectComponent::FLAG_DEC_ALPHA)
		{
			colorComp.col.a -= effectComp.fDecAlphaLv;
		}

		effectComp.nLife--;

		// 寿命が尽きたら
		if (registry.valid(entity) && effectComp.nLife <= 0)
		{
			registry.destroy(entity);
		}
	}
}
