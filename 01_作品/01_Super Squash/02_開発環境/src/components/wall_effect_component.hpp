//===================================================
//
// 壁のエフェクトのデータの定義 [wall_effect_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WALL_EFFECT_COMPONENT_HPP_
#define _WALL_EFFECT_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 壁のエフェクトのデータの定義
//***************************************************
struct WallEffectComponent
{
	WallEffectComponent(const entt::entity _wallID, const D3DXVECTOR3 _epicenter, const float _fRadius, const float _fRough, const int _nTime) :
		wallID(_wallID), epicenter(_epicenter), fRadius(_fRadius), fRough(_fRough), nTime(_nTime), nMaxTime(_nTime)
	{}

	entt::entity wallID;	// 発生させる壁のID
	D3DXVECTOR3 epicenter;	// 発生地点
	float fRadius;			// 半径
	float fRough;			// 粗さ
	int nTime;				// 発生時間
	int nMaxTime;			// 最大の時間
};
#endif