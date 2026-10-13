//===================================================
//
// オープニングのUIのマネージャー処理 [OpeningUIManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OPENING_UI_MANAGER_H_
#define _OPENING_UI_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class COpening_UI; // オープニングのUI

//***************************************************
// オープニングのUIのマネージャー
//***************************************************
class COpeningUIManager : public CObject
{
public:

	// UIの種類
	enum UITYPE
	{
		UITYPE_KEYBOARD = 0, // キーボード
		UITYPE_JOYPAD,		 // パッド
		UITYPE_MAX,
	};

	COpeningUIManager();
	~COpeningUIManager();

	static COpeningUIManager* Create(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	std::array<COpening_UI*, UITYPE_MAX> m_apUI; // UI
	UITYPE m_CurrentUIType;						 // 現在のUIの種類
};
#endif