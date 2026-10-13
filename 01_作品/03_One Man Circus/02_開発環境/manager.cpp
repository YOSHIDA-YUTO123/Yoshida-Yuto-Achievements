//===================================================
//
// マネージャー [manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "manager.h"
#include "renderer.h"
#include "object3D.h"
#include <ctime>
#include "Collision.h"
#include "sound.h"
#include "input.h"
#include "textureManager.h"
#include "modelManager.h"
#include "light.h"
#include "scene.h"
#include "object.h"
#include "fade.h"
#include "pause.h"
#include "CharacterManager.h"
#include "slow.h"
#include "camera.h"
#include "title.h"
#include "game.h"

//***************************************************
// 静的メンバ変数の宣言
//***************************************************
std::unique_ptr<CRenderer> CManager::m_pRenderer = nullptr;				// レンダラーへのポインタ
std::unique_ptr<CInputKeyboard> CManager::m_pInputKeyboard = nullptr;	// キーボードへのポインタ
std::unique_ptr<CInputJoypad> CManager::m_pInputJoypad = nullptr;		// パッドへのポインタ
std::unique_ptr<CInputMouse> CManager::m_pInputMouse = nullptr;			// マウスへのポインタ
std::unique_ptr<CSound> CManager::m_pSound = nullptr;					// サウンドのポインタ
std::unique_ptr<CLight> CManager::m_pLight = nullptr;					// ライトクラスへのポインタ
std::unique_ptr<CScene> CManager::m_pScene = nullptr;					// シーンクラスへのポインタ
std::unique_ptr<CSlow> CManager::m_pSlow = nullptr;						// スローモーションクラスへのポインタ
std::unique_ptr<CFade> CManager::m_pFade = nullptr;						// フェードクラスへのポインタ
std::unique_ptr<CCamera> CManager::m_pCamera = nullptr;					// カメラクラスへのポインタ
std::unique_ptr<CModelManager> CManager::m_pModelManager = nullptr;		// モデルマネージャーへのポインタ
std::unique_ptr<CTextureManager> CManager::m_pTextureManager = nullptr; // テクスチャマネージャーへのポインタ

int CManager::m_nFrameCounter = 0;									// フレームのカウンター
bool CManager::m_bShowDebug = true;									// デバッグ表示をするかしないか

//===================================================
// コンストラクタ
//===================================================
CManager::CManager()
{
	m_fps = 0;
}
//===================================================
// デストラクタ
//===================================================
CManager::~CManager()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	// 乱数の種の設定
	srand(static_cast<unsigned int>(time(nullptr)));

	// 名前空間stdの使用
	using namespace std;

	// レンダラーを生成
	m_pRenderer = make_unique<CRenderer>();

	// サウンドの初期化処理
	m_pSound = make_unique<CSound>();

	// キーボードを生成
	m_pInputKeyboard = make_unique<CInputKeyboard>();

	// パッドを生成
	m_pInputJoypad = make_unique<CInputJoypad>();

	// マウスを生成
	m_pInputMouse = make_unique<CInputMouse>();

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow))) return E_FAIL;

	// サウンドの初期化処理
	if (FAILED(m_pSound->Init(hWnd))) return E_FAIL;

	// キーボードの初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd))) return E_FAIL;

	// パッドの初期化処理
	if (FAILED(m_pInputJoypad->Init())) return E_FAIL;

	// マウスの初期化処理
	if (FAILED(m_pInputMouse->Init(hWnd))) return E_FAIL;

	// キャラクターマネージャーの生成
	CCharacterManager::Create();

	// テクスチャマネージャーの生成
	m_pTextureManager = make_unique<CTextureManager>();
	m_pTextureManager->Load();

	// すべてのモデルのロード処理
	m_pModelManager = make_unique<CModelManager>();
	m_pModelManager->Load();

	// カメラの生成
	m_pCamera = make_unique<CCamera>();
	m_pCamera->Init();
	m_pCamera->SetCamera(D3DXVECTOR3(0.0f, 650.0f, 0.0f), Const::VEC3_NULL, D3DXVECTOR3(D3DX_PI * 0.80f, 0.0f, 0.0f));

	// ライトの生成
	m_pLight = make_unique<CLight>();
	m_pLight->Init();

	// フェードの生成
	m_pFade.reset(CFade::Create());
	SetMode(make_unique<CTitle>());

	// スローの生成
	m_pSlow = make_unique<CSlow>();

	//m_pLight->SetDirectional(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-0.52f, -0.59f, 0.62f), { 0.0f,0.0f,0.0f });

#ifdef _DEBUG

#else
	// カーソルを非表示にする
	ShowCursor(false);
