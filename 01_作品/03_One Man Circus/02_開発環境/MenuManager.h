//===================================================
//
// メニューのマネージャーの基底クラスの定義 [MenuManager.h]
// Author:YUTO YOSHIDA
// 
// << このクラスを派生して●●MenuManagerを作る >>
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_MANAGER_H_
#define _MENU_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "MenuCommandBase.h"

//***************************************************
// 前方宣言
//***************************************************
class CMenuItem;		// メニューの項目の親クラス

//***************************************************
// メニューの構成情報の名前空間の定義
//***************************************************
namespace MenuEntry
{
	struct Menu
	{
		CMenuItem* pMenuItem;							// メニューの項目クラスへのポインタ(Object継承なので生ポインタで所持)
		std::unique_ptr<CMenuCommandBase> pMenuCommand;	// メニューのコマンド
	};
};

//***************************************************
// メニューのマネージャー
//***************************************************
class CMenuManager : public CObject
{
public:
	CMenuManager();
	virtual ~CMenuManager();

	virtual HRESULT Init(void) override = 0;
	virtual void Uninit(void) override;
	virtual void Update(void) override = 0;
	virtual void Draw(void) override = 0;
protected:
	void AddList(MenuEntry::Menu& Menu) { m_vMenuEntries.push_back(std::move(Menu)); }
	std::vector<MenuEntry::Menu>& GetList(void) { return m_vMenuEntries; }
private:
	std::vector<MenuEntry::Menu> m_vMenuEntries;	// メニューの構成情報
	// 派生でm_nCurrentIndex(現在選択中のインデックス)を追加してください
};
#endif