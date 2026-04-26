//===================================================
//
// モーションブラーのデータ [motion_blur_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_BLUR_COMPONENT_HPP_
#define _MOTION_BLUR_COMPONENT_HPP_

//***************************************************
// モーションブラーのデータの定義
//***************************************************
struct MotionBlurComponent
{
	MotionBlurComponent(const int _nStrength) : 
		nStrength(_nStrength), bShow(true)
	{}

	int nStrength;	// ブラーの強度
	bool bShow;		// 表示するかどうか
};
#endif