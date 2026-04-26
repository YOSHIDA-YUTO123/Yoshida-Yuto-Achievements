//===================================================
//
// エフェクトのデータ [effect_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _EFFECT_COMPONENT_HPP_
#define _EFFECT_COMPONENT_HPP_

//***************************************************
// エフェクトのデータの定義
//***************************************************
struct EffectComponent
{
	enum FLAG
	{
		FLAG_NORMAL		= 0,			// 通常
		FLAG_DEC_ALPHA	= 1 << 0,		// 透明度の減少
		FLAG_DEC_RADIUS = 1 << 1,		// 半径の減少
		FLAG_GRAVITY	= 1 << 2,		// 重力適応
		FLAG_MAX
	};

	EffectComponent() = default;
	EffectComponent(const int _nFlag, const int _nLife, const float _fDecRadius, const float _fDecAlphaLv) :
		fDecRadius(_fDecRadius), fDecAlphaLv(_fDecAlphaLv), nFlag(_nFlag), nLife(_nLife) {}

	float fDecRadius;			// 半径の減少量
	float fDecAlphaLv;			// 透明度の減少スピード
	int nFlag;					// フラグ
	int nLife;					// 寿命
};
#endif