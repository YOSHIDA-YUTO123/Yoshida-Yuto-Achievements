//===================================================
//
// DirectXの描画処理 [renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CScene;			// シーン
class CDebugProc;		// デバッグ情報
class CFade;			// フェード
class CScreenPolygon;	// スクリーン

//***************************************************
// レンダラークラスの定義
//***************************************************
class CRenderer
{
public:
	// スクリーンポリゴンの種類
	enum SCREEN_POLYGON
	{
		SCREEN_POLYGON_SCENE = 0,		// シーン
		SCREEN_POLYGON_LUMINANCE,		// 輝度抽出
		SCREEN_POLYGON_GAUSBLUR_X,	// ガウスブラー横
		SCREEN_POLYGON_GAUSBLUR_Y,	// ガウスブラー縦
		SCREEN_POLYGON_SHADOW_MAP,	// シャドウマップ
		SCREEN_POLYGON_MAX
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(CScene* pScene);
	void Draw(const int fps, CScene* pScene, CFade* pFade);

	LPDIRECT3DDEVICE9 GetDevice(void);

	void onWireFrame();
	void offWireFrame();
private:
	void ChangeRenderTarget(void);
	void ResetRenderTarget(void);

	std::array<std::unique_ptr<CScreenPolygon>, SCREEN_POLYGON_MAX> m_apScreenPolygon;	// スクリーンポリゴン
	std::unique_ptr<CDebugProc> m_pDebugProc;	// デバッグ情報のクラス
	LPDIRECT3D9					m_pD3D;			// Directxデバイスへのポインタ
	LPDIRECT3DDEVICE9			m_pD3DDevice;	// Directxデバイスへのポインタ
	LPDIRECT3DTEXTURE9			m_pTextureMT;	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9			m_pRenderMT;	// レンダリングターゲット用インターフェース
	LPDIRECT3DSURFACE9			m_pZBuffMT;		// レンダリングターゲット用Zバッファ
	D3DVIEWPORT9				m_viewport;		// テクスチャレンダリング用ビューポート

};

#endif