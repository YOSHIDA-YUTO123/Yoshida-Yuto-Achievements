//===================================================
//
// カメラの揺れの処理 [camera_shake.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_shake.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CCameraShake::CCameraShake() : 
	m_fRange(NULL),
	m_nTime(NULL),
	m_posRStock(Const::VEC3_NULL),
	m_bStock(false)
{
}

//===================================================
// デストラクタ
//===================================================
CCameraShake::~CCameraShake()
{
}

//===================================================
// 揺れの開始
//===================================================
void CCameraShake::Begin(const float fRange, const int nTime)
{
	if (m_nTime <= 0)
	{
		m_fRange = fRange;
		m_nTime = nTime;
	}
}

//===================================================
// 更新処理
//===================================================
void CCameraShake::Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot)
{
	// 揺れの時間を減らす
	m_nTime--;

	if (m_nTime >= 0)
	{
		// ストックされていないなら
		if (!m_bStock)
		{
			// 一時保存
			m_posRStock = posR;
			m_bStock = true;
		}

		// 注視点を設定する
		posR.x += math::Random(-m_fRange, m_fRange);
		posR.z += math::Random(-m_fRange, m_fRange);
	}
	else if(m_bStock)
	{
		// 終了したら注視点をもとに戻す
		posR = m_posRStock;
		m_bStock = false;
	}

	// 使わない
	(void)posV;
	(void)rot;
}