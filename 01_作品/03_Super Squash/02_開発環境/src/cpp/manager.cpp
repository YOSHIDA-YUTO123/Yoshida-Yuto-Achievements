//===================================================
//
// マネージャークラス [Manager.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "input.h"
#include "model_manager.h"
#include "fade.h"
#include "light.h"
#include "world_system_manager.h"
#include "camera.h"
#include "texture_mrt_manager.h"
#include "particle_helper.h"
#include "shader_manager.h"
#include "sound.h"
#include "scene.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ManagerConst
{
	const D3DXVECTOR3 LIGHT_DIR_000 = { 0.000f, -0.637f, -0.495f };	// ライトの方向0
	const D3DXVECTOR3 LIGHT_DIR_001 = { -0.403f, -0.47f, -0.401f };	// ライトの方向1
	const D3DXVECTOR3 LIGHT_DIR_002 = { 0.515f, -0.616f, 1.0f };	// ライトの方向2
	constexpr const char* PARTICLE_LOAD_PATH = "data/SYSTEM/particle_param.json";	// パーティクルの情報のファイルパス
}

//***************************************************
// 静的メンバ変数
//***************************************************
std::unique_ptr<CManager> CManager::m_pInstance = nullptr;			// 自分のインスタンス

//===================================================
// モードの変更
//===================================================
void CManager::ChangeMode(std::unique_ptr<CScene> pNewScene)
{
	// nullなら処理しない
	if (pNewScene == nullptr) return;

	// 現在のシーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene.reset();
	}

	// システムの破棄
	if (m_pWorldSystem != nullptr)
	{
		// システムのクリア
		m_pWorldSystem->Clear();
	}

	// 所有権を移動
	m_pScene = std::move(pNewScene);

	// カメラの破棄
	m_pCamera->Uninit();

	// シーンの初期化処理
	m_pScene->Init();

}

//===================================================
// フェードの開始
//===================================================
void CManager::BeginFade(std::unique_ptr<CScene> pNewScene, const D3DXCOLOR col)
{
	if (m_pFade != nullptr)
	{
		// フェードの開始
		m_pFade->SetFade(std::move(pNewScene), col);
	}
}

//===================================================
// コンストラクタ
//===================================================
CManager::CManager() :
	m_pRenderer(nullptr),
	m_pScene(nullptr),
	m_pTextureManager(nullptr),
	m_pModelManager(nullptr),
	m_pKeyboard(nullptr),
	m_pJoypad(nullptr),
	m_pMouse(nullptr),
	m_pFade(nullptr),
	m_pLight(nullptr),
	m_pWorldSystem(nullptr),
	m_pCamera(nullptr),
	m_pTextureMTManager(nullptr),
	m_pParticleHelper(nullptr),
	m_pShaderManager(nullptr),
	m_pSound(nullptr),
	m_nFps(NULL)
{
}

//===================================================
// デストラクタ
//===================================================
CManager::~CManager()
{
}

