//===================================================
//
// メニューの項目クラス [MenuItem.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_ITEM_H_
#define _MENU_ITEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object2D.h"

//***************************************************
// 前方宣言
//***************************************************
class CMenuVisualBase; // メニューの見た目のインターフェース

//***************************************************
// メニューの項目のクラスの定義
//***************************************************
class CMenuItem final : public CObject2D
{
public:
	CMenuItem();
	~CMenuItem();

	static CMenuItem* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& Size, const D3DXCOLOR& col, const char* pTexturePath,std::unique_ptr<CMenuVisualBase> pMenuVisual, const float fScal = 1.0f);

	void SetSelect(const bool bEnable) { m_bSelect = bEnable; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	std::unique_ptr<CMenuVisualBase> m_pMenuVisual;	// メニューの見た目
	D3DXCOLOR m_col;								// 色
	float m_fScal;									// 拡大率
	bool m_bSelect;									// 選択されているかどうか
};
#endif