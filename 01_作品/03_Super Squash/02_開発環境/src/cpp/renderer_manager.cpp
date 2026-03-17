//===================================================
//
// 描画状態のマネージャーの処理 [renderer_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_manager.h"
#include "renderer_component.hpp"
#include "manager.h"
#include "texture_mrt_manager.h"

//===================================================
// レンダーステートの設定
//===================================================
void RendererManager::SetRenderState(LPDIRECT3DDEVICE9 pDevice, const RendererComponent* pRendererComp)
{
	// ライトオフ
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_LIGHT_OFF)
	{
		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	// zテストON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ZTEST)
	{
		// ゼットテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// aテストON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ALPHA_TEST)
	{
		// アルファテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	// 加算合成ON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ALPHA_BLEND)
	{
		// aブレンディング
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// 減算合成ON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_SUB_BLEND)
	{
		// 減算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// カリングしない
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_NO_CULL)
	{
		// 両面描画
		pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
	}

	// 表面カリング(裏側表示)
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_CULL_FRONT)
	{
		// カリングをもとに戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}
}

//===================================================
// レンダーステートのリセット
//===================================================
void RendererManager::ResetRenderState(LPDIRECT3DDEVICE9 pDevice, const RendererComponent* pRendererComp)
{
	// zテストON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ZTEST)
	{
		// ゼットテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	// aテストON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ALPHA_TEST)
	{
		// アルファテストを無効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	// 加算合成ON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_ALPHA_BLEND)
	{
		// aブレンディングをもとに戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 減算合成ON
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_SUB_BLEND)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ライトオフ
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_LIGHT_OFF)
	{
		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	// カリングしない
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_NO_CULL)
	{
		// カリングをもとに戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	// 表面カリング(裏側表示)
	if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_CULL_FRONT)
	{
		// カリングをもとに戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//===================================================
// マルチテクスチャのレンダーステートの設定
//===================================================
void RendererManager::SetRendererStateTextureMT(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャステージステートの設定
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	// テクスチャステージステートの設定
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//===================================================
// マルチテクスチャのレンダーステートのリセット
//===================================================
void RendererManager::ResetRendererStateTextureMT(LPDIRECT3DDEVICE9 pDevice)
{
	// もとに戻す
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//===================================================
// デフォルトのレンダーターゲットの設定
//===================================================
void RendererManager::SetDefaultRenderTarget(void)
{
}

//===================================================
// レンダーターゲットの変更
//===================================================
void RendererManager::ChangeRenderTarget(const int nType)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャMTマネージャーの取得
	CTextureMRTManager* pTextureMTManager = pManager->GetTextureMRTManager();

	// 取得できなかったら処理しない
	if (pTextureMTManager == nullptr) return;

	// レンダーターゲットの変更
	pTextureMTManager->ChangeRenderTarget(nType);
}

//===================================================
// レンダーターゲットのリセット
//===================================================
void RendererManager::ResetRenderTarget(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャMTマネージャーの取得
	CTextureMRTManager* pTextureMTManager = pManager->GetTextureMRTManager();

	// 取得できなかったら処理しない
	if (pTextureMTManager == nullptr) return;

	// レンダーターゲットのリセット
	pTextureMTManager->ResetRenderTarget();
}