//===================================================
//
// キャラクターの位置、向きの条件クラス [character_transform_spec.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character_transform_spec.h"
#include "transform_component.hpp"
#include "utility_math.h"
#include "tag_component.hpp"
#include "collision.h"

//***************************************************
// 定数宣言
//***************************************************
namespace SpecTransform_Const
{
	constexpr float FOV_LENGTH = 500.0f;	// 視界の長さ
}

//===================================================
// ボールがキャラクターの左にいるかどうか判定
//===================================================
bool CharacterSpec::Transform::CBallOnLeftSide::IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const
{
	// 近いボール
	entt::entity nearBall = entt::null;

	// 最小の距離
	float fDistanceMin = FLT_MAX;

	// 空間情報の取得
	auto& characterTransform = registry.get<Transform3DComponent>(character);

	// ボールの数分回す
	for (auto ball : registry.view<Tag::BallTag>())
	{
		// ボールの空間情報の取得
		auto& ballTransfrom = registry.get<Transform3DComponent>(ball);
		float fDistance = math::GetDistance(ballTransfrom.pos - characterTransform.pos);

		// 最小の距離より短いなら
		if (fDistance < fDistanceMin)
		{
			// 最小距離を更新
			fDistanceMin = fDistance;
			nearBall = ball;
		}
	}

	// 近いボールがいないなら処理しない
	if (nearBall == entt::null) return false;

	// 近いボールの位置の取得
	auto& nearBallTransfrom = registry.get<Transform3DComponent>(nearBall);

	// 左側にいるなら
	if (Collision::FOV::Collision(nearBallTransfrom.pos, characterTransform, -D3DX_PI * 0.5f, 0.0f, SpecTransform_Const::FOV_LENGTH))
	{
		return true;
	}
	return false;
}
