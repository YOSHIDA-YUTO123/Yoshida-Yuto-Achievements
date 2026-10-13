//===================================================
//
// オープニングのUIのマネージャー処理 [OpeningUIManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "OpeningUIManager.h"
#include "Opening_UI.h"
#include "input.h"
#include "manager.h"

//***************************************************
// 定数宣言
//***************************************************
namespace OpeningUI_Constants
{
	const D3DXVECTOR3 POS = { 1050.0f,600.0f,0.0f };	// 位置
	const D3DXVECTOR2 SIZE = { 140.0f,70.0f };			// 大きさ
}

//===================================================
// コンストラクタ
//===================================================
COpeningUIManager::COpeningUIManager() : 
	m_apUI(),
	m_CurrentUIType(UITYPE_KEYBOARD)
{
}

//===================================================
// デストラクタ
//===================================================
COpeningUIManager::~COpeningUIManager()
{
}

//===================================================
// 生成処理
//===================================================
COpeningUIManager* COpeningUIManager::Create(void)
{
	COpeningUIManager* pInstance = new COpeningUIManager;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT COpeningUIManager::Init(void)
{
	m_apUI[UITYPE_KEYBOARD] = COpening_UI::Create(OpeningUI_Constants::POS, OpeningUI_Constants::SIZE, "SKIP_Keyboard.png");
	m_apUI[UITYPE_JOYPAD] = COpening_UI::Create(OpeningUI_Constants::POS, OpeningUI_Constants::SIZE, "SKIP_Joypad.png");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void COpeningUIManager::Uninit(void)
{
	for (auto& ui : m_apUI)
	{
		ui = nullptr;
	}

	// 破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void COpeningUIManager::Update(void)
{
	// サイズ分回す
	for (size_t nCnt = 0; nCnt < m_apUI.size();nCnt++)
	{
		// 現在の状態と同じだったら
		const bool bShow = (m_CurrentUIType == nCnt);

		// 表示状態の設定
		m_apUI[nCnt]->SetShow(bShow);
	}

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// キーを何かおしたら
	if (pKeyboard->GetPressAnyKey())
	{
		m_CurrentUIType = UITYPE_KEYBOARD;
	}
	// ボタンを何かおしたら
	else if (pJoypad->GetPressAnyButton())
	{
		m_CurrentUIType = UITYPE_JOYPAD;
	}
}

//===================================================
// 描画処理
//===================================================
void COpeningUIManager::Draw(void)
{
}
