//================================================
//
// タイマーを表示する処理 [timer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "timer.h"
#include "number.h"
#include <cassert>

//*************************************************
// 名前空間
//*************************************************
using namespace Const; // 名前空間Constの使用
using namespace std;   // 名前空間stdの使用

//=================================================
// コンストラクタ
//=================================================
CTimer::CTimer(int nPriority) :CObject(nPriority)
{
	m_nTime = NULL;
	m_pos = VEC3_NULL;
	m_Size = VEC2_NULL;

	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}

//=================================================
// デストラクタ
//=================================================
CTimer::~CTimer()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT CTimer::Init(void)
{
	// 横幅の割合を求める
	float fWidth = m_Size.x / MAX_DIGIT;

	// 基準の位置
	float fOffPosX = fWidth * 2.0f;

	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
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
	}

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CTimer::Uninit(void)
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

	// 自分自身の破棄
	CObject::Release();
}

//=================================================
// 更新処理
//=================================================
void CTimer::Update(void)
{
	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
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
		int nNumber = m_nTime / nDivi % 10;

		m_apNumber[nCnt]->SetUV(nNumber);

		// 制限
		if (m_nTime <= 0)
		{
			m_nTime = 0;
		}
	}
}

//=================================================
// 描画処理
//=================================================
void CTimer::Draw(void)
{
	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// nullなら処理しない
		if (m_apNumber[nCnt] == nullptr) continue;

		// 描画処理
		m_apNumber[nCnt]->Draw();
	}
}

//=================================================
// テクスチャのパスの設定
//=================================================
void CTimer::SetTexture(const char* pTexturePath)
{
	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// nullなら処理しない
		if (m_apNumber[nCnt] == nullptr) continue;

		// テクスチャのパスの設定
		m_apNumber[nCnt]->SetTextureID(pTexturePath);
	}
}

//=================================================
// ナンバークラスの取得
//=================================================
CNumber* CTimer::GetNumber(const int nIdx)
{
	// 超えていたら
	if (nIdx < 0 || nIdx > MAX_DIGIT)
	{
		assert(false && "タイマー配列オーバー");
	}

	return m_apNumber[nIdx].get();
}

//===============================================
// 読み込み
//===============================================
int CTimer::Load(const char* pFilename)
{
	int nTime = 0;

	FILE* pFile;
	// ファイルから既存スコアを読み込む
	pFile = fopen(pFilename, "r");
	if (pFile != NULL)
	{
		// 読み込み
		fscanf(pFile, "%d\n", &nTime);
		// 閉じる
		fclose(pFile);
	}
	else
	{// エラーチェック
	}

	// 値を返す
	return nTime;
}