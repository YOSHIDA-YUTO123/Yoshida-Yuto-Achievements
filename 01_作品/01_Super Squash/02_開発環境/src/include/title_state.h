//================================================
//
// シーンのタイトルの状態の処理 [title_state.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _TITLE_STATE_H_
#define _TITLE_STATE_H_

//************************************************
// インクルードファイル
//************************************************
#include "scene_state_base.h"

//************************************************
// 前方宣言
//************************************************
class CSystemManager;	// システムのマネージャー
class CSound;			// 音

//************************************************
// シーンのタイトルの状態の処理クラスの定義
//************************************************
class CTitleState : public CSceneState
{
public:
	CTitleState();
	~CTitleState();

	HRESULT Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
private:
	void CreateTitleMenu(entt::registry& registry);
	void CreateSystem(CSystemManager *pSystemManager);
	void CreateRendererSystem(CSystemManager* pSystemManager);
	void CreateWall(entt::registry& registry, CSound* pSound);
	void CreateSetTitleLogo(entt::registry& registry, CSound* pSound);
private:
	int nWallCreateTime;	// 壁を生成する時間
	int nSetTitlteLogoTime;	// タイトルロゴを生成するまで時間
};
#endif