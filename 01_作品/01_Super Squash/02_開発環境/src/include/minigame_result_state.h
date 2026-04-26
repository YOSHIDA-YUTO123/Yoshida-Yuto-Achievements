//================================================
//
// ミニゲームのリザルトの状態 [minigame_result_state.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _MINIGAME_RESULT_STATE_H_
#define _MINIGAME_RESULT_STATE_H_

//************************************************
// インクルードファイル
//************************************************
#include "scene_state_base.h"

//************************************************
// 前方宣言
//************************************************
class CSystemManager; // システムのマネージャー

//************************************************
// ミニゲームのリザルト状態のクラスの定義
//************************************************
class CMinigameResultState : public CSceneState
{
public:
	CMinigameResultState();
	~CMinigameResultState();

	HRESULT Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
private:
	void CreateSystem(CSystemManager *pSystemManager);
	void CreateRendererSystem(CSystemManager *pSystemManager);
};
#endif