//================================================
//
// エフェクト処理 [effect.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//=================================================
// コンストラクタ
//=================================================
CEffect::CEffect() : 
	m_col(Const::WHITE),
	m_fDecALv(NULL),
	m_move(Const::VEC3_NULL),
	m_nMaxLife(NULL),
	m_nLife(NULL),
	m_fDecRadius(NULL),
	m_fGravity(NULL)
{
}

//=================================================
// デストラクタ
//=================================================
CEffect::~CEffect()
{
}

//=================================================
// 生成処理
//=================================================
CEffect* CEffect::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius, const int nLife)
{
	CEffect* pInstance = new CEffect;

	pInstance->SetPosition(pos);
	pInstance->SetSize({ fRadius,fRadius });
	pInstance->m_col = col;
	pInstance->m_nLife = nLife;
	pInstance->m_nMaxLife = nLife;

	// a値の減少スピードを求める
	pInstance->m_fDecALv = col.a / nLife;
	pInstance->m_fDecRadius = fRadius / nLife;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 生成に失敗したため破棄
		pInstance->Uninit();
		pInstance = nullptr;

		return nullptr;
	}

	// 色の設定
	pInstance->SetColor(col);

	return pInstance;
}

//=================================================
// 初期化処理
//=================================================
HRESULT CEffect::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	CObjectBillboard::SetTextureID("effect/effect000.jpg");

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CEffect::Uninit(void)
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CEffect::Update(void)
{
	// 更新処理
	CObjectBillboard::Update();

	// 位置の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();

	// 大きさの取得
	D3DXVECTOR2 Radius = CObjectBillboard::GetSize();
	
	// 移動量の減衰処理
	m_move.x += (0.0f - m_move.x) * m_fInertia;
	m_move.z += (0.0f - m_move.z) * m_fInertia;

	// 移動量の更新
	pos += m_move;

	// 重力量
	m_move.y += m_fGravity;

	Radius.x -= m_fDecRadius;
	Radius.y -= m_fDecRadius;

	// 頂点の設定
	CObjectBillboard::SetColor(m_col);
	CObjectBillboard::SetSize(Radius);
	CObjectBillboard::SetPosition(pos);

	// a値を減らす
	m_col.a -= m_fDecALv;

	m_nLife--;

	// 寿命が尽きたので破棄
	if (m_nLife <= 0)
	{
		// 破棄
		Uninit();
		return;
	}
}

//=================================================
// 描画処理
//=================================================
void CEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObjectBillboard::Draw();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=================================================
// パラメータの設定処理
//=================================================
void CEffect::SetParam(const float fInertia, const D3DXVECTOR3 move, const float fGravity)
{
	m_fInertia = fInertia;
	m_move = move;
	m_fGravity = fGravity;
}
