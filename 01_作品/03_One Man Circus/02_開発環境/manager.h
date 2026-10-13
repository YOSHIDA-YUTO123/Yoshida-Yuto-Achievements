//===================================================
//
// マネージャー [manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include<memory>
#include"scene.h"

//***************************************************
// 前方宣言
//***************************************************
class CRenderer;
class CScene;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CLight;
class CFade;
class CSlow;
class CCamera;
class CModelManager;
class CTextureManager;

//***************************************************
// マネージャークラスの定義
//***************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFps(const int fps) { m_fps = fps; }

	static CRenderer* GetRenderer(void) { return m_pRenderer.get(); }
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard.get(); }
	static CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad.get(); }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse.get(); }
	static CSound* GetSound(void) { return m_pSound.get(); }
	static CFade* GetFade(void) { return m_pFade.get(); }
	static CSlow* GetSlow(void) { return m_pSlow.get(); }
	static CLight* GetLight(void) { return m_pLight.get(); }
	static CCamera* GetCamera(void) { return m_pCamera.get(); }
	static CTextureManager* GetTextureManager(void) { return m_pTextureManager.get(); }
	static CModelManager* GetModelManager(void) { return m_pModelManager.get(); }
	static int GetFrameCounter(void);
	static bool GetShowDebug(void) { return m_bShowDebug; }
	static void SetMode(std::unique_ptr<CScene> pNewScene);
	static void SetFade(std::unique_ptr<CScene> pNewScene);
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); }
	static void DrawScene(void);
	static CCamera* ChangeCamera(std::unique_ptr<CCamera> pNewCamera);
private:
	static std::unique_ptr<CRenderer> m_pRenderer;				// レンダラーのポインタ
	static std::unique_ptr<CInputKeyboard> m_pInputKeyboard;	// キーボードのポインタ
	static std::unique_ptr<CInputJoypad> m_pInputJoypad;		// パッドのポインタ
	static std::unique_ptr<CInputMouse> m_pInputMouse;			// マウスのポインタ
	static std::unique_ptr<CSound> m_pSound;					// サウンドのポインタ
	static std::unique_ptr<CLight> m_pLight;					// ライトへのポインタ
	static std::unique_ptr<CScene> m_pScene;					// シーンのクラスへのポインタ
	static std::unique_ptr<CSlow> m_pSlow;						// スローモーションクラスへのポインタ
	static std::unique_ptr<CFade> m_pFade;						// フェードクラスへのポインタ
	static std::unique_ptr<CCamera> m_pCamera;					// カメラクラスへのポインタ
	static std::unique_ptr<CTextureManager> m_pTextureManager;	// テクスチャマネージャーへのポインタ
	static std::unique_ptr<CModelManager> m_pModelManager;		// モデルマネージャーへのポインタ
	static int m_nFrameCounter;									// フレームカウンター
	static bool m_bShowDebug;									// デバッグ表示をするかしないか
	int m_fps;													// fps格納用変数
};
#endif