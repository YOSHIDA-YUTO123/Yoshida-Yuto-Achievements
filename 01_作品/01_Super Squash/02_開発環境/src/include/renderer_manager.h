//===================================================
//
// 描画状態のマネージャーの処理 [renderer_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_MANAGER_H_
#define _RENDERER_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
struct RendererComponent;	// 描画コンポーネント

//***************************************************
// 描画のマネージャー名前空間の定義
//***************************************************
namespace RendererManager
{
	void SetRenderState(LPDIRECT3DDEVICE9 pDevice, const RendererComponent* pRendererComp);
	void ResetRenderState(LPDIRECT3DDEVICE9 pDevice, const RendererComponent* pRendererComp);
	void SetRendererStateTextureMT(LPDIRECT3DDEVICE9 pDevice);
	void ResetRendererStateTextureMT(LPDIRECT3DDEVICE9 pDevice);
	void SetDefaultRenderTarget(void);
	void ChangeRenderTarget(const int nType);
	void ResetRenderTarget(void);
}
#endif