//===================================================
//
// リザルト失敗のメニューのマネージャークラス [ResultLoseMenuManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ResultLoseMenuManager.h"
#include "ResultLoseCommand.h"
#include "MenuItem.h"
#include "input.h"
#include "manager.h"
#include "math.h"
#include "MenuVisual.h"
#include "sound.h"

//===================================================
// コンストラクタ
//===================================================
CResultLoseMenuManager::CResultLoseMenuManager() : 
	m_nCurrentIndex(0)
{
}

//===================================================
// デストラクタ
//===================================================
CResultLoseMenuManager::~CResultLoseMenuManager()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CResultLoseMenuManager::Init(void)
{
	MenuEntry::Menu menu;

	menu.pMenuCommand = std::make_unique<CResultLoseRetryCommand>();
	menu.pMenuItem = CMenuItem::Create({ 440.0f,500.0f,0.0f }, { 150.0f,80.0f }, Const::WHITE, "Pause/Retry.png", std::make_unique<CMenuHighLightFlashVisual>());

	// リストの追加
	CMenuManager::AddList(menu);

	menu.pMenuCommand = std::make_unique<CResultLoseQuitCommand>();
	menu.pMenuItem = CMenuItem::Create({ 1000.0f,500.0f,0.0f }, { 150.0f,80.0f }, Const::WHITE, "Pause/EXIT.png", std::make_unique<CMenuHighLightFlashVisual>());

	// リストの追加
	CMenuManager::AddList(menu);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CResultLoseMenuManager::Uninit(void)
{
	// 終了処理
	CMenuManager::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CResultLoseMenuManager::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInputJoypad();

	// メニューのリストの取得
	auto& Menu_List = CMenuManager::GetList();

	if (pKeyboard->GetTrigger(DIK_LEFT) || pKeyboard->GetTrigger(DIK_A) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_LEFT))
	{
		// 音の取得
		CSound* pSound = CManager::GetSound();

		if (pSound != nullptr)
		{
			// 音の再生
			pSound->Play(CSound::SOUND_LABEL_SE_TITLE_SELECT);
		}

		m_nCurrentIndex++;
	}
	else if (pKeyboard->GetTrigger(DIK_RIGHT) || pKeyboard->GetTrigger(DIK_D) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_RIGHT))
	{
		// 音の取得
		CSound* pSound = CManager::GetSound();

		if (pSound != nullptr)
		{
			// 音の再生
			pSound->Play(CSound::SOUND_LABEL_SE_TITLE_SELECT);
		}

		m_nCurrentIndex--;
	}

	// 範囲内をループする
	m_nCurrentIndex = math::Wrap(m_nCurrentIndex, 0, static_cast<int>(Menu_List.size()) - 1);

	// 現在のサイズ分回す
	for (int nCnt = 0; nCnt < static_cast<int>(Menu_List.size()); nCnt++)
	{
		// 選択されているかどうか
		const bool bSelect = (nCnt == m_nCurrentIndex);

		// 選択状態を設定
		Menu_List[nCnt].pMenuItem->SetSelect(bSelect);
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
	{
		// コマンドの実行
		Menu_List[m_nCurrentIndex].pMenuCommand->Execute();
	}
}

//===================================================
// 描画処理
//===================================================
void CResultLoseMenuManager::Draw(void)
{

}
