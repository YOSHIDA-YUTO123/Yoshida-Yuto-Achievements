//===================================================
//
// 円のコライダーのデータ [sphere_collider_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SPHERE_COLLIDER_COMPONENT_HPP_
#define _SPHERE_COLLIDER_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 円のコライダーのデータ構造体の定義
//***************************************************
struct SphereColliderComponent
{
	SphereColliderComponent(const float _fRadius, const entt::entity _owner) :
		fRadius(_fRadius), ownerID(_owner) {}

	entt::entity ownerID;	// オーナーのID
	float fRadius;			// 半径
};
#endif