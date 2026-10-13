//================================================
//
// リザルトのプレイヤー [ResultPlayer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _RESULT_PLAYER_H_
#define _RESULT_PLAYER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayer.h"

//*************************************************
// リザルトのプレイヤークラスの定義
//*************************************************
class CResultPlayer : public CScenePlayer
{
public:
	CResultPlayer();
	~CResultPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};
#endif