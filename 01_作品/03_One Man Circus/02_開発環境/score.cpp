//================================================
//
// スコアを表示する処理 [score.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "score.h"
#include "number.h"
#include "math.h"
#include "manager.h"
#include "scene.h"
#include "GameManager.h"

//*************************************************
// 名前空間
//*************************************************
using namespace Const; // 名前空間Constの使用
using namespace math;  // 名前空間mathの使用
using namespace std;   // 名前空間stdの使用

//=================================================
// コンストラクタ
//=================================================
CScore::CScore() : CObject(PRIORITY_UI_FRONT)
{
	m_bShowZero = false;
	m_nDigit = NULL;
	m_pos = VEC3_NULL;
	m_Size = VEC2_NULL;
	m_nScore = NULL;
	m_pGameManagerRef = nullptr;

	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}

//=================================================
// デストラクタ
//=================================================
CScore::~CScore()
{
}

//=================================================
// 生成処理
//=================================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nScore, const bool bShowZero, CGameManager* pGameManager)
{
	// スコアの生成
	CScore* pScore = new CScore;

	pScore->m_pos = pos;
	pScore->m_Size = Size;
	pScore->m_nScore = nScore;
	pScore->m_bShowZero = bShowZero;
	pScore->m_pGameManagerRef = pGameManager;

	// 初期化処理
	if (FAILED(pScore->Init()))
	{
		pScore->Uninit();
		pScore = nullptr;
		return nullptr;
	}

	return pScore;
}

//=================================================
// 初期化処理
//=================================================
HRESULT CScore::Init(void)
{
	// 計算用
	int nScoreWk = m_nScore;

	// スコアがあるなら
	if (nScoreWk != 0)
	{
		// 桁数を求める
		while (nScoreWk != 0)
		{
			nScoreWk /= 10;
			m_nDigit++;
		}
	}
	else
	{
		// スコアが0だったら桁にする
		m_nDigit++;
	}

	// 桁数の制限
	m_nDigit = Clamp(m_nDigit, 0, MAX_DIGIT);

	// ゼロを表示するなら
	if (m_bShowZero)
	{
		// 桁数を最大にする
		m_nDigit = MAX_DIGIT;
	}
	// 横幅の割合を求める
	float fWidth = m_Size.x / MAX_DIGIT;

	// 基準の位置
	float fOffPosX = fWidth * 2.0f;

	// 桁数分回す
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		// ナンバークラスの生成
		m_apNumber[nCnt] = make_unique<CNumber>();

		// 初期化処理
		HRESULT resutl = m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fOffPosX * nCnt), m_pos.y, m_pos.z), D3DXVECTOR2(fWidth, m_Size.y));

		// 初期化に失敗したら
		if (FAILED(resutl))
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		m_apNumber[nCnt]->SetTextureID("number000.png");
	}

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CScore::Uninit(void)
{
	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// nullなら処理しない
		if (m_apNumber[nCnt] == nullptr) continue;

		// 終了処理
		m_apNumber[nCnt]->Uninit();
		m_apNumber[nCnt].reset();
	}
	
	if (m_pGameManagerRef != nullptr)
	{
		m_pGameManagerRef->SaveScore(CGameManager::SAVE_SCORE_FILE, m_nScore);
	}
	// 自分自身の破棄
	CObject::Release();
}

//=================================================
// 更新処理
//=================================================
void CScore::Update(void)
{
	// 桁数分回す
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		// nullなら処理しない
		if (m_apNumber[nCnt] == nullptr) continue;

		// ナンバーの更新処理
		m_apNumber[nCnt]->Update();

		// スコアを割る値
		int nData = 10;

		// 桁に応じた値を求める
		int nDivi = (int)pow((double)nData, (double)nCnt);

		// テクスチャ座標を求める
		int nNumber = m_nScore / nDivi % 10;

		m_apNumber[nCnt]->SetUV(nNumber);
	}
}

//=================================================
// 描画処理
//=================================================
void CScore::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_GAME)
	{
		// 桁数分回す
		for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
		{
			// nullなら処理しない
			if (m_apNumber[nCnt] == nullptr) continue;

			// 描画処理
			m_apNumber[nCnt]->Draw();
		}
	}
}
//===============================================
// 保存
//===============================================
void CScore::Save(const char* pFilename)
{
	FILE* pFile;
	//ファイルに書き込む
	pFile = fopen(pFilename, "w");
	if (pFile != NULL)
	{
		// 今回のスコアを保存
		fprintf(pFile, "%d", m_nScore);
		// 閉じる
		fclose(pFile);
	}
	else
	{// エラーチェック
	}
}
//===============================================
// 読み込み
//===============================================
int CScore::Load(const char* pFilename)
{
	int nScore = 0;

	FILE* pFile;
	// ファイルから既存スコアを読み込む
	pFile = fopen(pFilename, "r");
	if (pFile != NULL)
	{
		// 読み込み
		fscanf(pFile, "%d\n", &nScore);
		// 閉じる
		fclose(pFile);
	}
	else
	{// エラーチェック
	}

	// 値を返す
	return nScore;
}