#endif // _DEBUG

	// 結果を返す
	return S_OK;
}
//===================================================
// 終了処理
//===================================================
void CManager::Uninit(void)
{
	// すべてのサウンドの停止
	m_pSound->StopSound();

	// モデルの破棄
	if (m_pModelManager != nullptr)
	{
		m_pModelManager->UnLoad();
		m_pModelManager.reset();
	}

	// テクスチャの破棄
	if (m_pTextureManager != nullptr)
	{
		m_pTextureManager->UnLoad();
		m_pTextureManager.reset();
	}

	//// カメラの破棄
	//if (m_pCamera != nullptr)
	//{
	//	// 終了処理
	//	m_pCamera->Uninit();
	//}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	// サウンドの破棄
	if (m_pSound != nullptr)
	{
		// キーボードの終了処理
		m_pSound->Uninit();
	}

	// キーボードの破棄
	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
	}

	// パッドの破棄
	if (m_pInputJoypad != nullptr)
	{
		// パッドの終了処理
		m_pInputJoypad->Uninit();
	}

	// パッドの破棄
	if (m_pInputMouse != nullptr)
	{
		// マウスの終了処理
		m_pInputMouse->Uninit();
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		// オブジェクトの終了処理
		m_pRenderer->Uninit();
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
	}
}
//===================================================
// 更新処理
//===================================================
void CManager::Update(void)
{
	// フレームのカウンターを増やす
	m_nFrameCounter++;

	// 1フレーム経過したら
	if (m_nFrameCounter > Const::FRAME)
	{
		// カウンターをリセット
		m_nFrameCounter = 0;
	}

	if (m_pFade != nullptr)
	{
		// フェードの更新処理
		m_pFade->Update();
	}

	if (m_pRenderer != nullptr)
	{
		// 更新処理
		m_pRenderer->Update();
	}
	
	if (m_pScene != nullptr)
	{
		// シーンの更新処理
		m_pScene->Update();
	}

	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの更新処理
		m_pInputKeyboard->Update();
	}

	if (m_pInputJoypad != nullptr)
	{
		// パッドの更新処理
		m_pInputJoypad->Update();
	}

	if (m_pInputMouse != nullptr)
	{
		// マウスの更新処理
		m_pInputMouse->Update();
	}

	if (m_pCamera != nullptr)
	{
		// カメラの更新処理
		m_pCamera->Update();
	}

	if (m_pLight != nullptr)
	{	// ライトの更新処理
		m_pLight->Update();
	}

	if (m_pSlow != nullptr)
	{
		m_pSlow->Update();
	}

#ifdef _DEBUG

	// ワイヤーフレームの状態
	static bool bWireFrame = false;

	if (m_pInputKeyboard->GetTrigger(DIK_F6))
	{
		bWireFrame = bWireFrame ? false : true;
	}

	if (bWireFrame == true)
	{
		m_pRenderer->onWireFrame();
	}
	else
	{
		m_pRenderer->offWireFrame();
	}

#endif // _DEBUG
}
//===================================================
// 描画処理
//===================================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{
		// 描画処理
		m_pRenderer->Draw(m_fps);
	}
}

//===================================================
// フレームのカウンター
//===================================================
int CManager::GetFrameCounter(void)
{
	// 現在のフレームの取得
	return m_nFrameCounter;
}

//===================================================
// シーンの設定処理
//===================================================
void CManager::SetMode(std::unique_ptr<CScene> pNewScene)
{
	// 今のシーンの破棄
	if (m_pScene != nullptr)
	{
		// 終了処理
		m_pScene->Uninit();
		m_pScene.reset();
		m_pScene = nullptr;

		m_pSound->StopSound();

		// すべてのオブジェクトの破棄
		CObject::ReleaseAll();
	}
	
	// しっかり破棄できているなら
	if (m_pScene == nullptr)
	{
		// 新しいシーンを設定
		m_pScene = std::move(pNewScene);

		// シーンの初期化処理
		m_pScene->Init();
	}
}

//===================================================
// シーンの切替処理
//===================================================
void CManager::SetFade(std::unique_ptr<CScene> pNewScene)
{
	if (m_pFade != nullptr)
	{
		m_pFade->SetFade(move(pNewScene));
	}
}

//===================================================
// シーンの描画
//===================================================
void CManager::DrawScene(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//===================================================
// カメラの変更
//===================================================
CCamera* CManager::ChangeCamera(std::unique_ptr<CCamera> pNewCamera)
{
	// 現在のカメラの破棄
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		m_pCamera.reset();
		m_pCamera = nullptr;
	}

	if (pNewCamera == nullptr)
	{
		return nullptr;
	}

	// 新しいカメラの生成
	m_pCamera = std::move(pNewCamera);

	// 初期化処理
	m_pCamera->Init();

	return m_pCamera.get();
}
