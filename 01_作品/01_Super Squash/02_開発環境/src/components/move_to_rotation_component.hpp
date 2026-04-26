//===================================================
//
// 移動方向に回転するの処理のデータ [move_to_rotation_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOVE_TO_ROTATION_COMPONENT_HPP_
#define _MOVE_TO_ROTATION_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 移動方向に回転する処理のデータの定義
//***************************************************
struct MoveToRotationComponent
{
	MoveToRotationComponent(const float _fCircumference) : 
		axis(Const::VEC3_NULL),fCircumference(_fCircumference),fValueRot(0.0f)
	{}

	D3DXVECTOR3 axis;		// 回転軸
	float fCircumference;	// 一周の大きさ
	float fValueRot;		// 回転量
};
#endif