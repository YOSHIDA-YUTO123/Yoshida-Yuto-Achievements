//**********************************************
// 
// 2Dポリゴン処理
// Author Tetsuji Yamamoto
// 
//**********************************************
#include "porygon2D.h"
#include "renderer.h"
#include "manager.h"
#include "textureManager.h"
#include "myMath.h"

//**********************************************
// 2Dポリゴンコンストラクタ
//**********************************************
CPolygon2D::CPolygon2D()
{
	m_pVtxBuff = nullptr;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nIdxTexture = -1;
}

//**********************************************
// 2Dポリゴンデストラクタ
//**********************************************
CPolygon2D::~CPolygon2D()
{
}

//**********************************************
// 2Dポリゴン初期化処理
//**********************************************
void CPolygon2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	// テクスチャ取得
	CTextureManager*pTexture = CManager::GetTextureManager();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	HRESULT hr;

	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CREnderer::InitPoligon > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 各種変数の初期化
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pos = pos;
	m_rot = rot;
	m_col = col;

	// テクスチャの読込
	m_nIdxTexture = pTexture->Register(pTexturePass);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = pos.x - fWidth;
	pVtx[0].pos.y = pos.y - fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + fWidth;
	pVtx[1].pos.y = pos.y - fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x - fWidth;
	pVtx[2].pos.y = pos.y + fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + fWidth;
	pVtx[3].pos.y = pos.y + fHeight;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return;
}

//**********************************************
// 2Dポリゴン終了処理
//**********************************************
void CPolygon2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pVtxBuff != nullptr)
	{
		MessageBox(NULL, " CPolygon2D::Uninit() > メモリがNULLではありません", "Error", MB_OK);
	}

	// 自身の破棄
	delete this;
}

//**********************************************
// 2Dポリゴン更新処理
//**********************************************
void CPolygon2D::Update(void)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	float fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	float fAngleA = atan2f(m_fWidth, m_fHeight);
	float fAngleB = atan2f(m_fWidth, -m_fHeight);

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleB));
	pVtx[0].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleB));
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleB));
	pVtx[1].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleB));
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + fLength * sinf(m_rot.z - fAngleA));
	pVtx[2].pos.y = (m_pos.y + fLength * cosf(m_rot.z - fAngleA));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + fLength * sinf(m_rot.z + fAngleA));
	pVtx[3].pos.y = (m_pos.y + fLength * cosf(m_rot.z + fAngleA));
	pVtx[3].pos.z = 0.0f;

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**********************************************
// 2Dポリゴン描画処理
//**********************************************
void CPolygon2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャインスタンス取得
	CTextureManager* pTexture = CManager::GetTextureManager();

	// テクスチャを設定
	pDevice->SetTexture(0, pTexture->GetAdress(m_nIdxTexture));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//**********************************************
// 2Dポリゴン生成処理
//**********************************************
CPolygon2D* CPolygon2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass)
{
	//	ポリゴンUIのインスタンス生成
	CPolygon2D* pPolygonUI = new CPolygon2D;

	// 確保できた
	if (pPolygonUI != nullptr)
	{
		// 初期化処理
		pPolygonUI->Init(pos, rot, col, fWidth, fHeight, pTexturePass);
	}
	// 出系なかった
	else
	{
		MessageBox(NULL, " CPolygon2D::Create() > メモリを確保できませんでした", "Error", MB_OK);
	}

	return pPolygonUI;
}