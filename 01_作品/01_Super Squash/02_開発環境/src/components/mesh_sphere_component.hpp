//===================================================
//
// 球体メッシュの頂点のデータ [mesh_sphere_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_SPHERE_COMPONENT_HPP_
#define _MESH_SPHERE_COMPONENT_HPP_

//***************************************************
// 球体メッシュの頂点のデータ構造体の定義
//***************************************************
struct MeshSphereComponent
{
	MeshSphereComponent(const float _fRadius, const float _fHeight, const bool _bReverse = false, const int _nSideStartIndex = 0, const int _nBottomStartIndex = 0) :
		fHeight(_fHeight), fRadius(_fRadius), nSideStartIndex(_nSideStartIndex),
		nBottomStartIndex(_nBottomStartIndex), bReverse(_bReverse)
	{}

	float fRadius;			// 半径
	float fHeight;			// 高さ
	int nSideStartIndex;	// 側面の描画開始インデックス番号
	int nBottomStartIndex;	// 下の蓋の描画開始インデックス番号
	bool bReverse;			// 反転するかどうか
};
#endif