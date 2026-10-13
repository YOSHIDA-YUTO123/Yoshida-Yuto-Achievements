//***************************************************
//
// ジャグリングボール
// Author:
//
//***************************************************
#include "balljug.h"
#include "myMath.h"
#include "player.h"
#include "game.h"
#include "ruleBalance.h"
#include "ruleJuggling.h"

//***************************************************
// ジャグリングボール
//***************************************************
CBallJug::CBallJug()
{
	m_bApp = false;
	m_bFall = false;
	m_bFire = false;
	m_bLoss = false;
	m_nCnt = 0;
}

//***************************************************
// ジャグリングボール
//***************************************************
CBallJug::~CBallJug()
{

}

//***************************************************
// ジャグリングボール初期化
//***************************************************
HRESULT CBallJug::Init(void)
{
	m_pPlayer = nullptr;

	// ジャグリング用オブジェクト生成
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		CObjectX* pObjeX = CObjectX::Create(Const::VEC3_NULL, "MapObjectFile\\ballJa.x", Const::VEC3_NULL);
		m_vpJugObj.push_back(pObjeX);
	}

	m_fAngle = 0.0f;
	m_fAngleOld = 0.0f;
	m_fRadius = 0.0f;

	m_fAngleDest = ADDROTSTION;

	m_bLoss = false;
	return S_OK;
}

//***************************************************
// ジャグリングボール破棄
//***************************************************
void CBallJug::Uninit(void)
{
	// ボール削除
	for (auto& iter : m_vpJugObj)
	{
		iter->Uninit();
	}
	m_vpJugObj.clear();

	Release();
}

//***************************************************
// ジャグリングボール
//***************************************************
void CBallJug::Update(void)
{
	if (m_bFall == true)
	{
		fall();
	}
	else if (m_bFire == true)
	{
		fire();
	}
	else
	{
		Jugling();
	}

}

//***************************************************
// ジャグリングボール
//***************************************************
void CBallJug::Draw(void)
{

}

//***************************************************
// ジャグリングボール生成
//***************************************************
CBallJug* CBallJug::Create(void)
{
	CBallJug* pInstance = new CBallJug;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}
	// ジャグリングボールの生成
	return pInstance;
}

//***************************************************
// ジャグリングボール生成
//***************************************************
void CBallJug::Jugling(void)
{
	// プレイヤーのマトリックス取得
	D3DXMATRIX mtxPlayer = m_pPlayer->GetMatrix();
	D3DXVECTOR3 rotPlayer = m_pPlayer->GetRotationBarance();

	// オブジェクトカウンター
	int nCntObj = 0;

	// 個数で割った角度
	float fAngleDivide = D3DX_PI * 2.0f / (float)m_vpJugObj.size();

	// 成功していれば回す
	if (CGame::GetRuleResult() == CRuleBase::RESULT_SUCCESS)
	{
		// 角度をたす
		m_fAngleDest = ADDROTSTION;
	}
	// 失敗していれば回転数を下げる
	else if (CGame::GetRuleResult() == CRuleBase::RESULT_PLAYER_MISS)
	{
		// 角度をたす
		m_fAngleDest -= m_fAngleDest * 0.5f;
	}

	m_fAngleOld = m_fAngle;

	// 角度を近づける
	m_fAngle += m_fAngleDest;
	m_fAngleDest -= m_fAngleDest * 0.01f;
	m_fRadius += (RADIUS - m_fRadius) * 0.01f;

	// ジャグリング用オブジェクト更新
	for (auto& iterJugObj : m_vpJugObj)
	{
		// オブジェクト用のアングル計算
		float fAngleObj = m_fAngle;
		fAngleObj += fAngleDivide * (float)nCntObj;

		// 位置
		D3DXVECTOR3 pos = Const::VEC3_NULL;

		// 位置設定
		pos.x += sinf(fAngleObj) * m_fRadius;
		pos.y += cosf(fAngleObj) * m_fRadius;
		pos.z += DIPS;
		pos.y += 40.0f;

		// プレイヤーの回転をプラス
		pos = CMyMath::MulWorldMatrix(pos, mtxPlayer);

		// 位置設定
		iterJugObj->SetPosition(pos);

		// オブジェクトカウンター追加
		nCntObj++;
	}

	// 遅くなったら失敗
	if (fabsf(m_fAngle - m_fAngleOld) < D3DX_PI * 0.01f)
	{
		m_fRadius = 0.0f;
		m_fAngleOld = 0.0f;
		m_bApp = false;
		m_bFall = false;
		m_bFire = true;
		m_bLoss = true;
	}
	
}

//***************************************************
// ジャグリングボール落ちる
//***************************************************
void CBallJug::fall(void)
{
	m_fRadius -= 0.1f;

	// ジャグリング用オブジェクト更新
	for (auto& iterJugObj : m_vpJugObj)
	{
		D3DXVECTOR3 pos = iterJugObj->GetPosition();
		pos.y += m_fRadius;
		// 位置設定
		iterJugObj->SetPosition(pos);

		if (pos.y < -100.0f)
		{
			Uninit();
			return;
		}
	}
}

//***************************************************
// ジャグリングボール爆散
//***************************************************
void CBallJug::fire(void)
{
	m_fAngleOld -= 0.1f;
	m_fRadius += 1.0f + m_fRadius * 0.7f;

	// オブジェクトカウンター
	int nCntObj = 0;

	// 個数で割った角度
	float fAngleDivide = D3DX_PI * 2.0f / (float)m_vpJugObj.size();

	// ジャグリング用オブジェクト更新
	for (auto& iterJugObj : m_vpJugObj)
	{
		// オブジェクト用のアングル計算
		float fAngleObj = m_fAngle;
		fAngleObj += fAngleDivide * (float)nCntObj;

		// 位置
		D3DXVECTOR3 pos = iterJugObj->GetPosition();

		// 位置設定
		pos.x += sinf(fAngleObj) * m_fRadius;
		pos.y += cosf(fAngleObj) * m_fRadius ;
		pos.y += m_fAngleOld;

		// 位置設定
		iterJugObj->SetPosition(pos);

		// オブジェクトカウンター追加
		nCntObj++;

		if (pos.y < -100.0f)
		{
			Uninit();
			return;
		}
	}
}