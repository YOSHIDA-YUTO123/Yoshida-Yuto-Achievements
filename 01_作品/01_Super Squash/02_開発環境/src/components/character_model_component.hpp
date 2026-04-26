//===================================================
//
// キャラクターのモデルのデータ [character_model_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_MODEL_COMPONENT_HPP_
#define _CHARACTER_MODEL_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"

//***************************************************
// キャラクターのデータ構造体の定義
//***************************************************
struct CharacterModelComponent
{
	// モデルの種類
	enum MODEL
	{
		MODEL_WAIST = 0,	// 腰
		MODEL_CHEST,		// 胸
		MODEL_HEAD,			// 頭
		MODEL_ARM_UR,		// 右上腕
		MODEL_ARM_FR,		// 右前腕
		MODEL_HAND_R,		// 右手
		MODEL_ARM_UL,		// 左上腕
		MODEL_ARM_FL,		// 左前腕
		MODEL_HAND_L,		// 左手
		MODEL_LEG_UR,		// 右太もも
		MODEL_LEG_DR,		// 右脛
		MODEL_FOOT_R,		// 右足
		MODEL_LEG_UL,		// 左太もも
		MODEL_LEG_DL,		// 左脛
		MODEL_FOOT_L,		// 左足
		MODEL_RACKET,		// ラケット
		MODEL_MAX,
	};

	CharacterModelComponent(const int _nModelID, const int _nIdex, const D3DXVECTOR3& _size = Const::VEC3_NULL) :
		size(_size), nIdex(_nIdex), nModelID(_nModelID)
	{
	}

	D3DXVECTOR3 size;		// 大きさ
	int nIdex;				// インデックス					
	int nModelID;			// モデルのID
};
#endif