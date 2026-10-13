//===================================================
//
// ランキングのスコアのクラス [RankingScore.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "RankingScore.h"
#include "number.h"

//===================================================
// コンストラクタ
//===================================================
CRankingScore::CRankingScore() : 
	CObject(PRIORITY_UI_FRONT),
	m_apNumber(),
	m_pos(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_nScore(NULL),
	m_bFlash(false),
	m_fFlashCounter(NULL)
{
}

//===================================================
// デストラクタ
//===================================================
CRankingScore::~CRankingScore()
{
}

//===================================================
// 生成処理
//===================================================
CRankingScore* CRankingScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nScore, const bool bFlash)
{
	CRankingScore* pInstance = new CRankingScore;

	pInstance->m_pos = pos;
	pInstance->m_size = Size;
	pInstance->m_nScore = nScore;
	pInstance->m_bFlash = bFlash;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CRankingScore::Init(void)
{
	// 横幅の割合を求める
	float fWidth = m_size.x / MAX_DIGIT;

	// 基準の位置
	float fOffPosX = fWidth * 2.0f;

	int nCnt = 0;

	// スコアの数分回す
	for (auto& scores : m_apNumber)
	{
		// ナンバークラスの生成
		scores = std::make_unique<CNumber>();

		// 初期化処理
		HRESULT resutl = scores->Init(D3DXVECTOR3(m_pos.x - (fOffPosX * nCnt), m_pos.y, m_pos.z), D3DXVECTOR2(fWidth, m_size.y));

		// 初期化に失敗したら
		if (FAILED(resutl))
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		m_apNumber[nCnt]->SetTextureID("number000.png");

		nCnt++;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CRankingScore::Uninit(void)
{
	// スコアの破棄
	for (auto& scores : m_apNumber)
	{
		if (scores != nullptr)
		{
			scores->Uninit();
			scores.reset();
		}
	}

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CRankingScore::Update(void)
{
	int nCnt = 0;

	// スコアの更新
	for (auto& scores : m_apNumber)
	{
		// nullなら処理しない
		if (scores == nullptr) continue;
		
		if (m_bFlash)
		{
			m_fFlashCounter += 0.008f;

			float fAlpha = 1.0f - fabsf(sinf(m_fFlashCounter));

			scores->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, fAlpha));
		}
		// ナンバーの更新処理
		scores->Update();

		// スコアを割る値
		int nData = 10;

		// 桁に応じた値を求める
		int nDivi = (int)pow((double)nData, (double)nCnt);

		// テクスチャ座標を求める
		int nNumber = m_nScore / nDivi % 10;

		m_apNumber[nCnt]->SetUV(nNumber);
		
		nCnt++;
	}
}

//===================================================
// 描画処理
//===================================================
void CRankingScore::Draw(void)
{
	// スコアの描画
	for (auto& scores : m_apNumber)
	{
		if (scores != nullptr)
		{
			scores->Draw();
		}
	}
}
