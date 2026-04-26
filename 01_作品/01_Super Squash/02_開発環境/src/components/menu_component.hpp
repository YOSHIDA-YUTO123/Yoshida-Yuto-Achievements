//===================================================
//
// メニューのコンポーネント [menu_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_COMPONENT_HPP_
#define _MENU_COMPONENT_HPP_

//***************************************************
// メニューのデータの定義
//***************************************************
struct MenuComponent
{
	MenuComponent(const entt::entity _selectMenuID, const int _nID = -1) :
		selectMenuID(_selectMenuID), nID(_nID)
	{}

	entt::entity selectMenuID;		// メニューの選択処理のID
	const int nID;					// メニューのID
};

#endif