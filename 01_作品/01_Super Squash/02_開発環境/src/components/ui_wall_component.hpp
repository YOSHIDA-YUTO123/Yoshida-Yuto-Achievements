//===================================================
//
// UIの壁のデータ [ui_wall_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UI_WALL_COMPONENT_HPP_
#define _UI_WALL_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// UIの壁のデータの定義
//***************************************************
struct UIWallComponent
{
	UIWallComponent(const D3DXVECTOR2& _ScrolSpeed) :
		pTexture(nullptr), pTextureMT(nullptr), scrolSpeed(_ScrolSpeed),
		bScrol(false), bBackgrondScrol(true)
	{}

	~UIWallComponent()
	{
		pTexture = nullptr;
		pTextureMT = nullptr;
	}

	LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 pTextureMT;	// マルチテクスチャへのポインタ
	D3DXVECTOR2 scrolSpeed;			// スクロール速度
	bool bScrol;					// スクロールするかどうか
	bool bBackgrondScrol;			// 背景をスクロールするかどうか
};
#endif