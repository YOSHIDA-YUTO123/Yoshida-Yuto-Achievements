//===================================================
//
// カプセルコライダーのデータ [capsule_collider_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAPSULE_COLLIDER_COMPONENT_HPP_
#define _CAPSULE_COLLIDER_COMPONENT_HPP_

//***************************************************
// カプセルコライダーのコンポネント
//***************************************************
struct CapsuleColliderComponent
{
	CapsuleColliderComponent(const D3DXVECTOR3& _start, const D3DXVECTOR3& _end, const float _fRadius,const entt::entity _ownwerID) :
		ownerID(_ownwerID), startPosLocal(_start), endPosLocal(_end), fRadius(_fRadius), startPos(_start),
		endPos(_end),closestPos(Const::VEC3_NULL)
	{}

	entt::entity ownerID;		// 持ち主のID
	D3DXVECTOR3 startPosLocal; // 開始位置のローカル座標
	D3DXVECTOR3 endPosLocal;	// 終了位置のローカル座標

	D3DXVECTOR3 startPos;		// 開始位置
	D3DXVECTOR3 endPos;			// 終了位置
	D3DXVECTOR3 closestPos;		// 最接近点
	float fRadius;				// 半径
};
#endif