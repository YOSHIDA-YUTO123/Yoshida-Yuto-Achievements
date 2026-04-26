//===================================================
//
// ミニゲームのプレイヤーのデータ [minigame_player_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MINIGAME_PLAYER_COMPONENT_HPP_
#define _MINIGAME_PLAYER_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// ミニゲームのプレイヤーのコンポーネントの定義
//***************************************************
struct MinigamePlayerComponent
{
	static constexpr int COMBO_RESET_TIME = 600;	// コンボをリセットするまで時間
	static constexpr int INTERVAL = -1;				// インターバル-1は条件式interval <= 0に反応する可能性があるため。
	
	MinigamePlayerComponent() :
		fQuickMoveValue(0.0f), nQuickMoveInterval(INTERVAL), nQuickMoveTime(0),
		nCombo(0), nComboResetTime(0),bJetPackEffect(false)
	{}

	float fQuickMoveValue;	// 素早い移動の移動量
	int nQuickMoveInterval; // ジェットパックのス素早い移動のインターバル
	int nQuickMoveTime;		// 素早い移動の適応時間
	int nCombo;				// コンボ数
	int nComboResetTime;	// コンボをリセットする時間
	bool bJetPackEffect;	// ジェットパックのエフェクトをオンにするかどうか
};
#endif