//===============================================
// 
// オブジェクト2Dのアニメーションクラス　[object2DAnim.cpp]
// Author：Rio Ohno
// 
//===============================================

// インクリメント
#include "object2DAnim.h"
#include "renderer.h"
#include "manager.h"
#include "textureManager.h"

//===============================================
// コンストラクタ
//===============================================
CObject2DAnim::CObject2DAnim(int nPriority):CObject(nPriority)
{
	// 各メンバ変数の初期化
	m_pVtxBuffer = nullptr;					// 頂点バッファ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_Size = D3DXVECTOR2(0.0f, 0.0f);		// サイズ
	m_fAngle = 0.0f;
	m_Length = 0.0f;
	m_nTextureIdx = -1;
	m_nCntFream = 0;
}

//===============================================
// デストラクタ
//===============================================
CObject2DAnim::~CObject2DAnim()
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

//===============================================
// 生成処理
//===============================================
CObject2DAnim* CObject2DAnim::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, int UV, int HV)
{
	CObject2DAnim* p2DAnim = nullptr;

	// メモリの確保
	if (p2DAnim == nullptr)
	{
		p2DAnim = new CObject2DAnim;
	}
	else
	{
		return nullptr;
	}

	// メンバ変数の初期化
	p2DAnim->m_pos = pos;
	p2DAnim->m_Size = size;
	p2DAnim->m_nUV = UV;
	p2DAnim->m_nHV = HV;
	p2DAnim->m_fU = (float)1 / (float)UV;
	p2DAnim->m_fV = (float)1 / (float)HV;

	//初期化処理
	p2DAnim->Init();


	return p2DAnim;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CObject2DAnim::Init(void)
{
	m_fAngle = 0.0f;
	m_Length = 0.0f;

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

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	m_Length = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// 頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[1].tex = D3DXVECTOR2(m_fU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_fV);
	pVtx[3].tex = D3DXVECTOR2(m_fU, m_fV);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObject2DAnim::Uninit(void)
{
	// 破棄フラグを立てる
	CObject2DAnim::Release();
}

//===============================================
// 更新処理
//===============================================
void CObject2DAnim::Update(void)
{
	// フレームカウントアップ
	++m_nCntFream;

	if (m_nCntFream >= m_nSpan)
	{
		// パターンインクリメント
		++m_nCntPattern;

		// フレームカウンタリセット
		m_nCntFream = 0;

		if (m_nCntPattern >= (m_nHV * m_nUV))
		{
			// パターンリセット
			m_nCntPattern = 0;

			if (m_bLoop == false)
			{
				// 終了処理
				Uninit();

				return;
			}
		}
	}

	// 頂点の更新
	UpdateVertex();
}

//===============================================
// 描画処理
//===============================================
void CObject2DAnim::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャを設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類	
}

//===============================================
// ループするかどうか、アニメーションの間隔設定
//===============================================
void CObject2DAnim::SetAnim(bool bLoop, int Span)
{
	m_nSpan = Span;
	m_bLoop = bLoop;
}

//===============================================
// UV分割数設定
//===============================================
void CObject2DAnim::SetAnim(int nU, int nV)
{
	m_nUV = nU;
	m_nHV = nV;
}

//===============================================
// テクスチャの設定
//===============================================
void CObject2DAnim::SetTextureID(const char* pTextureName)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// 取得できなかったら処理しない
	if (pTextureManager == nullptr) return;

	// 省略用パス
	std::string filePath = "data/TEXTURE/";

	// テクスチャのパスの連結
	filePath += pTextureName;

	// テクスチャのIDの取得
	m_nTextureIdx = pTextureManager->Register(filePath.c_str());
}

//===============================================
// 色の設定
//===============================================
void CObject2DAnim::SetColor(const D3DXCOLOR col)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===============================================
// 頂点の更新処理
//===============================================
void CObject2DAnim::UpdateVertex(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 rot = m_rot;

	m_Length = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// 頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - m_fAngle)) * m_Length;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_Length;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_Length;
	pVtx[3].pos.z = 0.0f;

	float fV = (float)(m_nCntPattern % m_nUV);

	pVtx[0].tex = D3DXVECTOR2(m_fU * m_nCntPattern, m_fV * fV);
	pVtx[1].tex = D3DXVECTOR2(m_fU * (m_nCntPattern+1), m_fV * fV);
	pVtx[2].tex = D3DXVECTOR2(m_fU * m_nCntPattern, m_fV * (fV + 1));
	pVtx[3].tex = D3DXVECTOR2(m_fU * (m_nCntPattern + 1), m_fV * (fV + 1));

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}