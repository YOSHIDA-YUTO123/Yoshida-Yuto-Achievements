//===================================================
//
// シリンダーのデータ [mesh_cylinder_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_CYLINDER_COMPONENT_HPP_
#define _MESH_CYLINDER_COMPONENT_HPP_

//***************************************************
// シリンダーのデータの定義
//***************************************************
struct MeshCylinderComponent
{
	MeshCylinderComponent(const float _fHeight = 0.0f) : fHeight(_fHeight) {}

	float fHeight; // 高さ
};
#endif