//===================================================
//
// タイトルUI [TitleUI.cpp]
// Author:TAKUTO SAEKI
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "TitleUI.h"
#include "manager.h"
#include"input.h"
#include"math.h"
#include"fade.h"
#include "game.h"
#include<memory>
//#include"game.h"
#include "sound.h"
#include "OpeningScene.h"

using namespace math;  // 名前空間mathを使用
using namespace Const; // 名前空間Constを使用
using namespace std; // 名前空間stdを使用

//***************************************************
// 静的メンバ変数宣言
//***************************************************
CTitleUIManager* CTitleUIManager::m_pInstance = nullptr; // 自分のインスタンス

//===================================================
// コンストラクタ
//===================================================
CTitleUI::CTitleUI(const UI menu)
{
	m_col = WHITE;
	m_fCounter = NULL;
	m_UI = menu;
}

//===================================================
// デストラクタ
//===================================================
CTitleUI::~CTitleUI()
{
}

//===================================================
// 生成処理
//===================================================
CTitleUI* CTitleUI::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const UI menu)
{
	// メニューの生成
	CTitleUI* pUI = nullptr;

	switch (menu)
	{
	case UI_START:
		pUI = new CTitleStart;
		break;
	case UI_QUIT:
		pUI = new CTitleQuit;
		break;
	case UI_SELECT:
		pUI = new CTitleSelect;
		break;

	default:
		return nullptr;
	}

	// 初期化に失敗したら
	if (FAILED(pUI->Init()))
	{
		pUI->Uninit();
		pUI = nullptr;
		return nullptr;
	}

	pUI->SetPosition(pos);
	pUI->SetSize(Size);
	pUI->m_BaseSize = Size;

	return pUI;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitleUI::Init(void)
{
	// 初期化処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitleUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CTitleUI::Update(void)
{
	// タイトルマネージャーの取得
	auto pTitleManager = CTitleUIManager::GetInstance();

	// 取得できなかったら処理しない
	if (pTitleManager == nullptr) return;

	// 大きさの取得
	D3DXVECTOR2 Size = CObject2D::GetSize();

	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	// カウンターを初期化して加算
	if (m_fCounter == NULL) m_fCounter = 0.0f;
	m_fCounter += 0.03f;

	// 現在選択されているメニュー
	CTitleUI::UI currentUI = pTitleManager->GetUI();

	// 選択中のメニューなら既存の拡大＋点滅挙動
	if (m_UI == currentUI)
	{
		D3DXVECTOR2 destSize = m_BaseSize * 1.3f; // 拡大する

		Size += (destSize - Size) * 0.1f;

		// 大きさの設定
		CObject2D::SetSize(Size);

		// 透明度の変更（点滅）
		m_col.a = 1.0f - fabsf(sinf(m_fCounter) * 0.5f);

		// 位置の設定（カーソル位置を更新）
		pTitleManager->SetPosition(D3DXVECTOR3(pos.x - Size.x * 1.3f, pos.y, pos.z));
	}
	// UI_SELECT は常に単体で点滅させる（拡大・カーソル移動はしない）
	else if (m_UI == UI_SELECT)
	{
		// サイズはベースに補正（拡大はしない）
		Size += (m_BaseSize - Size) * 0.1f;
		CObject2D::SetSize(Size);

		// 透明度の変更（点滅）
		m_col.a = 1.0f - fabsf(sinf(m_fCounter) * 0.5f);
	}
	else
	{
		// 非選択時のリセット
		m_col.a = 1.0f;

		m_fCounter = NULL;

		Size += (m_BaseSize - Size) * 0.1f;

		// 大きさの設定
		CObject2D::SetSize(Size);
	}

	// 色の設定
	CObject2D::SetColor(m_col);

	// 頂点座標の更新
	pTitleManager->UpdateVertex();
	CObject2D::UpdateVertex();
}
//===================================================
// 描画処理
//===================================================
void CTitleUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//===================================================
// コンストラクタ
//===================================================
CTitleStart::CTitleStart() : CTitleUI(UI_START)
{
}

//===================================================
// デストラクタ
//===================================================
CTitleStart::~CTitleStart()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitleStart::Init(void)
{
	// 初期化処理
	if (FAILED(CTitleUI::Init()))
	{
		return E_FAIL;
	}
	
	m_fCounterStart = NULL;

	// テクスチャのIDの設定
	SetTextureID("TITLE/START.png");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitleStart::Uninit(void)
{
	// 終了処理
	CTitleUI::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CTitleStart::Update(void)
{
	// 更新処理
	CTitleUI::Update();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// タイトルマネージャーの取得
	auto pTitleManager = CTitleUIManager::GetInstance();

	// 取得できなかったら処理しない
	if (pTitleManager == nullptr) return;

	// 自分のメニュー
	UI myUI = GetUI();

	// 選択中のメニュー
	UI selectUI = pTitleManager->GetUI();

	// フェードの取得
	CFade* pFade = CManager::GetFade();

	// 選択されているメニューと自分のメニューが同じだったら
	if (myUI == selectUI && pFade != nullptr && pFade->GetState() == CFade::FADE_NONE)
	{
		// 押下トリガーを検出したら、まだ Start フラグが立っていない場合のみ処理する
		if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A) || pKeyboard->GetTrigger(DIK_SPACE))
		{
			// 既に Start が true なら音や二重セットを防ぐ
			if (!pTitleManager->CheckStart())
			{
				// 音の取得
				CSound* pSound = CManager::GetSound();

				if (pSound != nullptr)
				{
					// 音の再生（最初の一回のみ）
					pSound->Play(CSound::SOUND_LABEL_SE_START);
				}

				// スタートを押した => マネージャのフラグを立てる（以降毎フレームカウントする）
				pTitleManager->SetStart(true);

				// カウンター初期化（ここで確実に初期化）
				m_fCounterStart = 0.0f;
			}
		}
	}

	// スタートフラグが立っている間は毎フレームカウント（1フレームごとに1加算）
	if (pTitleManager->CheckStart())
	{
		// フェード開始済みでなければカウントし、新シーンへ切り替え
		if (pFade != nullptr && pFade->GetState() == CFade::FADE_NONE)
		{
			m_fCounterStart += 1.0f; // フレーム単位のカウント

			// 60fpsで2秒  60 * 2 フレーム
			if (m_fCounterStart >= 60.0f * 2.0f)
			{
				// 新しいモードの設定
				pFade->SetFade(std::make_unique<COpeningScene>(), 40, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			}
		}
	}
}
//===================================================
// 描画処理
//===================================================
void CTitleStart::Draw(void)
{
	// 描画処理
	CTitleUI::Draw();
}

//===================================================
// コンストラクタ
//===================================================
CTitleQuit::CTitleQuit() : CTitleUI(UI_QUIT)
{
}

//===================================================
// デストラクタ
//===================================================
CTitleQuit::~CTitleQuit()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitleQuit::Init(void)
{
	// 初期化処理
	if (FAILED(CTitleUI::Init()))
	{
		return E_FAIL;
	}

	// テクスチャのIDの設定
	SetTextureID("TITLE/EXIT.png");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitleQuit::Uninit(void)
{
	// 終了処理
	CTitleUI::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CTitleQuit::Update(void)
{
	// 更新処理
	CTitleUI::Update();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// タイトルマネージャーの取得
	auto pTitleManager = CTitleUIManager::GetInstance();

	// 取得できなかったら処理しない
	if (pTitleManager == nullptr) return;

	// 自分のメニュー
	UI myUI = GetUI();

	// 選択中のメニュー
	UI selectUI = pTitleManager->GetUI();

	// 選択されているメニューと自分のメニューが同じだったら
	if (myUI == selectUI)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A)|| pKeyboard->GetTrigger(DIK_SPACE))
		{
			// 終了
			PostQuitMessage(0);
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CTitleQuit::Draw(void)
{
	// 描画処理
	CTitleUI::Draw();
}

//===================================================
// コンストラクタ
//===================================================
CTitleSelect::CTitleSelect() : CTitleUI(UI_SELECT)
{
}

//===================================================
// デストラクタ
//===================================================
CTitleSelect::~CTitleSelect()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitleSelect::Init(void)
{
	// 初期化処理
	if (FAILED(CTitleUI::Init()))
	{
		return E_FAIL;
	}

	// テクスチャのIDの設定
	SetTextureID("TITLE/SELECT.png");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitleSelect::Uninit(void)
{
	// 終了処理
	CTitleUI::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CTitleSelect::Update(void)
{
	// 更新処理
	CTitleUI::Update();

}

//===================================================
// 描画処理
//===================================================
void CTitleSelect::Draw(void)
{
	// 描画処理
	CTitleUI::Draw();
}


//===================================================
// コンストラクタ
//===================================================
CTitleUIManager::CTitleUIManager()
{
	m_UI = CTitleUI::UI_START;
	m_bStart = false;
}

//===================================================
// デストラクタ
//===================================================
CTitleUIManager::~CTitleUIManager()
{
}

//===================================================
// 生成処理
//===================================================
void CTitleUIManager::Create(void)
{
	// 自分がnullだったら
	if (m_pInstance == nullptr)
	{
		//CObject2D* pTitle = CObject2D::Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, Const::CENTER_POS_2D);
		//pTitle->SetTextureID("TITLE/Title.png");

		// 自分自身の生成
		m_pInstance = new CTitleUIManager;
		m_pInstance->Init();
		m_pInstance->SetPosition(D3DXVECTOR3(150.0f, 400.0f, 0.0f));
		m_pInstance->SetSize(D3DXVECTOR2(40.0f, 30.0f));

		// タイトルの選択メニュー分
		for (int nCnt = CTitleUI::UI_START; nCnt <= CTitleUI::UI_QUIT; nCnt++)
		{
			// メニュー型にキャストする
			CTitleUI::UI menu = static_cast<CTitleUI::UI>(nCnt);

			// タイトルのメニューの生成
			CTitleUI::Create(D3DXVECTOR3(1185.0f, 550.0f + nCnt * 100.0f, 0.0f), D3DXVECTOR2(120.0f, 35.0f), menu);
		}

		// メニュー型にキャストする
		CTitleUI::UI menu = static_cast<CTitleUI::UI>(CTitleUI::UI_SELECT);

		// タイトルのメニューの生成
		CTitleUI::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f), D3DXVECTOR2(1280.0f*0.23f, 296.0f*0.23f), menu);

	}
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitleUIManager::Init(void)
{
	// 初期化処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	// テクスチャのIDの登録
	SetTextureID("TITLE/TITLE_CURSOR.png");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitleUIManager::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();

	m_pInstance = nullptr;
}

//===================================================
// 更新処理
//===================================================
void CTitleUIManager::Update(void)
{

	if (m_bStart)
	{
		return;
	}

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// 音の取得
	CSound* pSound = CManager::GetSound();

	if (pKeyboard->GetTrigger(DIK_DOWN) || pKeyboard->GetTrigger(DIK_S) || pJoypad->GetTrigger(pJoypad->JOYKEY_DOWN))
	{
		if (pSound != nullptr)
		{
			// 音の再生
			pSound->Play(CSound::SOUND_LABEL_SE_TITLE_SELECT);
		}

		// 次のメニューへ
		m_UI = static_cast<CTitleUI::UI>(m_UI + 1);
	}
	else if (pKeyboard->GetTrigger(DIK_UP) || pKeyboard->GetTrigger(DIK_W) || pJoypad->GetTrigger(pJoypad->JOYKEY_UP))
	{
		if (pSound != nullptr)
		{
			// 音の再生
			pSound->Play(CSound::SOUND_LABEL_SE_TITLE_SELECT);
		}

		// 次のメニューへ
		m_UI = static_cast<CTitleUI::UI>(m_UI - 1);
	}

	// 範囲の制限
	m_UI = Wrap(m_UI, CTitleUI::UI_START, CTitleUI::UI_QUIT);
}

//===================================================
// 描画処理
//===================================================
void CTitleUIManager::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}