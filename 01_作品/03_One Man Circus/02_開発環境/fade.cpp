//===================================================
//
// フェード処理 [fade.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "fade.h"
#include"manager.h"
#include"renderer.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Fade_Constants
{
	constexpr int INIT_TIME = 40; // フェードのタイム初期化
}

using namespace Const; // 名前空間Constを使用
using namespace std; // 名前空間stdを使用

//===================================================
// コンストラクタ
//===================================================
CFade::CFade() :
	m_pScene(nullptr),
	m_Fade(FADE_NONE),
	m_InColor({ 0.0f,0.0f,0.0f,1.0f }), 
	m_OutColor({ 0.0f,0.0f,0.0f,0.0f }),
	m_fAlphaChangeSpeed(NULL)
{
}

//===================================================
// デストラクタ
//===================================================
CFade::~CFade()
{
}

//===================================================
// 生成処理
//===================================================
CFade* CFade::Create(void)
{
	// フェードの生成
	CFade* pFade = new CFade;

	if (FAILED(pFade->Init()))
	{
		pFade->Uninit();

		pFade = nullptr;

		return nullptr;
	}

	return pFade;
}

//===================================================
// フェード
//===================================================
void CFade::SetFade(std::unique_ptr<CScene> pNewScene, const int nTime, const D3DXCOLOR& Incol, const D3DXCOLOR& OutColor)
{
	if (m_Fade != FADE_NONE)
	{
		// 破棄する
		pNewScene.reset();
		pNewScene = nullptr;

		return;
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene.reset();
		m_pScene = nullptr;
	}

	if (nTime != 0)
	{
		// アルファ値の変化スピードの設定
		m_fAlphaChangeSpeed = 1.0f / nTime;
	}

	m_InColor = Incol;
	m_OutColor = OutColor;
	m_Fade = FADE_OUT;

	// 所有権を渡す
	m_pScene = move(pNewScene);
}

//===================================================
// 初期化処理
//===================================================
HRESULT CFade::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	m_Fade = FADE_IN;

	m_InColor = D3DXCOLOR(0.0f, 0.0, 0.0f, 1.0f);
	m_OutColor = D3DXCOLOR(0.0f, 0.0, 0.0f, 0.0f);
	m_fAlphaChangeSpeed = 1.0f / Fade_Constants::INIT_TIME;

	m_pScene = nullptr;

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_InColor;
	pVtx[1].col = m_InColor;
	pVtx[2].col = m_InColor;
	pVtx[3].col = m_InColor;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//===================================================
// 更新処理
//===================================================
void CFade::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	if (m_Fade != FADE_NONE)
	{
		if (m_Fade == FADE_IN)
		{
			m_InColor.a -= m_fAlphaChangeSpeed;

			if (m_InColor.a <= 0.0f)
			{
				m_InColor.a = 0.0f;
				m_Fade = FADE_NONE;
			}

			// 頂点バッファのロック
			m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = m_InColor;
			pVtx[1].col = m_InColor;
			pVtx[2].col = m_InColor;
			pVtx[3].col = m_InColor;

			// 頂点バッファのアンロック
			m_pVtxBuffer->Unlock();
		}
		else if (m_Fade == FADE_OUT)
		{
			// フェードアウト状態
			m_OutColor.a += m_fAlphaChangeSpeed;

			if (m_OutColor.a >= 1.0f)
			{
				m_OutColor.a = 1.0f;
				m_Fade = FADE_IN;

				// モード設定
				CManager::SetMode(move(m_pScene));

				return;
			}

			// 頂点バッファのロック
			m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = m_OutColor;
			pVtx[1].col = m_OutColor;
			pVtx[2].col = m_OutColor;
			pVtx[3].col = m_OutColor;

			// 頂点バッファのアンロック
			m_pVtxBuffer->Unlock();
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャがない
	pDevice->SetTexture(0, NULL);

	// プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類	
}

