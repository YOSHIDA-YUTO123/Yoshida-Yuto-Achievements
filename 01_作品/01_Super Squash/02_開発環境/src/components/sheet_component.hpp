//===================================================
//
// 分割テクスチャポリゴンのデータ [sheet_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef SPRITE_SHEET_COMPONENT_HPP_
#define SPRITE_SHEET_COMPONENT_HPP_

//***************************************************
// 分割テクスチャポリゴンのデータの定義
//***************************************************
struct SheetComponent
{
	// UV座標の数
	static constexpr int NUM_UV			= 4;		// UVの数(4頂点分)
	static constexpr float UV_POS_MAX	= 1.0f;		// UV座標のMAX
	static constexpr float UV_POS_MIN	= 0.0f;		// UV座標のMIN

	SheetComponent(const int _nSegmentU, const int _nSegmentV) :
		nSegmentU(_nSegmentU), nSegmentV(_nSegmentV),aPosUV()
	{
		// テクスチャ座標の設定
		aPosUV[0] = { UV_POS_MIN,UV_POS_MIN };
		aPosUV[1] = { UV_POS_MAX / _nSegmentU,UV_POS_MIN };
		aPosUV[2] = { UV_POS_MIN,UV_POS_MAX / _nSegmentV };
		aPosUV[3] = { UV_POS_MAX / _nSegmentU,UV_POS_MAX / _nSegmentV };
	}

	int nSegmentU;	// 分割数横
	int nSegmentV;	// 分割数縦

	std::array<D3DXVECTOR2, NUM_UV> aPosUV; // テクスチャ座標
};
#endif