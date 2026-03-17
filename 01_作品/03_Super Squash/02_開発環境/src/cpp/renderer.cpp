//===================================================
//
// DirectXの描画処理 [renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "debug_proc.h"
#include "fade.h"
#include "world_system_manager.h"
#include "screen_polygon.h"
#include "texture_mrt_manager.h"
#include "shader_manager.h"

//===================================================
// コンストラクタ
//===================================================
CRenderer::CRenderer() : 
	m_apScreenPolygon(),
	m_pDebugProc(nullptr),
	m_pD3D(nullptr),
	m_pD3DDevice(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CRenderer::~CRenderer()
{

}

//===================================================
// 初期化処理
//===================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;		 // ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; // プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// バックバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24bit,ステンシルバッファ8bit2
	d3dpp.Windowed = bWindow;										// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成描画処理と頂点処理をハードウェアで行う
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// デバッグ情報の生成
	m_pDebugProc = std::make_unique<CDebugProc>();

	// 初期化処理
	if (FAILED(m_pDebugProc->Init()))
	{
		return E_FAIL;
	}

	m_apScreenPolygon[SCREEN_POLYGON_SCENE] = CScreenPolygon::Create(CTextureMRTManager::TYPE_SCENE, CTextureMRTManager::TYPE_GAUSSIAN_Y, CShaderManager::TYPE_BLOOM);

	m_apScreenPolygon[SCREEN_POLYGON_LUMINANCE] = CScreenPolygon::Create(CTextureMRTManager::TYPE_SCENE, CTextureMRTManager::INVALID, CShaderManager::TYPE_LUMINANCE);

	m_apScreenPolygon[SCREEN_POLYGON_GAUSBLUR_X] = CScreenPolygon::Create(CTextureMRTManager::TYPE_LUMINANCE, CTextureMRTManager::INVALID, CShaderManager::TYPE_GAUS_BLUR);
	m_apScreenPolygon[SCREEN_POLYGON_GAUSBLUR_Y] = CScreenPolygon::Create(CTextureMRTManager::TYPE_GAUSSIAN_X, CTextureMRTManager::INVALID, CShaderManager::TYPE_GAUS_BLUR);

	m_apScreenPolygon[SCREEN_POLYGON_SHADOW_MAP] = CScreenPolygon::Create(CTextureMRTManager::TYPE_SHADOW_MAP, CTextureMRTManager::INVALID, CShaderManager::INVALID);

	return S_OK;
}
//===================================================
// 終了処理
//===================================================
void CRenderer::Uninit(void)
{
	// 画面のポリゴンの破棄
	for (auto& polygon : m_apScreenPolygon)
	{
		polygon->Uninit();
		polygon.reset();
	}

	// Drectxデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Drectxデバイスの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	// デバッグ情報の破棄
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		m_pDebugProc = nullptr;
	}
}

//===================================================
// 更新処理
//===================================================
void CRenderer::Update(CScene* pScene)
{
	if (pScene != nullptr)
	{
		// 更新処理
		pScene->Update();
	}
}

//===================================================
// 描画処理
//===================================================
void CRenderer::Draw(const int fps, CScene* pScene, CFade* pFade)
{
	// 画面クリア(バックバッファ&Zバッファのクリア)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		Const::BACK_BUFFER_COLOR, 1.0f, 0);

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// MRTテクスチャの描画処理
	CTextureMRTManager* pTextureMTManager = pManager->GetTextureMRTManager();

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合
				
		// レンダーターゲットの変更
		pTextureMTManager->ChangeRenderTarget(CTextureMRTManager::TYPE_SCENE);

		if (pScene != nullptr)
		{
			// 描画処理
			pScene->Draw();
			pScene->SetFps(fps);
		}

		// 描画処理
		CManager::GetInstance()->GetWorldSystemManager()->Draw();

		// レンダーターゲットのリセット
		pTextureMTManager->ResetRenderTarget();

		if (pScene != nullptr)
		{
			// MRTの描画処理
			pScene->DrawMRT();
		}

		// レンダーターゲットの変更
		pTextureMTManager->ChangeRenderTarget(CTextureMRTManager::TYPE_LUMINANCE);

		// 描画処理
		m_apScreenPolygon[SCREEN_POLYGON_LUMINANCE]->Draw();

		// レンダーターゲットのリセット
		pTextureMTManager->ResetRenderTarget();

		{
			// レンダーターゲットの変更
			pTextureMTManager->ChangeRenderTarget(CTextureMRTManager::TYPE_GAUSSIAN_X);

			// 描画処理
			m_apScreenPolygon[SCREEN_POLYGON_GAUSBLUR_X]->Draw();

			// レンダーターゲットのリセット
			pTextureMTManager->ResetRenderTarget();

			// レンダーターゲットの変更
			pTextureMTManager->ChangeRenderTarget(CTextureMRTManager::TYPE_GAUSSIAN_Y);

			// 描画処理
			m_apScreenPolygon[SCREEN_POLYGON_GAUSBLUR_Y]->Draw(1);

			// レンダーターゲットのリセット
			pTextureMTManager->ResetRenderTarget();
		}

		// 画面のポリゴンの描画
		m_apScreenPolygon[SCREEN_POLYGON_SCENE]->Draw();

		// 画面のポリゴンの描画
		//m_apScreenPolygon[SCREEN_POLYGON_SHADOW_MAP]->Draw();

		if (pScene != nullptr)
		{
			// ポストプロセスを適応しない描画
			pScene->DrawPostProcessExcluded();
		}

		if (pFade != nullptr)
		{
			// フェードの描画処理
			pFade->Draw();
		}

		// デバッグ情報の描画
		CDebugProc::Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===================================================
// デバイスの取得処理
//===================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	// デバイスのポインタを返す
	return m_pD3DDevice;
}

//===================================================
// ワイヤーフレームオン
//===================================================
void CRenderer::onWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

//===================================================
// ワイヤーフレームオフ
//===================================================
void CRenderer::offWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}