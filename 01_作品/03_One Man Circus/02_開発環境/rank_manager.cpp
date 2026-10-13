//===============================================
//
// ランキングマネージャー[rank_manager.h]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "rank_manager.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "ranking.h"
#include "RankingScore.h"

//===============================================
// コンストラクタ
//===============================================
CRankMana::CRankMana()
{
	m_nRankIn = -1;
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_AnimCnt[nCnt] = 0.0f;
	}
	for (int nCnt = 0; nCnt < 64; nCnt++)
	{
		m_pTxtName[nCnt] = {};
	}
}
//===============================================
// デストラクタ
//===============================================
CRankMana::~CRankMana()
{
}
//===============================================
// 生成処理
//===============================================
CRankMana* CRankMana::Create(D3DXVECTOR3 pos, const char* pTxtName)
{
	CRankMana* pScoreMana = new CRankMana;

	pScoreMana->Init(pos, pTxtName);

	return pScoreMana;
}
//===============================================
// リセット
//===============================================
void CRankMana::Reset(void)
{
	FILE* pFile;
	// ランキングファイルを読み込んで m_nScore に格納
	pFile = fopen(m_pTxtName, "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &m_nScore[nCnt]);
		}
		fclose(pFile);
	}
	else
	{
		// ファイルが存在しない場合は全て 0 で初期化
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_nScore[nCnt] = 0;
		}
	}
}
//===============================================
// 初期化処理
//===============================================
HRESULT CRankMana::Init(D3DXVECTOR3 pos, const char* pTxtName)
{
	strcpy(m_pTxtName, pTxtName);

	Reset();

	//順位テクスチャ
	const char* apFileName[MAX_RANK] =
	{
		"data\\TEXTURE\\rank01.png",
		"data\\TEXTURE\\rank02.png",
		"data\\TEXTURE\\rank03.png",
		"data\\TEXTURE\\rank04.png",
		"data\\TEXTURE\\rank05.png",
	};

	float fSize = RANKSCORE_SIZE * 5.5f;
	m_posOffset = pos;
	m_fSize = fSize;

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CRankMana::Uninit(void)
{

}
//===============================================
// 更新処理
//===============================================
void CRankMana::Update(void)
{
}
//===============================================
// 描画処理
//===============================================
void CRankMana::Draw(void)
{
}
//===============================================
// 設定
//===============================================
void CRankMana::Set(int nScore)
{
	int tempScore[MAX_RANK + 1];

	FILE* pFile;
	// ファイルから既存スコアを読み込む
	pFile = fopen(m_pTxtName, "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_nScore[nCnt]);
			tempScore[nCnt] = m_nScore[nCnt];
		}
		// 今回のスコアを保存
		tempScore[MAX_RANK] = nScore;

		// 閉じる
		fclose(pFile);
	}

	// ソート
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		for (int nCnt1 = 0; nCnt1 < MAX_RANK - nCnt; nCnt1++)
		{
			if (tempScore[nCnt1] < tempScore[nCnt1 + 1])
			{
				int tmp = tempScore[nCnt1];
				tempScore[nCnt1] = tempScore[nCnt1 + 1];
				tempScore[nCnt1 + 1] = tmp;
			}
		}
	}
	// 上位を保存
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_nScore[nCnt] = tempScore[nCnt];
	}

	// ランクインしているかどうか
	SetRankIn(nScore);

	// スコアの生成
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		// 点滅するかどうか
		const bool bFlash = (m_nRankIn == nCnt);

		// ランキングのスコアの生成
		CRankingScore::Create({ m_posOffset.x,(m_posOffset.y + nCnt * 100.0f),0.0f }, { 250.0f,55.0f }, tempScore[nCnt], bFlash);
	}


	// ファイルに書き込み保存
	pFile = fopen(m_pTxtName, "w");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{// 最大ランク分書き込む
			fprintf(pFile, "%d\n", m_nScore[nCnt]);
		}
		// 閉じる
		fclose(pFile);
	}
	else
	{// エラーチェック
	}
}
//===============================================
// ランクインしているか
//===============================================
void CRankMana::SetRankIn(int nScore)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_nScore[nCnt] == nScore)
		{
			m_nRankIn = nCnt;
			break;
		}
	}
}

