//================================================
//
// シーンのタイトルの状態の処理 [minigame_state.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _MINIGAME_STATE_H_
#define _MINIGAME_STATE_H_

//************************************************
// インクルードファイル
//************************************************
#include "scene_state_base.h"

//************************************************
// 前方宣言
//************************************************
class CMiniGameManager;

//************************************************
// ミニゲームの状態のクラスの定義
//************************************************
class CMinigameState : public CSceneState
{
public:
	CMinigameState();
	~CMinigameState();

	static CMiniGameManager* GetManager(void) { return m_pManager.get(); }

	HRESULT Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
private:
	void CreatePauseMenu(entt::registry& registry);
	void SetCameraSystem(entt::registry& registry);
	void CreateSystem(CScene *pScene);
	void CreateRendererSysytem(CScene* pScene);
private:
	static std::unique_ptr<CMiniGameManager> m_pManager;	// ミニゲームのマネージャー
};
#endif