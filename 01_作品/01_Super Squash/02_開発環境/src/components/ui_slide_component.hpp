//===================================================
//
// UIのスライドイン、アウトのアニメーションのデータ [ui_slide_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef UI_SLIDE_COMPONENT_HPP_
#define UI_SLIDE_COMPONENT_HPP_

//***************************************************
// UIのスライドイン、アウトのアニメーションのデータの定義
//***************************************************
struct UISlideComponent
{
	UISlideComponent(const D3DXVECTOR2& _destPosIn,const D3DXVECTOR2& _destPosOut, const int _nTime,const D3DXVECTOR2& _firstPos) :
		firstPos(_firstPos), destPosIn(_destPosIn), destPosOut(_destPosOut),
		nTime(_nTime), nCounter(0), bOut(false)
	{}

	D3DXVECTOR2 firstPos;	// 最初の位置
	D3DXVECTOR2 destPosIn;	// 目的の位置(スライドイン)
	D3DXVECTOR2 destPosOut;	// 目的の位置(スライドアウト)
	int nTime;				// スライド時間
	int nCounter;			// スライドカウンター
	bool bOut;				// スライドアウト開始フラグ
};
#endif