//===================================================
//
// 大きさのデータ [size_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef SIZE_COMPONENT_HPP_
#define SIZE_COMPONENT_HPP_

//***************************************************
// 大きさのデータ2D構造体の定義
//***************************************************
struct Size2DComponent
{
	Size2DComponent(const D3DXVECTOR2 _size = Const::VEC2_NULL) : size(_size) {}

	D3DXVECTOR2 size;	// 大きさ
};

//***************************************************
// 大きさのデータ3D構造体の定義
//***************************************************
struct Size3DComponent
{
	Size3DComponent(const D3DXVECTOR3 _size = Const::VEC3_NULL) : size(_size) {}

	D3DXVECTOR3 size; // 大きさ
};

#endif