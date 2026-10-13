//==============================================
//
// タイトルシーン [title.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "scene.h"
#include <memory>

//**********************************************
// 前方宣言
//**********************************************
class CCollisionMapObjManager;	// 当たり判定するマップオブジェクトのマネージャー

//**********************************************
// タイトルシーンクラスの定義
//**********************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	std::unique_ptr<CCollisionMapObjManager> m_pCollMapObjManager; // 当たり判定マップオブジェクトのマネージャー
};
#endif