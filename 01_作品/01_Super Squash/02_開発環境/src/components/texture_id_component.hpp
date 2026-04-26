//===================================================
//
// テクスチャのIDのデータ [texture_id_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTUREID_COMPONENT_HPP_
#define _TEXTUREID_COMPONENT_HPP_

//***************************************************
// テクスチャのIDのコンポーネントの取得
//***************************************************
struct TextureIDComponent
{
	static constexpr int INVALID_ID = -1;	// 無効なテクスチャのID

	TextureIDComponent(const int _nTextureID = INVALID_ID) :
		nTextureID(_nTextureID)
	{}

	int nTextureID; // テクスチャのID
};
#endif