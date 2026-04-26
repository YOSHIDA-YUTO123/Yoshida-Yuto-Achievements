//==============================================
//
// 線の描画をするクラス [line.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "line.h"
#include "manager.h"
#include "renderer.h"

//**********************************************
// 静的メンバ変数宣言
//**********************************************
std::unique_ptr<CLineRenderer> CLineRenderer::m_pInstance = nullptr; // 自分のインスタンス

//==============================================
// コンストラクタ
//==============================================
CLineRenderer::CLineRenderer() :
	m_pVtxBuffer(nullptr)
{

}

//==============================================
// デストラクタ
//==============================================
CLineRenderer::~CLineRenderer()
{
}

//==============================================
// 自分の取得
//==============================================
CLineRenderer* CLineRenderer::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		// 生成
		m_pInstance.reset(new CLineRenderer);
		m_pInstance->Init();
	}

	return m_pInstance.get();
}

//==============================================
// 初期化処理
//==============================================
HRESULT CLineRenderer::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// あらかじめメモリを確保
	m_aLine.reserve(NUM_VERTEX);

	return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CLineRenderer::Uninit(void)
{
	m_aLine.clear();

	//頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// 自分自身の破棄
	m_pInstance.reset();
}

//==============================================
// 描画処理
//==============================================
void CLineRenderer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライティングOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// ZバッファをOFF

	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	int nCnt = 0;

	for (auto& line : m_aLine)
	{
		pVtx[nCnt].pos = line.pos;
		pVtx[nCnt].col = line.col;
		nCnt++;
	}
	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 要素分回す
	for (nCnt = 0; nCnt < static_cast<int>(m_aLine.size()); nCnt++)
	{
		pDevice->SetTexture(0, nullptr);
		pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 2 * nCnt);
	}

	// 要素のクリア
	m_aLine.clear();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライティングON
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// ZバッファをON
}

//==============================================
// 線の追加
//==============================================
void CLineRenderer::AddLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& col)
{
	Vertex vertex;

	vertex.pos = start;
	vertex.col = col;

	// 要素の追加
	m_aLine.push_back(vertex);

	vertex.pos = end;
	vertex.col = col;

	// 要素の追加
	m_aLine.push_back(vertex);
}
