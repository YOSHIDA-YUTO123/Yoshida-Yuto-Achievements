//================================================
//
// タイトルのプレイヤー [TitlePlayer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _TITLE_PLAYER_H_
#define _TITLE_PLAYER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayer.h"

//*************************************************
// タイトルのプレイヤーのクラスの定義
//*************************************************
class CTitlePlayer : public CScenePlayer
{
public:
	CTitlePlayer();
	~CTitlePlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	bool m_bStartAnim;
	float m_fMoveSpeed;
};
#endif