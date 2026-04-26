//===================================================
//
// スコアのエリアのデータ [score_area_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCORE_AREA_COMPONENT_HPP_
#define _SCORE_AREA_COMPONENT_HPP_

//***************************************************
// スコアのエリアのデータの定義
//***************************************************
struct ScoreAreaComponent
{
	static constexpr float COLLIDER_DEPTH	= 70.0f;	// コライダーのz方向の大きさ
	static constexpr float POLYGON_ALPHA	= 0.4f;		// ポリゴンの透明度
	static constexpr int MAX_AREA			= 36;		// 最大のエリア数
	static constexpr int INTERVAL			= -1;		// インターバル-1はinterval <= 0の条件式に反応する可能性があるため。	
	static constexpr int INVALID_AREA		= -1;		// 無効なインデックス

	// データ
	struct Data
	{
		entt::entity polygonID;			// エリアのポリゴンのID
		bool bActive;					// 使用状態かどうか
	};

	ScoreAreaComponent(const D3DXVECTOR3& _CenterPos) :
		data(), centerPos(_CenterPos), size(Const::VEC2_NULL),
		nInterval(INTERVAL), nSegmentU(0), nSegmentV(0),
		nOldScoreArea(INVALID_AREA)
	{}

	std::array<Data, MAX_AREA> data;	// データ
	D3DXVECTOR3 centerPos;				// 中心座標
	D3DXVECTOR2 size;					// 大きさ
	int nInterval;						// インターバル
	int nOldScoreArea;					// 前回のスコアが入るエリアの番号
	int nSegmentU;						// 出現エリアの横の分割数
	int nSegmentV;						// 出現エリアの縦の分割数
};
#endif