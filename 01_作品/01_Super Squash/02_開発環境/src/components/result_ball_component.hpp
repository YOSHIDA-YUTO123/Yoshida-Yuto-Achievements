//===================================================
//
// リザルトのボールのデータ [result_ball_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RESULT_BALL_COMPONENT_HPP_
#define _RESULT_BALL_COMPONENT_HPP_

//***************************************************
// リザルトのボールのデータの定義
//***************************************************
struct ResultBallComponent
{
	ResultBallComponent(const int _nLife) :
		nLife(_nLife), bWallHit(false)
	{}

	int nLife;		// 寿命
	bool bWallHit;	// 壁に当たったか
};
#endif