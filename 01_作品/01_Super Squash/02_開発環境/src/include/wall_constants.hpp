//===================================================
//
// 壁の定数 [wall_constants.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WALL_CONSTANTS_HPP_
#define _WALL_CONSTANTS_HPP_

//***************************************************
// 壁の定数の定義
//***************************************************
namespace WallConst
{
	constexpr const char* FRONT_WALL = "front_wall";	// 正面の壁
	constexpr const char* BACK_WALL  = "back_wall";		// 後ろの壁
	constexpr const char* RIGHT_WALL = "right_wall";	// 右の壁
	constexpr const char* LEFT_WALL	 = "left_wall";		// 左の壁
	constexpr const char* TOP_WALL	 = "top_wall";		// 上の壁

	const float TUTORIAL_SCROL_SPEED = 0.005f;			// スクロール速度
};

#endif