//===================================================
//
// テクスチャアニメーションパーティクルのデータ [animation_particle_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ANIMATION_PARTICLE_COMPONENT_HPP_
#define _ANIMATION_PARTICLE_COMPONENT_HPP_

//***************************************************
// テクスチャアニメーションパーティクルのデータの定義
//***************************************************
struct AnimationParticleComponent
{
	AnimationParticleComponent(const int _nSpeed, const bool _bLoop, const int _nSegmentU, const int _nSegmentV) :
		nSpeed(_nSpeed), bLoop(_bLoop), nSegmentU(_nSegmentU), nSegmentV(_nSegmentV)
	{}

	int nSpeed;		// 速さ
	int bLoop;		// ループするかどうか
	int nSegmentU;	// 分割数横
	int nSegmentV;	// 分割数縦
};
#endif