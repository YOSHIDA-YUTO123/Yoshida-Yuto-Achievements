//===================================================
//
// プレイヤーの定数データ [player_constants.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_CONSTANTS_HPP_
#define _PLAYER_CONSTANTS_HPP_

//***************************************************
// プレイヤーの定数データの名前空間の定義
//***************************************************
namespace PlayerConstants
{
	constexpr float JUMP_VALUE				= 20.0f;				// ジャンプ量
	constexpr float SHOT_POWER				= 2.5f;					// 発射力
	constexpr float MAX_CHAGE_POWER			= 40.0f;				// チャージできる最大power
	constexpr float CHAGE_POWER_OFFSET		= 2.0f;					// 最低保障power
	constexpr float KEY_W_PITCH_ANGLE		= D3DXToRadian(15);		// Wキーを押しているときの発射角度
	constexpr float KEY_S_PITCH_ANGLE		= D3DXToRadian(25);		// Sキーを押しているときの発射角度
	constexpr float NO_INPUT_ANGLE			= D3DXToRadian(100);	// キーを押していないときの発射角度
	constexpr float JET_PACK_MOVE_UP		= 3.0f;					// ジェットパック移動量(上昇時)
	constexpr float JET_PACK_MOVE_DOWN		= -4.0f;				// ジェットパック移動量(下降時)
	constexpr float JET_PACK_MOVE_LEFT		= -4.0f;				// ジェットパック移動量(左移動)
	constexpr float JET_PACK_MOVE_RIGHT		= 4.0f;					// ジェットパック移動量(右移動)
	constexpr int JET_PACK_QUICKMOVE_TIME	= 15;					// ジェットパックの素早い移動時間
	constexpr int JET_PACK_QUICK_INTERVAL	= 120;					// ジェットパックの素早い移動のインターバル
	constexpr int CHAGE_TIME				= 10;					// チャージ時間
}
#endif