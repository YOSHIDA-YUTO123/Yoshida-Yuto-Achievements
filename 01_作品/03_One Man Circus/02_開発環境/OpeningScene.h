//===================================================
//
// オープニングシーンクラス [OpeningScene.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OPENING_SCENE_H_
#define _OPENING_SCENE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "scene.h"

//***************************************************
// オープニングシーンのクラスの定義
//***************************************************
class COpeningScene : public CScene
{
public:
	COpeningScene();
	~COpeningScene();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};
#endif