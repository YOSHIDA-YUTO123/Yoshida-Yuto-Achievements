//===================================================
//
// スコアエリアのポリゴンのデータ [score_area_polygon_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef SCRE_AREA_POLYGON_COMPONENT_HPP_
#define SCRE_AREA_POLYGON_COMPONENT_HPP_

//***************************************************
// スコアエリアのポリゴンのデータの定義
//***************************************************
struct ScoreAreaPolygonComponent
{
	static constexpr int INVALID_TIME = -1;	// 揺れの時間-1は time <= 0に反応する可能性があるため。

	ScoreAreaPolygonComponent() :
		destPos(Const::VEC3_NULL), fShakeRangeMin(0.0f),fShakeRangeMax(0.0f), nShakeTime(INVALID_TIME),
		bFadeOut(false), bScoreArea(false)
	{}

	D3DXVECTOR3 destPos;	// 目的の位置
	float fShakeRangeMin;	// 揺れの範囲最小
	float fShakeRangeMax;	// 揺れの範囲最大
	int nShakeTime;			// 揺れる時間
	bool bFadeOut;			// フェードアウト開始用フラグ
	bool bScoreArea;		// スコアを獲得できるエリアかどうか
};

#endif