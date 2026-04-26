//===================================================
//
// パーティクルのデータ [particle_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PARTICLE_COMPONENT_HPP_
#define _PARTICLE_COMPONENT_HPP_

//***************************************************
// パーティクルのデータの定義
//***************************************************
struct ParticleComponent
{
	static constexpr float MIN_ANGLE = -314;	// 最小の角度
	static constexpr float MAX_ANGLE = 314;		// 最大の角度
	static constexpr int MIN_PARTICLE = 1;		// 最小のパーティクル数
	static constexpr int MIN_SPAWN_TIME = 1;	// 最小の生成時間

	// パラメータ
	struct Param
	{
		std::string texturePath;				// テクスチャのパス
		D3DXVECTOR3 pos = Const::VEC3_NULL;		// 位置
		D3DXVECTOR3 move = Const::VEC3_NULL;	// 移動量
		D3DXCOLOR col = Const::WHITE;			// 色
		float fAngleMax = MAX_ANGLE;			// 最大放射角度
		float fAngleMin = MIN_ANGLE;			// 最小放射角度
		float fRadius = 0.0f;					// 半径
		int nNum = MIN_PARTICLE;				// 発生する粒子の数
		int nLife = 0;							// エフェクトの寿命
		int nSpawnTime = MIN_SPAWN_TIME;		// 発生時間
		unsigned int nEffectFlag = 0;			// エフェクトのフラグ
	};

	ParticleComponent(const Param& _param) :
		param(_param)
	{}

	Param param; // パラメータ
};
#endif