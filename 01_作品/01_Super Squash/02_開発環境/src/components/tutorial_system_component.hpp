//===================================================
//
// チュートリアルのデータ [tutorial_system_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TUTORIAL_SYSTEM_COMPONENT_HPP_
#define _TUTORIAL_SYSTEM_COMPONENT_HPP_

//***************************************************
// チュートリアルのデータの定義
//***************************************************
struct TutorialSystemComponent
{
	static constexpr int RULE_UI_DISPLAY_TIME	= 420;	// ルールUIの使用時間
	static constexpr int FIRST_UI_DISPLAY_TIME	= 240;	// ルールUIの使用時間
	static constexpr int INTERVAL_TIME			= 180;	// インターバル

	// 状態
	enum class State
	{
		None = 0,			// 無し
		Move,				// 移動
		jetPack,			// ジェットパック
		QuickMove,			// 素早い移動
		Shot,				// ショット
		BallShot_Small,		// ボールの速度を中にする
		BallShot_Medium,	// ボールの速度を大にする
		MinigameRule,		// ミニゲームのルール
		MinigameStart,		// ミニゲームの開始
		Max
	};

	TutorialSystemComponent() :
		currentState(State::None), nFirstuiDisplayTime(FIRST_UI_DISPLAY_TIME), nInterval(INTERVAL_TIME),
		nRuleDisplayTime(RULE_UI_DISPLAY_TIME), bSetBall(false), bSetTask(false),
		bSkip(false)
	{}

	State currentState;				// 現在の状態
	int nFirstuiDisplayTime;		// 最初のUIの表示時間
	int nInterval;					// 成功判定までのインターバル
	int nRuleDisplayTime;			// ルールを表示する時間
	bool bSetBall;					// ボールがせっとされているかどうか
	bool bSetTask;					// タスクのUIを設定したかどうか
	bool bSkip;						// スキップしたかどうか
};
#endif