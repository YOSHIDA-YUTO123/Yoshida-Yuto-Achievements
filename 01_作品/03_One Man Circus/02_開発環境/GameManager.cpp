//================================================
//
// ゲームのマネージャークラス [GameManager.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "GameManager.h"
#include "manager.h"
#include "fade.h"
#include "result.h"
#include <fstream>
#include "LastTimer.h"

//=================================================
// コンストラクタ
//=================================================
CGameManager::CGameManager() : 
	m_nScore(NULL),
	m_nTime(NULL),
	m_bSetLastTimer(false)
{
}

//=================================================
// デストラクタ
//=================================================
CGameManager::~CGameManager()
{
}

//=================================================
// スコアのセーブ処理
//=================================================
void CGameManager::SaveScore(const char* pFilePath, const int nScore)
{
	// ファイルを開く
	std::ofstream file(pFilePath);

	if (file.is_open() == false)
	{
		MessageBox(NULL, "ファイルが開けません(CGameManager)", pFilePath, MB_OK);
		return;
	}

	// ファイルに保存
	file << nScore;

	// ファイルをクリア
	file.clear();
	file.close();
}

//=================================================
// タイムのセーブ処理
//=================================================
void CGameManager::SaveTime(const char* pFilePath, const int nTime)
{
	// ファイルを開く
	std::ofstream file(pFilePath);

	if (file.is_open() == false)
	{
		MessageBox(NULL, "ファイルが開けません(CGameManager)", pFilePath, MB_OK);
		return;
	}

	// ファイルに保存
	file << nTime;

	// ファイルをクリア
	file.clear();
	file.close();
}

//=================================================
// 更新処理
//=================================================
void CGameManager::Update(void)
{
	if (m_nTime <= 20 && !m_bSetLastTimer)
	{
		// 制限時間目前のタイマー
		CLastTimer::Create({ 300.0f,300.0f }, 20);

		m_bSetLastTimer = true;
	}

	// ゲームのタイマーが0以下になったら
	if (m_nTime <= 0)
	{
		// ゲームシーンへ移行
		CManager::GetFade()->SetFade(std::make_unique<CResultLose>());
	}
}
