//===================================================
//
// ゲームに関わる処理の補助クラスのマネージャー [world_system_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "world_system.h"
#include "world_system_manager.h"

//===================================================
// コンストラクタ
//===================================================
CWorldSystemManager::CWorldSystemManager() :
	m_pSystem()
{
}

//===================================================
// デストラクタ
//===================================================
CWorldSystemManager::~CWorldSystemManager()
{
}

//===================================================
// 登録処理
//===================================================
void CWorldSystemManager::Register(std::unique_ptr<CWorldSystem> pNewSystem)
{
	// 追加されていない要素だったら
	if (std::find(m_pSystem.begin(), m_pSystem.end(), pNewSystem) == m_pSystem.end())
	{
		m_pSystem.push_back(std::move(pNewSystem));

		m_pSystem.back()->Init();
	}
}

//===================================================
// 消去処理
//===================================================
void CWorldSystemManager::UnRegister(CWorldSystem* pNewSystem)
{
	// 要素を調べる
	for (auto itr = m_pSystem.begin(); itr != m_pSystem.end(); itr++)
	{
		if ((*itr).get() == pNewSystem)
		{
			(*itr).reset();
			itr = m_pSystem.erase(itr);

			break;
		}
	}
}

//===================================================
// リストのクリア
//===================================================
void CWorldSystemManager::Clear(void)
{
	// システムの破棄
	for (auto& systems : m_pSystem)
	{
		// 終了処理
		systems->Uninit();
		systems.reset();
	}
	m_pSystem.clear();
}

//===================================================
// 更新処理
//===================================================
void CWorldSystemManager::Update(void)
{
	// 更新処理
	for (auto& systems : m_pSystem)
	{
		// nullだったら処理しない
		if (systems == nullptr) continue;

		// 更新処理
		systems->Update();
	}
}

//===================================================
// 描画処理
//===================================================
void CWorldSystemManager::Draw(void)
{
	// 描画処理
	for (auto& systems : m_pSystem)
	{
		// nullだったら処理しない
		if (systems == nullptr) continue;

		// 描画処理
		systems->Draw();
	}
}
