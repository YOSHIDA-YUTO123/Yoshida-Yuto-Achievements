//===================================================
//
// ボールとフィールドの当たり判定のデータ [ball_field_collision_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_FIELD_COLLISION_COMPONENT_HPP_
#define _BALL_FIELD_COLLISION_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// ボールとフィールドの当たり判定のデータの取得
//***************************************************
struct BallFieldCollisionComponent
{
	BallFieldCollisionComponent(const entt::entity _entity) :
		entity(_entity), meshID(entt::null), normal(Const::VEC3_NULL), bLanding(false) {}

	entt::entity entity;	// 当たり判定をするエンティティID
	entt::entity meshID;	// 乗ってるメッシュのエンティティID
	D3DXVECTOR3 normal;		// 乗っている地面の法線
	bool bLanding;			// 乗ったかどうか
};
#endif