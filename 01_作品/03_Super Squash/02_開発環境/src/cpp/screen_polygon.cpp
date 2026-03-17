//===================================================
//
// 画面のポリゴンの描画 [screen_polygon.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "screen_polygon.h"
#include "manager.h"
#include "texture_mrt_manager.h"
#include "renderer.h"
#include "shader.h"
#include "shader_manager.h"

//===================================================
// コンストラクタ
//===================================================
CScreenPolygon::CScreenPolygon() : 
	m_pVtxBuffer(nullptr),
	m_nMRTType(),
	m_nShaderType(-1)
{
}

//===================================================
// デストラクタ
//===================================================
CScreenPolygon::~CScreenPolygon()
{
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//===================================================
// 画面のポリゴンの描画処理
//===================================================
std::unique_ptr<CScreenPolygon> CScreenPolygon::Create(const int nMRTType1, const int nMRTType2, const int nShaderType)
{
	auto pInstance = std::make_unique<CScreenPolygon>();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance.reset();
		return nullptr;
	}

	pInstance->m_nMRTType[0] = nMRTType1;
	pInstance->m_nMRTType[1] = nMRTType2;
	pInstance->m_nShaderType = nShaderType;

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CScreenPolygon::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = Const::WHITE;
	pVtx[1].col = Const::WHITE;
	pVtx[2].col = Const::WHITE;
	pVtx[3].col = Const::WHITE;

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
void CScreenPolygon::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CScreenPolygon::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CScreenPolygon::Draw(const int nPass)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// MRTテクスチャの描画処理
	CTextureMRTManager* pTextureMTManager = pManager->GetTextureMRTManager();

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();
#if 1
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	CShader* pShader = nullptr;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (m_nShaderType != -1)
	{
		// シェーダーの取得
		pShader = pShaderManager->Get(static_cast<CShaderManager::TYPE>(m_nShaderType));

		pShader->Begin();

		pShader->Apply(nullptr, entt::null);

		pShader->BeginPass(nPass);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, pTextureMTManager->GetAdress(m_nMRTType[0]));
	pDevice->SetTexture(1, pTextureMTManager->GetAdress(m_nMRTType[1]));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類

	if (m_nShaderType != -1)
	{
		pShader->EndPass();

		pShader->End();
	}

#endif // 0
}