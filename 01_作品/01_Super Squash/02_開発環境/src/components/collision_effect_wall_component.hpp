//===================================================
//
// エフェクトの壁の当たり判定のデータ [collision_effect_wall_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISION_EFFECT_WALL_COMPONENT_HPP_
#define _COLLISION_EFFECT_WALL_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// エフェクトの壁の当たり判定のデータの定義
//***************************************************
struct CollisionEffectWallComponent
{
	CollisionEffectWallComponent() :
		wallID(entt::null), hitPos(Const::VEC3_NULL), bCollision(false)
	{}

	entt::entity wallID;	// 当たった壁のID
	D3DXVECTOR3 hitPos;		// 当たった位置
	bool bCollision;		// 当たったかどうか
};
#endif