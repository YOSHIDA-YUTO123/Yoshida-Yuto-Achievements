//===================================================
//
// チュートリアルのUIのデータ [tutorial_ui_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TUTORIAL_UI_COMPONENT_HPP_
#define _TUTORIAL_UI_COMPONENT_HPP_

//***************************************************
// チュートリアルのUIのデータの定義
//***************************************************
struct TutorialUIComponent
{
	TutorialUIComponent(const int _nLife) :
		nLife(_nLife)
	{}

	int nLife; // 寿命
};
#endif