//===================================================
//
// リザルトの処理のデータ [result_system_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RESULT_SYSTEM_COMPONENT_HPP_
#define _RESULT_SYSTEM_COMPONENT_HPP_

//***************************************************
// リザルトのスコアのシステムのデータの定義
//***************************************************
struct ResultSystemComponent
{
	ResultSystemComponent(const int _nBallCreateTime) :
		nDigit(0), nBallCreateTime(_nBallCreateTime), bFinishCreateScore(false),
		bFinishCreateRanking(false)
	{}

	int nDigit;					// 現在表示中の桁
	int nBallCreateTime;		// ボールが出現する時間
	bool bFinishCreateScore;	// スコアの生成が完了したら
	bool bFinishCreateRanking;	// ランキングの生成が完了したら
};
#endif