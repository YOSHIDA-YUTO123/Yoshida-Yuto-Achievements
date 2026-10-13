//================================================
//
// パーティクル処理 [particle.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "particle.h"
#include "math.h"
#include "effect.h"
#include "AnimationEffect.h"

//*************************************************
// 定数宣言
//*************************************************
namespace Particle_Constants
{
	// エフェクトのテクスチャ
	constexpr const char* EFFECT_TEXTURE[CParticle::TYPE_MAX] =
	{
		"effect/effect000.jpg",
		"effect/effect000.jpg",
		"explosion001.png",
	};
}

//=================================================
// コンストラクタ
//=================================================
CParticle::CParticle() : 
	m_pos(Const::VEC3_NULL),
	m_move(Const::VEC3_NULL),
	m_col(Const::WHITE),
	m_fPosRange(NULL),
	m_nLife(NULL),
	m_nSpawnTime(NULL),
	m_nNum(1),
	m_nAngleMax(1),
	m_nAngleMin(1),
	m_fRadius(NULL),
	m_fGravity(NULL),
	m_fInertia(NULL),
	m_type(TYPE_NORMAL),
	m_AnimParam(),
	m_bAnim(false)
{
}

//=================================================
// デストラクタ
//=================================================
CParticle::~CParticle()
{
}

//=================================================
// 生成処理
//=================================================
CParticle* CParticle::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius, const int nNum, const TYPE type)
{
	CParticle* pInstance = new CParticle;

	pInstance->m_pos = pos;
	pInstance->m_col = col;
	pInstance->m_fRadius = fRadius;
	pInstance->m_nNum = nNum;
	pInstance->m_type = type;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{// 初期化に失敗したら破棄する

		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//=================================================
// 初期化処理
//=================================================
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CParticle::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//=================================================
// 更新処理
//=================================================
void CParticle::Update(void)
{
	// パーティクルの粒子の数分回す
	for (int nCnt = 0; nCnt < m_nNum; nCnt++)
	{
		D3DXVECTOR3 pos = m_pos;

		// 範囲
		float fPosRangeX = math::Random(-m_fPosRange, m_fPosRange);
		float fPosRangeY = math::Random(-m_fPosRange, m_fPosRange);
		float fPosRangeZ = math::Random(-m_fPosRange, m_fPosRange);

		pos += D3DXVECTOR3(fPosRangeX, fPosRangeY, fPosRangeZ);

		D3DXCOLOR col = m_col;

		// ランダムな角度を求める
		float fAngleX = static_cast<float>(math::Random(m_nAngleMin, m_nAngleMax)) * 0.01f;
		float fAngleY = static_cast<float>(math::Random(m_nAngleMin, m_nAngleMax)) * 0.01f;

		D3DXVECTOR3 speed;

		// 移動スピードを求める
		speed.x = math::Random(m_move.x * 0.5f, m_move.x);
		speed.y = math::Random(m_move.y * 0.5f, m_move.y);
		speed.z = math::Random(m_move.z * 0.5f, m_move.z);

		D3DXVECTOR3 move;

		// 移動量を求める
		move.x = sinf(fAngleX) * sinf(fAngleY) * speed.x;
		move.y = cosf(fAngleX) * speed.y;
		move.z = sinf(fAngleX) * cosf(fAngleY) * speed.z;

		// 半径を求める
		float fRadius = math::Random(m_fRadius * 0.5f, m_fRadius);

		// 寿命
		int nLife = math::Random(m_nLife / 2, m_nLife);

		if (m_bAnim)
		{
			CAnimationEffect* pEffectAnim = CAnimationEffect::Create(pos, col, fRadius, m_AnimParam.nDiviU, m_AnimParam.nDiviV, nLife);

			// nullだったら処理を飛ばす
			if (pEffectAnim == nullptr) continue;

			// パラメータの設定処理
			pEffectAnim->SetParam(m_fInertia, move, m_AnimParam.nAnimSpeed, m_AnimParam.bLoop, m_fGravity);
			pEffectAnim->SetTextureID(Particle_Constants::EFFECT_TEXTURE[m_type]);
		}
		else
		{
			CEffect* pEffect = CEffect::Create(pos, col, fRadius, nLife);

			// nullだったら処理を飛ばす
			if (pEffect == nullptr) continue;

			// パラメータの設定処理
			pEffect->SetParam(m_fInertia, move, m_fGravity);
			pEffect->SetTextureID(Particle_Constants::EFFECT_TEXTURE[m_type]);
		}
	}

	m_nSpawnTime--;

	// 発生時間がおわったら破棄
	if (m_nSpawnTime <= 0)
	{
		Uninit();
		return;
	}
}

//=================================================
// 描画処理
//=================================================
void CParticle::Draw(void)
{
}

//=================================================
// パラメータの設定処理
//=================================================
void CParticle::SetParam(const D3DXVECTOR3 move, const int nSpawnTime, const int nLife, const int nAngleMax, const int nAngleMin)
{
	m_move = move;
	m_nSpawnTime = nSpawnTime;
	m_nLife = nLife;
	m_nAngleMax = nAngleMax;
	m_nAngleMin = nAngleMin;
}

//=================================================
// パラメータの設定処理
//=================================================
void CParticle::SetParam(const float fInertia, const float fGravity, const float fPosRange)
{
	m_fGravity = fGravity;
	m_fInertia = fInertia;
	m_fPosRange = fPosRange;
}

//=================================================
// アニメーションパラメータの設定
//=================================================
void CParticle::SetAnim(const int nDiviU, const int nDiviV, const int nAnimSpeed, const bool bLoop)
{
	m_AnimParam.nDiviU = nDiviU;
	m_AnimParam.nDiviV = nDiviV;
	m_AnimParam.nAnimSpeed = math::Random(nAnimSpeed / 2, nAnimSpeed);
	m_AnimParam.bLoop = bLoop;
	m_bAnim = true;
}