//===================================================
//
// メッシュの頂点のデータ [mesh_vtx_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_VTX_COMPONENT_HPP_
#define _MESH_VTX_COMPONENT_HPP_

//***************************************************
// メッシュの頂点のデータ構造体の定義
//***************************************************
struct MeshVtxComponent
{
	static constexpr int MIN_SEGMENT = 1; // 最小の分割数

	MeshVtxComponent(const int _nSegmentU = MIN_SEGMENT, const int _nSegmentV = MIN_SEGMENT, const D3DXCOLOR& _col = Const::WHITE) :
		nSegmentU(_nSegmentU), nSegmentV(_nSegmentV), col(_col),
		nNumVertex(0), nNumPolygon(0),
		nNumIndex(0) {}

	D3DXCOLOR col;		// 色
	int nSegmentU;		// 縦の分割数
	int nSegmentV;		// 横の分割数
	
	int nNumVertex;		// 頂点の数
	int nNumPolygon;	// ポリゴンの数
	int nNumIndex;		// インデックスの数
};
#endif