//===================================================
// インスタンスの取得
//===================================================
CManager* CManager::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		// 生成
		m_pInstance.reset(new CManager);
	}

	return m_pInstance.get();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWind)
{
	// 乱数の種の設定
	srand(static_cast<unsigned int>(time(nullptr)));

	// 音の生成
	m_pSound = std::make_unique<CSound>();

	// レンダラーの生成
	m_pRenderer = std::make_unique<CRenderer>();

	// キーボードの生成
	m_pKeyboard = std::make_unique<CInputKeyboard>();

	// ジョイパッドの生成
	m_pJoypad = std::make_unique<CInputJoypad>();

	// マウスの生成
	m_pMouse = std::make_unique<CInputMouse>();

	// 音の初期化処理
	if (FAILED(m_pSound->Init(hWnd))) return E_FAIL;

	// キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd))) return E_FAIL;

	// ジョイパッドの初期化処理
	if (FAILED(m_pJoypad->Init())) return E_FAIL;

	// マウスの初期化処理
	if (FAILED(m_pMouse->Init(hWnd))) return E_FAIL;

	// レンダラー初期化処理 
	if (FAILED(m_pRenderer->Init(hWnd, bWind))) return E_FAIL;

	// カメラの生成
	m_pCamera = std::make_unique<CCamera>();
	m_pCamera->Init();

	// ライトの生成
	m_pLight = std::make_unique<CLight>();

	D3DXCOLOR col = { 1.1f, 1.1f, 1.1f,1.0f };

	m_pLight->Init();
	//m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_000, Const::VEC3_NULL);
	m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_001, Const::VEC3_NULL);
	m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_002, Const::VEC3_NULL);

	// テクスチャマネージャーの生成
	m_pTextureManager = std::make_unique<CTextureManager>();
	m_pTextureManager->Load();

	// モデルのマネージャーの生成
	m_pModelManager = std::make_unique<CModelManager>();
	m_pModelManager->Load();

	// マルチレンダーターゲットのテクスチャのマネージャーの生成
	m_pTextureMTManager = std::make_unique<CTextureMRTManager>();
	m_pTextureMTManager->Init();

	// 初期モードの設定
	ChangeMode(std::make_unique<CScene>());

	// フェードの生成
	m_pFade = CFade::Create();

	m_pWorldSystem = std::make_unique<CWorldSystemManager>();

	m_pParticleHelper = std::make_unique<CParticleHelper>();
	m_pParticleHelper->Load(ManagerConst::PARTICLE_LOAD_PATH);

	// シェーダーのマネージャーの生成
	m_pShaderManager = CShaderManager::Create();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CManager::Uninit(void)
{
	// 破棄
	m_pParticleHelper.reset();
	m_pShaderManager.reset();

	// マルチレンダーターゲットのテクスチャのマネージャーの破棄
	if (m_pTextureMTManager != nullptr)
	{
		m_pTextureMTManager->Release();
		m_pTextureMTManager = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		m_pCamera.reset();
	}

	// システムの破棄
	if (m_pWorldSystem != nullptr)
	{
		m_pWorldSystem->Clear();
		m_pWorldSystem.reset();
	}
	// ライトの破棄
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight.reset();
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		m_pRenderer.reset();
	}

	// テクスチャマネージャの破棄
	if (m_pTextureManager != nullptr)
	{
		m_pTextureManager->UnLoad();
		m_pTextureManager.reset();
	}

	// モデルマネージャの破棄
	if (m_pModelManager != nullptr)
	{
		m_pModelManager->UnLoad();
		m_pModelManager.reset();
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene.reset();
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade.reset();
	}

	// キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		m_pKeyboard.reset();
	}

	// パッドの破棄
	if (m_pJoypad != nullptr)
	{
		m_pJoypad->Uninit();
		m_pJoypad.reset();
	}

	// マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();
		m_pMouse.reset();
	}

	// 音の破棄
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		m_pSound.reset();
	}

	// 自分自身の破棄
	if (m_pInstance != nullptr)
	{
		// 破棄
		m_pInstance.reset();
	}
}

//===================================================
// 更新処理
//===================================================
void CManager::Update(void)
{
	// カメラの更新
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	// ライトの更新
	if (m_pLight != nullptr)
	{
		m_pLight->Update();
	}

	// キーボードの更新処理
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// パッドの更新処理
	if (m_pJoypad != nullptr)
	{
		m_pJoypad->Update();
	}

	// マウスの更新処理
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	if (m_pWorldSystem != nullptr)
	{
		// 更新処理
		m_pWorldSystem->Update();
	}

	// レンダラーの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update(m_pScene.get());
	}

	// フェードの更新
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

#ifdef _DEBUG

	// ワイヤーフレームの状態
	static bool bWireFrame = false;

	if (m_pKeyboard->GetTrigger(DIK_F6))
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
	// レンダラーの描画処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw(m_nFps, m_pScene.get(),m_pFade.get());
	}
}

