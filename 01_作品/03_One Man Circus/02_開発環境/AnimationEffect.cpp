//===================================================
//
// アニメーションするエフェクト [AnimationEffect.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "AnimationEffect.h"
#include "renderer.h"
#include "manager.h"

//===================================================
// コンストラクタ
//===================================================
CAnimationEffect::CAnimationEffect()
{
}

//===================================================
// デストラクタ
//===================================================
CAnimationEffect::~CAnimationEffect()
{
}

//===================================================
// 生成処理
//===================================================
CAnimationEffect* CAnimationEffect::Create(const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const int nDiviU, const int nDiviV, const int nLife)
{
	CAnimationEffect* pInstance = new CAnimationEffect;

	pInstance->SetPosition(pos);
	pInstance->SetSize({ fRadius ,fRadius });
	pInstance->m_col = col;
	pInstance->m_nLife = nLife;
	pInstance->m_nMaxLife = nLife;
	pInstance->m_fDecALv = col.a / nLife;
	pInstance->m_fDecRadius = fRadius / nLife;
	pInstance->SetDivUV(nDiviU, nDiviV);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	pInstance->SetColor(col);

	return pInstance;
}

//===================================================
// パラメータの設定処理
//===================================================
void CAnimationEffect::SetParam(const float fInertia, const D3DXVECTOR3 move, const int nAnimSpeed, const bool bLoop, const float fGravity)
{
	m_fInertia = fInertia;
	m_move = move;
	m_fGravity = fGravity;

	// パラメータの設定
	CBillboardAnimation::SetParam(nAnimSpeed, bLoop);
}

//===================================================
// 初期化処理
//===================================================
HRESULT CAnimationEffect::Init(void)
{
	// 初期化処理
	if (FAILED(CBillboardAnimation::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CAnimationEffect::Uninit(void)
{
	// 終了処理
	CBillboardAnimation::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CAnimationEffect::Update(void)
{
	// 更新処理
	CBillboardAnimation::Update();

	// 位置の取得
	D3DXVECTOR3 pos = CBillboardAnimation::GetPosition();

	// 大きさの取得
	D3DXVECTOR2 Radius = CBillboardAnimation::GetSize();

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
	CBillboardAnimation::SetColor(m_col);
	CBillboardAnimation::SetSize(Radius);
	CBillboardAnimation::SetPosition(pos);

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

//===================================================
// 描画処理
//===================================================
void CAnimationEffect::Draw(void)
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
	CBillboardAnimation::Draw();

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