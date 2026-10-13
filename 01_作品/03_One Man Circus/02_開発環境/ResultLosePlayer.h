//================================================
//
// リザルト失敗のプレイヤー [ResultLosePlayer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _RESULT_LOSE_PLAYER_H_
#define _RESULT_LOSE_PLAYER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayer.h"

//*************************************************
// リザルト(失敗)のプレイヤークラスの定義
//*************************************************
class CResultLosePlayer : public CScenePlayer
{
public:
	CResultLosePlayer();
	~CResultLosePlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};
#endif