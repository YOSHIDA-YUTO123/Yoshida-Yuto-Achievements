//===================================================
//
// リザルトの処理 [FunctionResult.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "FunctionResult.h"
#include "particle.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Result_Constants
{
	const D3DXVECTOR3 EFFECT_POS_000 = { -130.0f,30.0f,0.0f };	// エフェクトの発生位置(左)
	const D3DXVECTOR3 EFFECT_POS_001 = { 130.0f,30.0f,0.0f };	// エフェクトの発生位置(右)
	constexpr int PARTICLE_SPAWN_TIME = 90;						// エフェクトの発生インターバル
}

//===================================================
// コンストラクタ
//===================================================
CFunctionResult::CFunctionResult() : 
	m_nEffectCounter(NULL)
{
}

//===================================================
// リザルトの処理
//===================================================
void CFunctionResult::Update(void)
{
	m_nEffectCounter--;

	if (m_nEffectCounter <= 0)
	{
		// パーティクルの生成
		auto p = CParticle::Create(Result_Constants::EFFECT_POS_000, D3DCOLOR_RGBA(240, 230, 140, 255), 4.0f, 10, CParticle::TYPE_NORMAL);
		p->SetParam({ 10.0f,4.0f,1.0f }, 5, 120, 50, 40);
		p->SetParam(0.0f, -0.1f);

		// パーティクルの生成
		p = CParticle::Create(Result_Constants::EFFECT_POS_001, D3DCOLOR_RGBA(240, 230, 140, 255), 4.0f, 10, CParticle::TYPE_NORMAL);
		p->SetParam({ -10.0f,4.0f,1.0f }, 5, 120, 50, 40);
		p->SetParam(0.0f, -0.1f);

		m_nEffectCounter = Result_Constants::PARTICLE_SPAWN_TIME;
	}
}
