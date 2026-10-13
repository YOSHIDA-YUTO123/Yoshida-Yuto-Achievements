//===============================================
// 
// ルールジャグリングクラス[ruleJuggling.cpp]
// Author：Rio Ohno
// 
//===============================================

// インクルード
#include "ruleJuggling.h"
#include "manager.h"
#include "input.h"
#include "game.h"
#include "score.h"
#include "sound.h"

// 静的メンバ変数
CNormalTimer* CRuleJug::m_pTime = nullptr;
CObject2D* CRuleJug::m_apObject2D[UI_MAX] = { nullptr };

//===============================================
// コンストラクタ
//===============================================
CRuleJug::CRuleJug() : CRuleBase(CRuleBase::RULETYPE_JUG)
{
	// メンバ変数の初期化
	for (int nCnt = 0; nCnt < NUM_BOTTON; nCnt++)
	{
		m_anBottonList[nCnt] = -1;		// コマンドリスト
	}

	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;	// オブジェクト2Dのポインタ
	}

	m_nCommandIndx = 0;// コマンドのインデックス
}

//===============================================
// デストラクタ
//===============================================
CRuleJug::~CRuleJug()
{
	// なし
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRuleJug::Init(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	//for (int nCnt = 0; nCnt < NUM_BOTTON; nCnt++)
	//{
	//	// コマンドの順番決め
	//	m_anBottonList[nCnt] = rand() % BOTTON_MAX;
	//}

	// タイマーの設定
	m_pTime = CNormalTimer::Create(D3DXVECTOR3(780.0f, 180.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), 10);

	// ポリゴンの生成
	m_apObject2D[UI_BASE] = CObject2D::Create(60.0f, 60.0f, D3DXVECTOR3(760.0f, 280.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apObject2D[UI_Y] = CObject2D::Create(15.0f, 15.0f, D3DXVECTOR3(760.0f, 245.0f, 0.0f));
	m_apObject2D[UI_X] = CObject2D::Create(15.0f, 15.0f, D3DXVECTOR3(725.0f, 280.0f, 0.0f));
	m_apObject2D[UI_A] = CObject2D::Create(15.0f, 15.0f, D3DXVECTOR3(760.0f, 315.0f, 0.0f));
	m_apObject2D[UI_B] = CObject2D::Create(15.0f, 15.0f, D3DXVECTOR3(795.0f, 280.0f, 0.0f));

	// テクスチャの設定
	m_apObject2D[UI_BASE]->SetTextureID("ButtonUI_Juggling_Base.png");
	if (pJoypad->GetLastInput() <= pKeyboard->GetLastPress())
	{
		m_apObject2D[UI_Y]->SetTextureID("ButtonUI_Juggling_JoyPad_Y.png");
		m_apObject2D[UI_X]->SetTextureID("ButtonUI_Juggling_JoyPad_X.png");
		m_apObject2D[UI_A]->SetTextureID("ButtonUI_Juggling_JoyPad_A.png");
		m_apObject2D[UI_B]->SetTextureID("ButtonUI_Juggling_JoyPad_B.png");
	}
	else if (pJoypad->GetLastInput() > pKeyboard->GetLastPress())
	{
		m_apObject2D[UI_Y]->SetTextureID("ButtonUI_Juggling_Keyboard_Up.png");
		m_apObject2D[UI_X]->SetTextureID("ButtonUI_Juggling_Keyboard_Left.png");
		m_apObject2D[UI_A]->SetTextureID("ButtonUI_Juggling_Keyboard_Down.png");
		m_apObject2D[UI_B]->SetTextureID("ButtonUI_Juggling_Keyboard_Right.png");

		// 向きの設定
		m_apObject2D[UI_BASE]->SetRotaition(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	}

	for (int nCnt = 1; nCnt < UI_MAX; nCnt++)
	{
		if (nCnt != (m_nCommandIndx + 1))
		{
			// 色の設定
			m_apObject2D[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			// サイズの設定
			m_apObject2D[nCnt]->SetSize(D3DXVECTOR2(ENLARGED_SIZE, ENLARGED_SIZE));
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRuleJug::Uninit(void)
{
	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			// 終了処理
			m_apObject2D[nCnt]->Uninit();

			m_apObject2D[nCnt] = nullptr;
		}
	}

	if (m_pTime != nullptr)
	{
		// 終了処理
		m_pTime->Uninit();

		m_pTime = nullptr;
	}

	// 自身の破棄
	CRuleBase::Release();
}

//===============================================
// 更新処理
//===============================================
int CRuleJug::Update(void)
{
	// 入力
	CommandInput();

	// ベースの更新
	CRuleBase::Update();

	// 結果取得して返す
	return CRuleBase::GetResult();
}

//===============================================
// コマンド入力
//===============================================
bool CRuleJug::CommandInput(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	bool bSuccess = false;

	// 結果をなしにするにする
	CRuleBase::SetResult(CRuleBase::RESULT_NONE);

	// 音の取得
	CSound* pSound = CManager::GetSound();

	switch (m_nCommandIndx)
	{
	case BOTTON_Y:

		if (pKeyboard->GetTrigger(DIK_UP) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_Y) == true)
		{
			// 成功
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_SUCCESS);

			// 結果を成功にする
			CRuleBase::SetResult(CRuleBase::RESULT_SUCCESS);
			bSuccess = true;
			CGame::GetScore()->AddScore(20000);
		}
		else if (
			pKeyboard->GetTrigger(DIK_DOWN) == true ||
			pKeyboard->GetTrigger(DIK_LEFT) == true ||
			pKeyboard->GetTrigger(DIK_RIGHT) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_B) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_X) == true
			)
		{
			// 失敗
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_FAIL);

			// 結果を失敗にする
			CRuleBase::SetResult(CRuleBase::RESULT_PLAYER_MISS);
			bSuccess = true;
		}

		break;

	case BOTTON_X:

		if (pKeyboard->GetTrigger(DIK_LEFT) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_X) == true)
		{
			// 成功
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_SUCCESS);

			// 結果を成功にする
			CRuleBase::SetResult(CRuleBase::RESULT_SUCCESS);
			bSuccess = true;
			CGame::GetScore()->AddScore(20000);
		}
		else if (
			pKeyboard->GetTrigger(DIK_DOWN) == true ||
			pKeyboard->GetTrigger(DIK_UP) == true ||
			pKeyboard->GetTrigger(DIK_RIGHT) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_B) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_Y) == true
			)
		{
			// 失敗
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_FAIL);

			// 結果を失敗にする
			CRuleBase::SetResult(CRuleBase::RESULT_PLAYER_MISS);
			bSuccess = true;
		}
		break;

	case BOTTON_A:

		if (pKeyboard->GetTrigger(DIK_DOWN) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			// 成功
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_SUCCESS);

			// 結果を成功にする
			CRuleBase::SetResult(CRuleBase::RESULT_SUCCESS);
			bSuccess = true;
			CGame::GetScore()->AddScore(20000);
		}
		else if (
			pKeyboard->GetTrigger(DIK_UP) == true ||
			pKeyboard->GetTrigger(DIK_LEFT) == true ||
			pKeyboard->GetTrigger(DIK_RIGHT) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_Y) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_B) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_X) == true
			)
		{
			// 失敗
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_FAIL);

			// 結果を失敗にする
			CRuleBase::SetResult(CRuleBase::RESULT_PLAYER_MISS);
			bSuccess = true;
		}
		break;

	case BOTTON_B:

		if (pKeyboard->GetTrigger(DIK_RIGHT) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_B) == true)
		{
			// 成功
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_SUCCESS);

			// 結果を成功にする
			CRuleBase::SetResult(CRuleBase::RESULT_SUCCESS);
			bSuccess = true;
			CGame::GetScore()->AddScore(20000);
		}
		else if (
			pKeyboard->GetTrigger(DIK_DOWN) == true ||
			pKeyboard->GetTrigger(DIK_LEFT) == true ||
			pKeyboard->GetTrigger(DIK_UP) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_Y) == true ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_X) == true
			)
		{
			// 失敗
			if (pSound != nullptr) pSound->Play(CSound::SOUND_LABEL_SE_JUGGLING_FAIL);

			// 結果を失敗にする
			CRuleBase::SetResult(CRuleBase::RESULT_PLAYER_MISS);
			bSuccess = true;
		}
		break;
	default:
		break;
	}

	if (bSuccess == true)
	{
		int nOldCommnd = m_nCommandIndx;

		// 同じ数字を避ける
		//for (int nCnt = 0; nCnt < 50; nCnt++)
		for (;;)
		{
			// コマンドの順番決め
			m_nCommandIndx = rand() % BOTTON_MAX;

			if (m_nCommandIndx != nOldCommnd) break;
		}

		// ポリゴンの設定
		for (int nCnt = 1; nCnt < UI_MAX; nCnt++)
		{
			if (nCnt != (m_nCommandIndx + 1))
			{
				// 色の設定
				m_apObject2D[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				// サイズの設定
				m_apObject2D[nCnt]->SetSize(D3DXVECTOR2(15.0f, 15.0f));
			}
			else
			{
				// サイズの設定
				m_apObject2D[nCnt]->SetSize(D3DXVECTOR2(ENLARGED_SIZE, ENLARGED_SIZE));

				// 色の設定
				m_apObject2D[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	return bSuccess;
}