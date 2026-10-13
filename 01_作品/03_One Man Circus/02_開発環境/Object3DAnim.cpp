//===================================================
//
// アニメーション3Dポリゴン [Object3DAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "Object3DAnim.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"

//===================================================
// コンストラクタ
//===================================================
CObject3DAnim::CObject3DAnim(const int nPriority) : CObject(nPriority)
{
	m_nTextureIdx = -1;
	m_pVtxBuffer = nullptr;			 // 頂点バッファへのポインタ
	D3DXMatrixIdentity(&m_mtxWorld); // ワールドマトリックス
	m_rot = Const::VEC3_NULL;				 // 向き
	m_pos = Const::VEC3_NULL;				 // 位置
	m_Size = Const::VEC3_NULL;				 // 大きさ
	m_nSpeed = NULL;				 // アニメーションのスピード
	m_nCounter = NULL;				 // カウンター
	m_nPattern = NULL;				 // パターン
	m_nDivU = 1;					 // 横の分割数
	m_nDivV = 1;					 // 縦の分割数
	m_bLoop = false;				 // ループするかどうか
}

//===================================================
// デストラクタ
//===================================================
CObject3DAnim::~CObject3DAnim()
{
}

//===================================================
// 生成処理
//===================================================
CObject3DAnim* CObject3DAnim::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 Size, const int nDivU, const int nDivV)
{
	CObject3DAnim* pObject3D = new CObject3DAnim;

	pObject3D->SetPosition(pos);
	pObject3D->SetSize(Size);
	pObject3D->SetDivUV(nDivU, nDivV);

	// 初期化処理
	if (FAILED(pObject3D->Init()))
	{
		pObject3D->Uninit();
		pObject3D = nullptr;
		return nullptr;
	}

	return pObject3D;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CObject3DAnim::Init(void)
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
	D3DXVECTOR3 Size = m_Size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, Size.z);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, Size.z);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, -Size.z);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, -Size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = Const::WHITE;
	pVtx[1].col = Const::WHITE;
	pVtx[2].col = Const::WHITE;
	pVtx[3].col = Const::WHITE;

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
void CObject3DAnim::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CObject3DAnim::Update(void)
{
	// アニメーションのカウンターを進める
	m_nCounter++;

	float fPosTexU = 1.0f / m_nDivU;
	float fPosTexH = 1.0f / m_nDivV;

	if (m_nCounter >= m_nSpeed)
	{
		m_nCounter = 0;

		m_nPattern++;

		// Y座標の割合
		int nRatePosY = m_nPattern / m_nDivU;

		float UV = fPosTexU * m_nPattern;
		float HV = nRatePosY * fPosTexH;

		// 頂点情報のポインタ
		VERTEX_3D* pVtx;

		// 頂点バッファのロック
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(UV, HV);
		pVtx[1].tex = D3DXVECTOR2(UV + fPosTexU, HV);
		pVtx[2].tex = D3DXVECTOR2(UV, HV + fPosTexH);
		pVtx[3].tex = D3DXVECTOR2(UV + fPosTexU, HV + fPosTexH);

		// 頂点バッファのアンロック
		m_pVtxBuffer->Unlock();

		// 最大まで行ったら
		if (m_nPattern >= (m_nDivU * m_nDivV))
		{
			// パターンを初期化
			m_nPattern = 0;

			// ループしないなら
			if (!m_bLoop)
			{
				// 破棄
				Uninit();

				return;
			}
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CObject3DAnim::Draw(void)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//	ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向き
	D3DXVECTOR3 rot = m_rot;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置
	D3DXVECTOR3 pos = m_pos;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===================================================
// テクスチャのIDの設定
//===================================================
void CObject3DAnim::SetTextureID(const char* pTextureName)
{
	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// 省略用ファイルパス
	std::string filePath = "data/TEXTURE/";

	// 文字列の連結
	filePath += pTextureName;

	// テクスチャの
	if (pTextureManager == nullptr) return;

	// テクスチャのパスの設定
	m_nTextureIdx = pTextureManager->Register(filePath.c_str());
}

//===================================================
// パラメータの設定
//===================================================
void CObject3DAnim::SetParam(const int nSpeed, const bool bLoop)
{
	m_nSpeed = nSpeed;
	m_bLoop = bLoop;
}

//===================================================
// 分割の設定
//===================================================
void CObject3DAnim::SetDivUV(const int nDivU, const int nDivV)
{
	m_nDivU = nDivU;
	m_nDivV = nDivV;
}