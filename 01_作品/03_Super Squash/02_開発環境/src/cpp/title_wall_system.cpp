//================================================
//
// タイトルの壁の処理 [title_wall_system.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "title_wall_system.h"
#include "tag_component.hpp"
#include "title_wall_component.hpp"
#include "transform_component.hpp"
#include "entity_name_component.hpp"
#include "wall_constants.hpp"
#include "utility_math.h"

//************************************************
// 定数宣言
//************************************************
namespace TitleWallConst
{
	const int MAX_TIME = 600;	// 補間時間
}

//================================================
// 更新処理
//================================================
void TitleWallSystem::Update(entt::registry& registry)
{
	bool bFinishLerp;	 // 壁の補間がおわったかどうか

	// 壁の取得
	auto view = registry.view<Tag::MeshWallTag>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& titleWallComp = registry.get<TitleWallComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& nameComp = registry.get<EntityNameComponent>(entity);

		// 範囲制限
		titleWallComp.nLerpTime = math::Clamp(titleWallComp.nLerpTime, 0, TitleWallConst::MAX_TIME);

		// 補間が終了したら
		if (titleWallComp.nLerpTime == TitleWallConst::MAX_TIME)
		{
			bFinishLerp = true;
		}

		// 割合を求める
		float fRateTime = titleWallComp.nLerpTime / static_cast<float>(TitleWallConst::MAX_TIME);

		// 補間時間を加算
		titleWallComp.nLerpTime++;

		// 目的の位置に近づける
		transformComp.pos += (titleWallComp.posDest - transformComp.pos) * fRateTime;

		// 天井じゃないなら
		if (nameComp.name != WallConst::TOP_WALL)
		{
			// 目的の向きに近づける
			D3DXQuaternionSlerp(&transformComp.quaternion, &transformComp.quaternion, &titleWallComp.rotDest, fRateTime);
		}
	}
}