//===================================================
//
// ゲーム、ミニゲームの全体の操作をするクラス [GameController.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "game_controller.h"
#include "hitstop_controller.h"
#include "camera_shake.h"
#include "input.h"
#include "manager.h"
#include "hitstop.h"

//***************************************************
// 静的メンバ変数宣言
//***************************************************
std::unique_ptr<CGameController> CGameController::m_pInstance = nullptr; // 自分のインスタンス

//===================================================
// デストラクタ
//===================================================
CGameController::~CGameController()
{

}

//===================================================
// 生成処理
//===================================================
void CGameController::Create(void)
{
	if (m_pInstance == nullptr)
	{
		// 生成処理
		m_pInstance.reset(new CGameController);
	}
}

//===================================================
// 取得処理
//===================================================
CGameController* CGameController::GetInstance(void)
{
	return m_pInstance.get();
}

//===================================================
// ヒットストっプを操作するクラスの取得
//===================================================
CHitStopController* CGameController::GetHitStop(void)
{
	if (m_pHitStop == nullptr) return nullptr;

	// 操作クラスの取得
	CHitStopController* pController = m_pHitStop->GetController();

	return pController;
}

//===================================================
// 終了処理
//===================================================
void CGameController::Uninit(void)
{
	m_pHitStop = nullptr;
	m_pInstance.reset();
}

//===================================================
// 更新処理
//===================================================
void CGameController::Update(void)
{
}

//===================================================
// ポーズの設定処理
//===================================================
void CGameController::SetPause(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// ポーズの切り替え
	if (pKeyboard->GetTrigger(DIK_P) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_START))
	{
		m_bPause = m_bPause ? false : true;
	}
}

//===================================================
// コンストラクタ
//===================================================
CGameController::CGameController() : 
	m_pHitStop(nullptr),
	m_pCameraShack(nullptr),
	m_bPause(false)
{
}