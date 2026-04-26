//===================================================
//
// 点滅UIのデータ [ui_flash_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UI_FLASH_COMPONENT_HPP_
#define _UI_FLASH_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 点滅UIのデータの定義
//***************************************************
struct UIFlashComponent
{
	static constexpr float MAX_ALPHA = 1.0f;	// 最大のアルファ値

	UIFlashComponent(const float _fSpeed, const int _nFlashTime, const float _fMaxAlpha = MAX_ALPHA, const bool bPlay = false) :
		fTime(0.0f), fSpeed(_fSpeed), fMaxAlpha(_fMaxAlpha), nFlashTime(_nFlashTime), bPlay(bPlay)
	{}

	float fTime;			// 時間
	float fSpeed;			// 点滅速度
	float fMaxAlpha;		// アルファ値の最大
	int nFlashTime;			// 点滅時間
	bool bPlay;				// 再生できるかどうか
};
#endif