//================================================
//
// シーンのタイトルの状態の処理 [scene_state_base.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _SCENE_STATE_BASE_H_
#define _SCENE_STATE_BASE_H_

//************************************************
// インクルードファイル
//************************************************
#include "statebase.h"

//************************************************
// 前方宣言
//************************************************
class CScene; // シーン

//************************************************
// シーンのタイトルの状態の処理クラスの定義
//************************************************
class CSceneState : public CStateBase
{
public:

	// 状態の種類
	enum STATE
	{
		STATE_NONE = 0,			// 無し
		STATE_TITLE,			// タイトル
		STATE_MINIGAME,			// ミニゲーム
		STATE_GAME,				// ゲーム
		STATE_MINIGAME_RESULT,	// ミニゲームのリザルト
		STATE_GAME_RESULT,		// ゲームのリザルト
		STATE_MAX
	};

	CSceneState(const STATE state) :
		m_pScene(nullptr), m_state(state)
	{}

	~CSceneState() = default;

	void SetOwner(CScene* pOwner) { m_pScene = pOwner; }

	virtual HRESULT Init(void) override = 0;
	virtual void Update(void) override = 0;
	virtual void Uninit(void) override = 0;
	int GetID(void) const override { return m_state; }
protected:
	CScene* GetOwner(void) { return m_pScene; }
private:
	CScene* m_pScene;	// シーンクラスへのポインタ
	STATE m_state;		// 現在の状態
};
#endif