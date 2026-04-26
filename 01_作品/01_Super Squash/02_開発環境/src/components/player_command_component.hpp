//===================================================
//
// プレイヤーのコマンドのデータ [player_command_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_COMMAND_COMPONENT_HPP_
#define _PLAYER_COMMAND_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// プレイヤーのコマンド
//***************************************************
struct PlayerCommandComponent
{
	// プレイヤーの移動方向
	enum class MoveDir : uint8_t
	{
		None = 0,		// 移動していない
		Forward,		// 正面
		Left_Forward,	// 左上
		Left_Back,		// 左下
		Left,			// 左
		Back,			// 後ろ
		Right_Forward,	// 右上
		Right_Back,		// 右下
		Right,			// 右
		Max
	};

	PlayerCommandComponent() :
		moveDir(MoveDir::None) {}

	MoveDir moveDir;	// 移動方向
};
#endif