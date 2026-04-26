//===================================================
//
// 読み込むファイルパスをまとめる名前空間 [loader_constants.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LOADER_CONSTANTS_HPP_
#define _LOADER_CONSTANTS_HPP_

//***************************************************
// 読み込むファイルパスをまとめる名前空間の定義
//***************************************************
namespace LoaderConst
{
	constexpr const char* SKY_PATH					= "sky.json";						// 空の情報のファイル
	constexpr const char* EFFECT_WALL_PATH			= "WALL/effect_wall.json";			// エフェクトの壁の情報のファイル
	constexpr const char* PLAYER_PATH				= "player.json";					// プレイヤーの情報のファイル
	constexpr const char* SCORE_PATH				= "data/SYSTEM/score.bin";			// スコアの情報のファイル
	constexpr const char* RANKING_PATH				= "data/SYSTEM/ranking.bin";		// ランキングの情報のファイル
	constexpr const char* SCENE_BASE_FIELD_PATH		= "FIELD/scenebase_field.json";     // シーンの基盤のフィールドの情報のファイル
	constexpr const char* FIRST_SCENE_FIELD_PATH	= "FIELD/first_scene_field.json";   // 最初のシーンのフィールドの情報のファイル
	constexpr const char* PAUSE_MENU_PATH			= "pause_menu.json";				// ポーズのメニューの情報のファイル

}
#endif