//===================================================
//
// マルチテクスチャのデータ [texture_mt_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_MT_COMPONENT_HPP_
#define _TEXTURE_MT_COMPONENT_HPP_

//***************************************************
// マルチテクスチャのデータの定義
//***************************************************
struct TextureMTComponent
{
	static constexpr int MAX_TEXTURE = 2;	// テクスチャの最大数
	static constexpr int INVALID = -1;		// 無効なテクスチャのID

	TextureMTComponent(const int _nTextureID_1 = INVALID, const int _nTextureID_2 = INVALID) :
		textureMT(Const::VEC2_NULL), aTextureID()
	{
		aTextureID[0] = _nTextureID_1;
		aTextureID[1] = _nTextureID_2;
	}

	std::array<int, MAX_TEXTURE> aTextureID;	// テクスチャのID
	D3DXVECTOR2 textureMT;						// テクスチャ座標
};
#endif