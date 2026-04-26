//===================================================
//
// マネージャークラス [manager.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// 二重インクルード防止
//***************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CRenderer;			// レンダラー
class CScene;				// シーン
class CTextureManager;		// テクスチャマネージャー
class CModelManager;		// モデルマネージャー
class CInputKeyboard;		// キーボード
class CInputJoypad;			// ジョイパッド
class CInputMouse;			// マウス
class CFade;				// フェード
class CLight;				// ライト
class CWorldSystemManager;	// ゲームの補助のシステムのマネージャ
class CCamera;				// カメラの取得
class CTextureMRTManager;	// マルチレンダーターゲットのテクスチャのマネージャー
class CParticleHelper;		// パーティクルのヘルパークラス
class CShaderManager;		// シェーダーのマネージャークラス
class CSound;				// 音のクラス

//***************************************************
// マネージャークラスの定義
//***************************************************
class CManager final
{
public:
	~CManager();

	static CManager* GetInstance(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWind);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeMode(std::unique_ptr<CScene> pNewScene);
	void BeginFade(std::unique_ptr<CScene> pNewScene, const D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f });

	CRenderer* GetRenderer(void)						{ return m_pRenderer.get (); }
	CTextureManager* GetTextureManager(void)			{ return m_pTextureManager.get (); }
	CInputKeyboard* GetInputKeyboard(void)				{ return m_pKeyboard.get(); }
	CInputJoypad* GetInputJoypad(void)					{ return m_pJoypad.get(); }
	CInputMouse* GetInputMouse(void)					{ return m_pMouse.get(); }
	CModelManager* GetModelManager(void)				{ return m_pModelManager.get(); }
	CScene* GetScene(void)								{ return m_pScene.get(); }
	CLight* GetLight(void)								{ return m_pLight.get(); }
	CWorldSystemManager* GetWorldSystemManager(void)	{ return m_pWorldSystem.get(); }
	CCamera* GetCamera(void)							{ return m_pCamera.get(); }
	CTextureMRTManager* GetTextureMRTManager(void)		{ return m_pTextureMTManager.get(); }
	CParticleHelper* GetParticleHelper(void)			{ return m_pParticleHelper.get(); }
	CShaderManager* GetShaderManager(void)				{ return m_pShaderManager.get(); }
	CSound* GetSound(void)								{ return m_pSound.get(); }
	void SetFps(const int nFps)							{ m_nFps = nFps; }
private:
	CManager();

	static std::unique_ptr<CManager>		m_pInstance;			// 自分のインスタンス
	std::unique_ptr<CRenderer>				m_pRenderer;			// 描画処理クラスへのポインタ
	std::unique_ptr<CScene>					m_pScene;				// シーンクラスへのポインタ
	std::unique_ptr<CTextureManager>		m_pTextureManager;		// テクスチャマネージャークラスへのポインタ
	std::unique_ptr<CModelManager>			m_pModelManager;		// モデルマネージャークラスへのポインタ
	std::unique_ptr<CInputKeyboard>			m_pKeyboard;			// キーボードクラスへのポインタ
	std::unique_ptr<CInputJoypad>			m_pJoypad;				// パッドクラスへのポインタ
	std::unique_ptr<CInputMouse>			m_pMouse;				// マウスクラスへのポインタ
	std::unique_ptr<CFade>					m_pFade;				// フェードクラスへのポインタ
	std::unique_ptr<CLight>					m_pLight;				// ライトクラスへのポインタ
	std::unique_ptr<CWorldSystemManager>	m_pWorldSystem;			// ゲームの補助のシステムのマネージャ
	std::unique_ptr<CCamera>				m_pCamera;				// カメラクラスへのポインタ
	std::unique_ptr<CTextureMRTManager>		m_pTextureMTManager;	// マルチレンダーターゲットのテクスチャのマネージャーへのポインタ
	std::unique_ptr<CParticleHelper>		m_pParticleHelper;		// パーティクルの補助クラス
	std::unique_ptr<CShaderManager>			m_pShaderManager;		// シェーダーのマネージャークラス
	std::unique_ptr<CSound>					m_pSound;				// 音クラスへポインタ
	int m_nFps;														// フレームレート
};

#endif
