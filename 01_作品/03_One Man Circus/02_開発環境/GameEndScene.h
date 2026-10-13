//==================================================
// 
// ゲーム終了ムービー [GameEndScene.h]
// Author：Aoto Kaichi
// 
//==================================================

// 2重インクルード防止
#ifndef _GAMEENDSCENE_H_
#define _GAMEENDSCENE_H_

// インクルード
#include "scene.h"
#include "ScenePlayer.h"
#include "GameEndPlayer.h"
#include "CollisionMapObjManager.h"
#include "FunctionGameEnd.h"

class CGameEndScene :public CScene
{
public:
	CGameEndScene();
	~CGameEndScene();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	//**********
	void AddFunction(std::unique_ptr<CFunctionBase> pFunction);
	//**********

	static constexpr const char* GOAL_MODEL_PATH = "MapObjectFile/goal.x";			// ゴールのモデルのパス

	// メンバ変数
	CGameEndPlayer* m_pPlayer;
	int m_nCntTime;

	//**********
	std::vector<std::unique_ptr<CFunctionBase>> m_vpFunction;		// 機能ポインタ
	static std::unique_ptr<CCollisionMapObjManager> m_pCollMapObjManager;	// 当たり判定するマップオブジェクトのマネージャー
	//**********
};

#endif