//===================================================
//
// ビルボードアニメーションの描画 [billboardAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "billboardAnim.h"
#include "manager.h"
#include "textureManager.h"
#include "renderer.h"
#include <string>

using namespace Const;	// 名前空間Constを使用
using namespace std;	// 名前空間stdを使用

//===================================================
// コンストラクタ
//===================================================
CBillboardAnimation::CBillboardAnimation(int nPriority) : CObject(nPriority)
{
	m_bLoop = false;
	m_fPosTexH = NULL;
	m_fPosTexU = NULL;
	m_nSpeed = NULL;
	m_nCounter = NULL;
	m_nPattern = NULL;
	m_nDivU = 1;
	m_nDivV = 1;
	m_nTextureIdx = -1;
}

//===================================================
// デストラクタ
//===================================================
CBillboardAnimation::~CBillboardAnimation()
{
}

//===================================================
// アニメーションの設定処理
//===================================================
void CBillboardAnimation::SetParam(const int nSpeed, const bool bLoop)
{
	m_bLoop = bLoop;
	m_nSpeed = nSpeed;
}

//===================================================
// 分割数の設定
//===================================================
void CBillboardAnimation::SetDivUV(const int nDivU, const int nDivV)
{
	m_nDivU = nDivU;
	m_nDivV = nDivV;
}

//===================================================
// テクスチャの座標の設定
//===================================================
void CBillboardAnimation::SetTextureVtx(const D3DXVECTOR2 OffPos, const D3DXVECTOR2 RateUV)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(RateUV.x, RateUV.y);
	pVtx[1].tex = D3DXVECTOR2(RateUV.x + OffPos.x, RateUV.y);
	pVtx[2].tex = D3DXVECTOR2(RateUV.x, RateUV.y + OffPos.y);
	pVtx[3].tex = D3DXVECTOR2(RateUV.x + OffPos.x, RateUV.y + OffPos.y);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===================================================
// 色の設定
//===================================================
void CBillboardAnimation::SetColor(const D3DXCOLOR col)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

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

//===================================================
// テクスチャのIDの設定
//===================================================
void CBillboardAnimation::SetTextureID(const char* pTextureName)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// 取得できなかったら処理しない
	if (pTextureManager == nullptr) return;

	// 省略用パス
	string filePath = "data/TEXTURE/Animation/";

	// テクスチャのパスの連結
	filePath += pTextureName;

	// テクスチャのIDの取得
	m_nTextureIdx = pTextureManager->Register(filePath.c_str());
}

//===================================================
// 頂点座標の更新
//===================================================
void CBillboardAnimation::UpdateVertexPos(const D3DXVECTOR3 pos)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 位置の設定
	m_pos = pos;

	// 大きさの取得
	D3DXVECTOR2 Size = m_Size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===================================================
// 生成処理
//===================================================
CBillboardAnimation* CBillboardAnimation::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nDivU, const int nDivV)
{
	CBillboardAnimation* pBillboard = new CBillboardAnimation;

	pBillboard->SetPosition(pos);
	pBillboard->SetSize(Size);

	pBillboard->m_nDivU = nDivU;
	pBillboard->m_nDivV = nDivV;

	// 初期化処理
	if (FAILED(pBillboard->Init()))
	{
		pBillboard->Uninit();
		pBillboard = nullptr;
		return nullptr;;
	}

	return pBillboard;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CBillboardAnimation::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR2 Size = m_Size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = WHITE;
	pVtx[1].col = WHITE;
	pVtx[2].col = WHITE;
	pVtx[3].col = WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / m_nDivU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / m_nDivV);
	pVtx[3].tex = D3DXVECTOR2(1.0f / m_nDivU, 1.0f / m_nDivV);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CBillboardAnimation::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// 自分自身の破棄
	Release();
}

//===================================================
// 更新処理
//===================================================
void CBillboardAnimation::Update(void)
{
	// アニメーションのカウンターを進める
	m_nCounter++;

	m_fPosTexU = 1.0f / m_nDivU;
	m_fPosTexH = 1.0f / m_nDivV;

	if (m_nCounter >= m_nSpeed)
	{
		m_nCounter = 0;

		m_nPattern++;

		// Y座標の割合
		int nRatePosY = m_nPattern / m_nDivV;

		float UV = m_fPosTexU * m_nPattern;
		float HV = nRatePosY * m_fPosTexH;

		// テクスチャ座標の更新
		SetTextureVtx(D3DXVECTOR2(m_fPosTexU, m_fPosTexH), D3DXVECTOR2(UV, HV));

		// 最大まで行ったら
		if (m_nPattern >= (m_nDivU * m_nDivV))
		{
			// パターンを初期化
			m_nPattern = 0;

			// ループしないなら
			if (!m_bLoop)
			{
				Uninit();
				return;
			}
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CBillboardAnimation::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを宣言
	D3DXMATRIX mtxView;

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
