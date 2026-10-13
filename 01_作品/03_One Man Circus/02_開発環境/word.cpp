//**************************************
// 
// ワード
// AouthorTetsuji Yamamoto
// 
//**************************************
#include "word.h"
#include "debugproc.h"
#include "manager.h"
#include "myMath.h"
#include "fontManager.h"
#include "input.h"
#include "renderer.h"
#include <locale.h>

//**************************************
// ワードコンストラクタ
//**************************************
CWord::CWord()
{
	m_pVtxBuff = nullptr;				// 頂点バッファ
	m_pTextureFont = nullptr;			// フォント用のテクスチャ
	m_pSurfaceTexFont = nullptr;		// フォント用テクスチャのレンダーターゲット
	m_nStrWidth = 0;					// 文字列の幅
	m_col = Const::WHITE;				// カラー
	m_format = DT_LEFT;					// 文字の書き出し形式
}

//**************************************
// ワードデストラクタ
//**************************************
CWord::~CWord()
{
}

//**************************************
// ワード初期化
//**************************************
void CWord::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* pStr, DWORD format, int nFontID, const char* pFontName)
{
	// マルチバイト文字系関数を使用するための設定
	setlocale(LC_ALL, "");

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 文字列
	SetStringWord(pStr);

	// メンバー変数設定
	m_col = col;
	m_format = format;

	// フォントを生成
	m_sStrFont = CFontManager::Regiter(nFontID,pFontName);

	// 文字列の長さを取得
	int nLength = strlen(pStr);

	// 文字列の幅設定
	m_nStrWidth = (int)(100.f * 0.5f) * nLength;

	// 頂点バッファの生成
	 HRESULT hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	 // 頂点バッファの生成に失敗した
	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init > 頂点バッファをクリエイトできませんでした", "Error", MB_OK);
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

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

	// テクスチャの生成
	if (m_pTextureFont == nullptr)
	{
		hr = pDevice->CreateTexture(
			m_nStrWidth,
			100,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pTextureFont,
			NULL);
	}

	// テクスチャの生成に失敗した
	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init() > pDevice->CreateTexture() > 失敗", "Error", MB_OK);
		return;
	}

	// テクスチャからレンダーターゲット用のサーフェスを取得
	if (m_pSurfaceTexFont == nullptr)
	{
		hr = m_pTextureFont->GetSurfaceLevel(0, &m_pSurfaceTexFont);
	}

	if (FAILED(hr))
	{
		MessageBox(NULL, "CWord::Init() > GetSurfaceLevel() > 失敗", "Error", MB_OK);
		return;
	}
}

//**************************************
// ワード終了
//**************************************
void CWord::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// サーフェイスの破棄
	if (m_pSurfaceTexFont != nullptr)
	{
		m_pSurfaceTexFont->Release();
		m_pSurfaceTexFont = nullptr;
	}

	// テクスチャの破棄
	if (m_pTextureFont != nullptr)
	{
		m_pTextureFont->Release();
		m_pTextureFont = nullptr;
	}

	// 自身の破棄
	delete this;
}

//**************************************
// ワード更新
//**************************************
void CWord::Update(void)
{

}

//**************************************
// ワード描画
//**************************************
void CWord::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 枠を設定
	RECT rect = { 0,0,m_nStrWidth,100 };

	// レンダーターゲットとしてテクスチャに描画
	pDevice->SetRenderTarget(0, m_pSurfaceTexFont);

	// テクスチャのクリア
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// テキストの描画
	LPD3DXFONT pFont = CFontManager::GetFont(m_sStrFont.c_str());
	if (pFont != NULL)
	{
		pFont->DrawText(NULL, m_sStrWord.c_str(), -1, &rect, m_format, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// バックバッファを取得
	LPDIRECT3DSURFACE9 pBuckBuff = nullptr;
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBuckBuff);

	// レンダーターゲットをバックバッファに設定
	pDevice->SetRenderTarget(0, pBuckBuff);

	// テクスチャを設定
	pDevice->SetTexture(0, m_pTextureFont);
	
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

//**************************************
// ワード色変更
//**************************************
void CWord::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色を変更
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//**************************************
// ワード生成
//**************************************
CWord* CWord::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* pStr, DWORD format, int nFontID, const char* pFontName)
{
	// ワードインスタンス生成
	CWord* pWord = new CWord;

	if (pWord != nullptr)
	{
		// 初期化処理
		pWord->Init(pos, col, fWidth, fHeight, pStr,format, nFontID, pFontName);
	}

	return  pWord;
}