//===================================================
//
// ボールの定数データ [ball_constants.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_CONSTANTS_HPP_
#define _BALL_CONSTANTS_HPP_

//***************************************************
// ボールの定数データの名前空間の定義
//***************************************************
namespace BallConstants
{
	constexpr const char* TEXTURE_PATH			= "data/TEXTURE/ball000.png";		// テクスチャのパス
	constexpr float RADIUS						= 7.0f;								// ボールの半径
	constexpr float INIT_SPEED					= 1.0f;								// 初期スピード	
	constexpr float MOVE_THRESHOLD_LEVEL_SMALL	= 20.0f;							// 移動量のしきい値Lv1
	constexpr float MOVE_THRESHOLD_LEVEL_MEDIUM	= 40.0f;							// 移動量のしきい値Lv2
	constexpr float MOVE_THRESHOLD_LEVEL_MAX	= 70.0f;							// 移動量のしきい値最大
	constexpr float MAX_HEIGHT					= 20.0f;							// 高速移動時の伸びの最大の高さ
	constexpr int SEGMENT						= 16;								// 分割数
	constexpr int INTERVAVL						= 5;								// ショットしてから次打てるまでの時間
};
#endif