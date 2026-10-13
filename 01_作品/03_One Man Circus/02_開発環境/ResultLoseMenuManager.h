//===================================================
//
// リザルト失敗のメニューのマネージャークラス [ResultLoseMenuManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RESULT_LOSE_MENU_MANAGER_H_
#define _RESULT_LOSE_MENU_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "MenuManager.h"

//***************************************************
// リザルトのメニューのマネージャークラスの定義
//***************************************************
class CResultLoseMenuManager : public CMenuManager
{
public:
	CResultLoseMenuManager();
	~CResultLoseMenuManager();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	int m_nCurrentIndex; // 選択中のインデックス
};
#endif