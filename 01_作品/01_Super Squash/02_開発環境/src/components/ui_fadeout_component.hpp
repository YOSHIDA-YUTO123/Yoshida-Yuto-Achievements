//===================================================
//
// フェードアウトして消えるUIのデータ [ui_fadeout_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UI_FADE_OUT_COMPONENT_HPP_
#define _UI_FADE_OUT_COMPONENT_HPP_

//***************************************************
// フェードアウトして消えるUIのデータの定義
//***************************************************
struct UIFadeOutComponent
{
	UIFadeOutComponent(const int _nTime, const bool _bPlay = false) :
		nTime(_nTime), nCount(0), bPlay(_bPlay)
	{}

	int nTime;	// 時間
	int nCount;	// カウンター
	bool bPlay; // 再生できるかどうか
};
#endif