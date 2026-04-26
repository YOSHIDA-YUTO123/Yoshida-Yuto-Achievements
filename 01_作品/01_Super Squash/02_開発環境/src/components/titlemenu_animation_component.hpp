//===================================================
//
// タイトルのメニューのアニメーションのデータ [titlemenu_animation_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TITLEMENU_ANIMATION_COMPONENT_HPP_
#define _TITLEMENU_ANIMATION_COMPONENT_HPP_

//***************************************************
// タイトルのメニューのアニメーションのデータの定義
//***************************************************
struct TitleMenuAnimationComponent
{
	TitleMenuAnimationComponent() :
		bPlay(false)
	{}

	bool bPlay;	// 再生できるかどうか
};
#endif