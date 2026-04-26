//===================================================
//
// 空間情報のデータ [transform_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TRANSFORM_COMPONENT_HPP_
#define _TRANSFORM_COMPONENT_HPP_

//***************************************************
// 3D空間情報コンポーネントの定義
//***************************************************
struct Transform3DComponent
{
	Transform3DComponent(const D3DXVECTOR3& _pos = Const::VEC3_NULL, const D3DXVECTOR3& _rot = Const::VEC3_NULL, const D3DXVECTOR3& _Scal =Const::INIT_SCAL) :
		pos(_pos), posOld(_pos), scal(_Scal), mtxWorld(Const::MTX_IDENTITY)
	{
		// クォータニオンの作成
		D3DXQuaternionRotationYawPitchRoll(&quaternion, _rot.y, _rot.x, _rot.z);
	}

	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 scal;			// 大きさ
	D3DXQUATERNION quaternion;  // 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
};

//***************************************************
// 2D空間情報コンポーネントの定義
//***************************************************
struct Transform2DComponent
{
	Transform2DComponent(const D3DXVECTOR2 _pos = Const::VEC2_NULL, const D3DXVECTOR3 _rot = Const::VEC3_NULL) :
		pos(_pos), rot(_rot) {}

	D3DXVECTOR2 pos; // 位置
	D3DXVECTOR3 rot; // 向き
};

#endif