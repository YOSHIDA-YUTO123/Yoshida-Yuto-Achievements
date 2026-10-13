//===================================================
//
// リザルトの処理 [FunctionResult.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "FunctionGameEnd.h"
#include "particle.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Result_Constants
{
	const D3DXVECTOR3 EFFECT_POS_000 = { 1450.0f,0.0f, 50.0f };	// エフェクトの発生位置(左)
	const D3DXVECTOR3 EFFECT_POS_001 = { 1450.0f,0.0f, 80.0f };	// エフェクトの発生位置(右)
	const D3DXVECTOR3 EFFECT_POS_002 = { 1550.0f,0.0f, 80.0f };	// エフェクトの発生位置(右)
	const D3DXVECTOR3 EFFECT_POS_003 = { 1550.0f,0.0f, 50.0f };	// エフェクトの発生位置(右)

	constexpr int PARTICLE_SPAWN_TIME = 30;						// エフェクトの発生インターバル
}

//===================================================
// コンストラクタ
//===================================================
CFunctionGameEnd::CFunctionGameEnd() : 
	m_nEffectCounter(NULL)
{
}

//===================================================
// リザルトの処理
//===================================================
void CFunctionGameEnd::Update(void)
{
	m_nEffectCounter--;

	if (m_nEffectCounter <= 0)
	{
		// パーティクルの生成
		auto p = CParticle::Create(Result_Constants::EFFECT_POS_001, D3DCOLOR_RGBA(255, 127, 10, 255), 10.0f, 8, CParticle::TYPE_NORMAL);
		p->SetParam({ 10.0f,15.0f,1.0f }, 30, 10, 5, -5);
		p->SetParam(0.0f, 0.0f);

		// パーティクルの生成
		p = CParticle::Create(Result_Constants::EFFECT_POS_002, D3DCOLOR_RGBA(255, 127, 10, 255), 10.0f, 8, CParticle::TYPE_NORMAL);
		p->SetParam({ 10.0f,15.0f,1.0f }, 30, 10, 5, -5);
		p->SetParam(0.0f, 0.0f);


		// パーティクルの生成
		p = CParticle::Create(Result_Constants::EFFECT_POS_000, D3DCOLOR_RGBA(240, 230, 140, 255), 4.0f, 8, CParticle::TYPE_NORMAL);
		p->SetParam({ 10.0f,5.0f,10.0f }, 5, 60, 50, -50);
		p->SetParam(0.0f, 0.0f);

		// パーティクルの生成
		p = CParticle::Create(Result_Constants::EFFECT_POS_003, D3DCOLOR_RGBA(240, 230, 140, 255), 4.0f, 8, CParticle::TYPE_NORMAL);
		p->SetParam({ 10.0f,5.0f,10.0f }, 5, 60, 50, -50);
		p->SetParam(0.0f, 0.0f);

		m_nEffectCounter = Result_Constants::PARTICLE_SPAWN_TIME;
	}
}
