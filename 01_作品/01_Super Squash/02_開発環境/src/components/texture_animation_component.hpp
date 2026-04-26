//===================================================
//
// テクスチャアニメーションのデータ [texture_animation_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_ANIMATION_COMPONENT_HPP_
#define _TEXTURE_ANIMATION_COMPONENT_HPP_

//***************************************************
// テクスチャアニメーションのデータの定義
//***************************************************
struct TextureAnimationComponent
{
	TextureAnimationComponent(const int _nSpeed, const bool _bLoop) :
		nSpeed(_nSpeed), bLoop(_bLoop), nPattern(0),nCounter(0)
	{}

	int nPattern;	// パターン
	int nCounter;	// カウンター
	int nSpeed;		// 速さ
	bool bLoop;		// ループするかどうか
};
#endif