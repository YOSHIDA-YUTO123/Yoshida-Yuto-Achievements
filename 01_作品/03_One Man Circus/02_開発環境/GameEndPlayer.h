//================================================
//
// ゲーム終了ムービーのプレイヤー [GameEndPlayer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _GAMEENDPLAYER_H_
#define _GAMEENDPLAYER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayer.h"

//*************************************************
// リザルトのプレイヤークラスの定義
//*************************************************
class CGameEndPlayer : public CScenePlayer
{
public:
	CGameEndPlayer();
	~CGameEndPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};
#endif