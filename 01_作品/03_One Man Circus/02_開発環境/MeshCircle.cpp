//================================================
//
// メッシュサークルの描画 [impact.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "MeshCircle.h"
#include "manager.h"
#include "renderer.h"
#include "meshfield.h"
#include "textureManager.h"

//================================================
// コンストラクタ
//================================================
CMeshCircle::CMeshCircle() : CObject(PRIORITY_UI_BACK)
{
	m_fDecAlv = NULL;
	m_fHeight = NULL;
	m_fInRadius = NULL;
	m_fOutRadius = NULL;
	m_nLife = NULL;
	m_fSpeed = NULL;
	m_InCol = Const::WHITE;
	m_OutCol = Const::WHITE;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nNumIdx = NULL;
	m_nNumPolygon = NULL;
	m_nNumVtx = NULL;
	m_nSegH = 1;
	m_nSegV = 1;
	m_pIdxBuffer = nullptr;
	m_pVtxBuffer = nullptr;
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	m_nTextureIdx = -1;
}

//================================================
// デストラクタ
//================================================
CMeshCircle::~CMeshCircle()
{
}

//================================================
// 生成処理
//================================================
CMeshCircle* CMeshCircle::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fInRadius, const float fOutRadius, const int nSegH)
{
	// メッシュサークルを生成
	CMeshCircle* pCircle = new CMeshCircle;

	// zの分割数1固定
	const int nSegV = 1;

	// 頂点数の設定
	pCircle->m_nNumVtx = (nSegH + 1) * (nSegV + 1);

	// ポリゴン数の設定
	pCircle->m_nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// インデックス数の設定
	pCircle->m_nNumIdx = pCircle->m_nNumPolygon + 2;

	pCircle->m_nSegH = nSegH;
	pCircle->m_nSegV = nSegV;

	pCircle->m_pos = pos;
	pCircle->m_OutCol = col;
	pCircle->m_InCol = col;
	pCircle->m_fInRadius = fInRadius;
	pCircle->m_fOutRadius = fOutRadius;

	// 内側の透明度を下げる
	pCircle->m_InCol.a = col.a * 0.5f;

	// 初期化処理
	if (FAILED(pCircle->Init()))
	{
		pCircle->Uninit();
		pCircle = nullptr;
		return nullptr;
	}

	return pCircle;
}

//================================================
// サークルの設定処理
//================================================
void CMeshCircle::SetParam(const float fHeight, const float fSpeed, const int nLife, const D3DXVECTOR3 rot)
{
	// 寿命に応じた透明度の減少値の計算
	m_fDecAlv = m_OutCol.a / nLife;

	// 要素の設定処理
	m_fHeight = fHeight;
	m_fSpeed = fSpeed;
	m_nLife = nLife;
	m_rot = rot;
}

//================================================
// 初期化処理
//================================================
HRESULT CMeshCircle::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 計算用の位置
	D3DXVECTOR3 posWk;

	// 頂点のカウント
	int nCntVtx = 0;

	float fTexPosX = 1.0f / m_nSegH;

	// 横の分割数分回す
	for (int nCnt = 0; nCnt <= m_nSegH; nCnt++)
	{
		// 角度の割合を求める
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCnt;

		// 位置を求める
		posWk.x = sinf(fAngle) * m_fInRadius;
		posWk.y = m_fHeight;
		posWk.z = cosf(fAngle) * m_fInRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色の設定
		pVtx[nCntVtx].col = m_InCol;

		// テクスチャの設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCnt, 1.0f);

		nCntVtx++;
	}

	// 横の分割数分回す
	for (int nCnt = 0; nCnt <= m_nSegH; nCnt++)
	{
		// 角度の割合を求める
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCnt;

		// 位置を求める
		posWk.x = sinf(fAngle) * m_fOutRadius;
		posWk.y = 0.0f;
		posWk.z = cosf(fAngle) * m_fOutRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色の設定
		pVtx[nCntVtx].col = m_OutCol;

		// テクスチャの設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCnt, 1.0f);

		nCntVtx++;
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	int IndxNum = m_nSegH + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	WORD* pIdx = nullptr;

	//インデックスバッファのロック
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_nSegV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_nSegH; IndxCount2++, IndxNum++, Num++)
		{
			// インデックスバッファの設定
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Num);
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < m_nSegV - 1)
		{
			pIdx[IdxCnt] = static_cast<WORD>(Num - 1);
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxNum);
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuffer->Unlock();

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CMeshCircle::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}

	// 自分自身の破棄
	CObject::Release();
}

//================================================
// 更新処理
//================================================
void CMeshCircle::Update(void)
{
	int nCntVtx = 0; // 頂点数のカウンター

	// 半径を拡大する
	m_fInRadius += m_fSpeed;
	m_fOutRadius += m_fSpeed;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 内側の頂点の更新
	for (int nCntX = 0; nCntX <= m_nSegH; nCntX++)
	{
		D3DXVECTOR3 posWk = Const::VEC3_NULL;

		// 一周の割合を求める
		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntX;

		// 変形に頂点を撃つ
		posWk.x = sinf(fAngle) * m_fInRadius;
		posWk.y = m_pos.y;
		posWk.z = cosf(fAngle) * m_fInRadius;

		pVtx[nCntVtx].pos = posWk;

		pVtx[nCntVtx].col = m_InCol;

		// 頂点のカウントを加算
		nCntVtx++;
	}

	// 外側の頂点の更新
	for (int nCntX = 0; nCntX <= m_nSegH; nCntX++)
	{
		D3DXVECTOR3 posWk = Const::VEC3_NULL;

		float fAngle = (D3DX_PI * 2.0f) / m_nSegH * nCntX;

		posWk.x = sinf(fAngle) * m_fOutRadius;
		posWk.y = m_pos.y;
		posWk.z = cosf(fAngle) * m_fOutRadius;

		pVtx[nCntVtx].pos = posWk;

		pVtx[nCntVtx].col = m_OutCol;

		nCntVtx++;
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	// a値を減らす
	m_InCol.a -= m_fDecAlv;
	m_OutCol.a -= m_fDecAlv;

	// 寿命を減らす
	m_nLife--;

	// 寿命が尽きたら
	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();

		return;
	}
}

//================================================
// 描画処理
//================================================
void CMeshCircle::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャのマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// カリングをオンにする
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffer);

	//テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureIdx));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//================================================
// テクスチャのパスの設定
//================================================
void CMeshCircle::SetTextureID(const char* pTexturePath)
{
	// テクスチャのマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// 取得できなかったら処理しない
	if (pTextureManager == nullptr) return;

	// 省略用パス
	std::string filePath = "data/TEXTURE/";

	// 文字列の連結
	filePath += pTexturePath;

	// テクスチャのIDの設定
	m_nTextureIdx = pTextureManager->Register(filePath.c_str());
}
