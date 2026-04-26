//===================================================
//
// タイマーのデータ [timer_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TIMER_COMPONENT_HPP_
#define _TIMER_COMPONENT_HPP_

//***************************************************
// タイマーのデータの定義
//***************************************************
struct TimerComponent
{
	// 最大の桁数
	static constexpr int MAX_DIGIT = 2;

	TimerComponent(const int _nTime) : 
		aSecondID(), aMinuteID(), nTime(_nTime), 
		nMinuteValue(_nTime / Const::FRAME), nSecondValue(_nTime % Const::FRAME),
		nCounter(NULL)
	{}

	std::array<entt::entity, MAX_DIGIT> aSecondID;	// 秒のエンティティ
	std::array<entt::entity, MAX_DIGIT> aMinuteID;	// 分のエンティティ

	int nTime;										// 時間
	int nMinuteValue;								// 分
	int nSecondValue;								// 秒
	int nCounter;									// 一秒経過を計測するカウンター
};
#endif