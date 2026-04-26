//===================================================
//
// 回転する処理のデータ [rotation_move_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ROTATION_MOVE_COMPONENT_HPP_
#define _ROTATION_MOVE_COMPONENT_HPP_

//***************************************************
// 回転する処理のデータの定義
//***************************************************
struct RotationMoveComponent
{
	RotationMoveComponent(const D3DXVECTOR3& _speed) : 
		speed(Const::QUATERNION_IDENTITY)
	{
		D3DXQuaternionRotationYawPitchRoll(&speed, _speed.y, _speed.x, _speed.z);
	}

	D3DXQUATERNION speed; // 速さ
};
#endif