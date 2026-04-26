//===================================================
//
// スコアのデータ [score_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef SCORE_COMPONENT_HPP_
#define SCORE_COMPONENT_HPP_

//***************************************************
// 分割テクスチャポリゴンのデータの定義
//***************************************************
struct ScoreComponent
{
	static constexpr int MAX_DIGIT = 8; // 最大の桁数
	
	ScoreComponent(const int _nScore = 0) :
		aNumberID(), nScore(_nScore), nDigit(0)
	{}

	std::array<entt::entity, MAX_DIGIT> aNumberID; // スコアのシートのID

	int nScore; // スコア
	int nDigit;	// 桁数
};
#endif