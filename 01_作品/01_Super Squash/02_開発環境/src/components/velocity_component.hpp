//===================================================
//
// 移動量のデータ [velocity_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _VELOCITY_COMPONENT_HPP_
#define _VELOCITY_COMPONENT_HPP_

//***************************************************
// 移動量コンポーネントの定義
//***************************************************
struct VelocityComponent
{
	VelocityComponent(const float _fInertia = 0.0f, const float _fGravity = Const::MAX_GRABITY, const D3DXVECTOR3 _move = Const::VEC3_NULL) :
		move(_move), fGravity(_fGravity),fInertia(_fInertia), fMoveLength(0.0f) {}

	D3DXVECTOR3 move;	// 移動量
	float fGravity;		// 重力量
	float fInertia;		// 慣性
	float fMoveLength;	// 移動した長さ
};
#endif