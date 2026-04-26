//===================================================
//
// ロケーターの更新処理(デバッグ用) [locator_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "locator_system.h"
#include "tag_component.hpp"
#include "factory_billboard.h"
#include "transform_component.hpp"
#include "utility_math.h"
#include "color_constants.hpp"
#include "effect_constants.hpp"

//===================================================
// 更新処理
//===================================================
void LocatorSystem::Update(entt::registry& registry)
{
#ifdef _DEBUG
	// ロケーター分回す
	for (auto entity : registry.view<Tag::LocatorTag>())
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		
		// ワールド座標の取得
		D3DXVECTOR3 worldPos = math::GetPositionFromMatrix(transformComp.mtxWorld);

		// エフェクトの生成
		auto effect = FactoryBillboard::Create::Effect(registry, worldPos, Color::WHITE, 5.0f, EffectConstants::TEXTURE_NORMAL);
		FactoryBillboard::Build::Effect(registry, effect, 2, Const::VEC3_NULL);
	}
#else
	(void)registry;
#endif // _DEBUG
}