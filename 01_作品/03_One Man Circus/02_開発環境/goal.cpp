//**************************************************
// 
// ゴール
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "goal.h"
#include "myMath.h"
#include "cylinder.h"
#include "particle.h"
#include "player.h"
#include "game.h"

//**************************************************
// ゴールデストラクタ
//**************************************************
CGoal::CGoal()
{
	m_fRadius = 0.0f;		// 半径
	m_fRadius = 0.0f;		// 半径
}

//**************************************************
// ゴールコンストラクタ
//**************************************************
CGoal::~CGoal()
{

}

//**************************************************
// ゴール初期化
//**************************************************
HRESULT CGoal::Init(void)
{
	return S_OK;
}

//**************************************************
// ゴール終了
//**************************************************
void CGoal::Uninit(void)
{
	CObjectX::Uninit();
}

//**************************************************
// ゴール更新
//**************************************************
void CGoal::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 pos = CObjectX::GetPosition();

	// プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		if (pPlayer->GetDistance(pos) <= 400.0f)
		{
			// パーティクルの生成
			CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 2.0f, 5, CParticle::TYPE_NORMAL);
			pParticle->SetParam(D3DXVECTOR3(2.0f, 5.0f, 2.0f), 1, 30, 50, -50);
			pParticle->SetParam(0.0f, 0.0f, 20.0f);
		}
	}
}

//**************************************************
// ゴール描画
//**************************************************
void CGoal::Draw(void)
{
	CObjectX::Draw();
}

//***************************************************************
// ゴール生成
//***************************************************************
CGoal* CGoal::Create(void)
{
	// ゴールインスタンス生成
	CGoal* pGoal = new CGoal;

	// 初期化
	if (pGoal != nullptr)
	{
		pGoal->Init();
	}

	return pGoal;
}

//***************************************************************
// ゴール生成
//***************************************************************
bool CGoal::Collision(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutMove, D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
	D3DXVECTOR3 posMy =  CObjectX::GetPosition();
	float fTop = posMy.y + m_fHeight;
	float fButtom = posMy.y;

	// 上下の範囲内で
	if (pos.y <= fTop && pos.y >= fButtom)
	{
		// 側面からめり込んだ
		if (CMyMath::IsColiisionCircleAndCircle2D(D3DXVECTOR2(posMy.x, posMy.z), m_fRadius, D3DXVECTOR2(pos.x, pos.z), 0.0f) == true)
		{
			if (CMyMath::IsColiisionCircleAndCircle2D(D3DXVECTOR2(posMy.x, posMy.z), m_fRadius, D3DXVECTOR2(posOld.x, posOld.z), 0.0f) == false)
			{
				// 押し出す向きを算出
				D3DXVECTOR3 vecPush = pos - posMy;
				vecPush.y = 0.0f;
				D3DXVec3Normalize(&vecPush, &vecPush);

				// 押し出し市設定
				*pOutPos = posMy + vecPush * m_fRadius;
				*pOutPos += vecPush * 0.1f;
				pOutPos->y = pos.y;

				return false;
			}
		}
	}

	// 位置が内側であって
	if (CMyMath::IsColiisionCircleAndCircle2D(D3DXVECTOR2(posMy.x, posMy.z), m_fRadius, D3DXVECTOR2(pos.x, pos.z), 0.1f) == true)
	{
		// 上からめり込んだ
		if (pos.y < fTop && posOld.y >= fTop)
		{
			// 押し返す
			*pOutPos = pos;
			pOutPos->y = fTop;
			pOutMove->y = 0.0f;
			return true;
		}
		// 下からめり込んだ
		else if (pos.y > fButtom && posOld.y <= fButtom)
		{
			// 押し返す
			*pOutPos = pos;
			pOutPos->y = fButtom;
			pOutMove->y = 0.0f;
			return false;
		}
	}

	return false;
}

//***************************************************************
// ゴール生成
//***************************************************************
void CGoal::LoadModel(const char* pModelFilePath)
{
	// モデルを読み込む
	CObjectX::LoadModel(pModelFilePath); 
	
	// 最大値と最小値を取る
	D3DXVECTOR3 max = CObjectX::GetMax();
	D3DXVECTOR3 min = CObjectX::GetMin();

	// 高さと幅を決める
	m_fHeight = max.y;
	m_fRadius = max.x;
}