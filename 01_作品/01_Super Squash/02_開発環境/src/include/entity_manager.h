//===================================================
//
// エンティティのマネージャー [entity_manager.h]
// Author:YUTO YOSHIDA
// 
// << エンティティのソート処理をする >>
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LAYER_MANAGER_H_
#define _LAYER_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"
#include "layer_component.hpp"

//***************************************************
// エンティティのマネージャー名前空間の定義
//***************************************************
namespace EntityManager
{
	template<typename... Type>
	std::vector<entt::entity> GetSortLayerEntity(entt::registry& registry)
	{
		// entity格納用コンテナ
		std::vector<entt::entity> drawList;

		// 型が入っていなかったらコンパイルエラーを出す
		static_assert(sizeof...(Type) > 0, "<<<<<< type not set >>>>>>>>");

		auto view = registry.view<LayerComponent, Type...>();

		// メモリの確保
		drawList.reserve(view.size_hint());

		// LayerComponent分回す
		for (auto entity : view)
		{
			// entityをリストに追加
			drawList.push_back(entity);
		}

		// レイヤー順でソート
		std::sort(drawList.begin(), drawList.end(),
			[&](entt::entity EntityA, entt::entity EntityB)
			{
				return registry.get<LayerComponent>(EntityA).nLayer < registry.get<LayerComponent>(EntityB).nLayer;
			}
		);
		
		return drawList;
	}

	template<typename... Type>
	std::vector<entt::entity> GetSortEntity(entt::registry& registry)
	{
		// entity格納用コンテナ
		std::vector<entt::entity> drawList;

		// 型が入っていなかったらコンパイルエラーを出す
		static_assert(sizeof...(Type) > 0, "<<<<<< type not set >>>>>>>>");

		auto view = registry.view<Type...>();

		// メモリの確保
		drawList.reserve(view.size_hint());

		// LayerComponent分回す
		for (auto entity : view)
		{
			// entityをリストに追加
			drawList.push_back(entity);
		}

		// ID順にソート
		std::sort(drawList.begin(), drawList.end(),
			[&](entt::entity EntityA, entt::entity EntityB)
			{
				return EntityA < EntityB;
			}
		);

		return drawList;
	}
}
#endif