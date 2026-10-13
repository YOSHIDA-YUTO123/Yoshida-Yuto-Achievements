//*************************************
//
// 線
// Author Tetsuji Yamamoto
//
//*************************************
#include "line.h"
#include "manager.h"
#include "renderer.h"

//*************************************
// 線コンストラクタ
//*************************************
CLine::CLine()
{
	m_pVtxBuff = NULL;	// 頂点情報
	m_NumLine = 0;		// 線の数
}

//*************************************
// 線デストラクタ
//*************************************
CLine::~CLine()
{

}

//*************************************
// 線初期化処理
//*************************************
HRESULT CLine::Init(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CLine::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return E_FAIL;
	}

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = posA;
	pVtx[1].pos = posB;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//*************************************
// 線終了処理
//*************************************
void CLine::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//*************************************
// 線更新処理
//*************************************
void CLine::Update(void)
{

}

//*************************************
// 線描画処理
//*************************************
void CLine::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxWorld;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャを設定
	pDevice->SetTexture(0, NULL);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_LINELIST,
		0,
		2);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//*************************************
// 線生成処理
//*************************************
CLine* CLine::Create(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXCOLOR col)
{
	CLine* pLine = new CLine;

	if (pLine != NULL)
	{
		// 初期化処理
		pLine->Init(posA,posB,col);
	}
	else
	{
		MessageBox(NULL, " CLine::Create() メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pLine;
}