//===================================================
//
// メッシュウォールの当たり判定のデータ [mesh_wall_collision_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WALL_COLLISION_COMPONENT_HPP_
#define _MESH_WALL_COLLISION_COMPONENT_HPP_

//***************************************************
// メッシュウォールの当たり判定のデータの取得
//***************************************************
struct MeshWallCollisionComponent
{
	MeshWallCollisionComponent(const entt::entity _entity) :
		entity(_entity), meshID(entt::null), normal(Const::VEC3_NULL),
		hitPos(Const::VEC3_NULL), bCollision(false) {}

	entt::entity entity;	// 当たり判定をするエンティティID
	entt::entity meshID;	// 当たったメッシュのエンティティID
	D3DXVECTOR3 normal;		// 当たっている面の法線
	D3DXVECTOR3 hitPos;		// 衝突時の座標
	bool bCollision;		// 当たっていたら
};
#endif