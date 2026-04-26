//===================================================
//
// UIの拡大縮小アニメーションのコンポーネント [ui_scaling_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UI_SCALING_COMPONENT_HPP_
#define _UI_SCALING_COMPONENT_HPP_

//***************************************************
// メニューのデータの定義
//***************************************************
struct UIScalingComponent
{
	UIScalingComponent(const D3DXVECTOR2& _baseSize, const float _fSpeed, const float _fScal, const float _fScalCoef) :
		baseSize(_baseSize), fSpeed(_fSpeed), fCounter(0.0f),
		fScalCoef(_fScalCoef), fScal(_fScal), bPlay(false)
	{}

	D3DXVECTOR2 baseSize;	// 基準の大きさ
	float fSpeed;			// 速さ
	float fCounter;			// 拡大のカウンター
	float fScalCoef;		// 0.0f - 1.0f指定
	float fScal;			// 拡大率
	bool bPlay;				// アニメーションできるかどうか
};
#endif