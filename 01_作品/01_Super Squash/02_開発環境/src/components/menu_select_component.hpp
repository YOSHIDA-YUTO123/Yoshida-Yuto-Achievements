//===================================================
//
// メニューの選択のコンポーネント [menu_select_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_SELECT_COMPONENT_HPP_
#define _MENU_SELECT_COMPONENT_HPP_

//***************************************************
// メニューの選択のデータの定義
//***************************************************
struct MenuSelectComponent
{
	static constexpr int START_MENU = 0; // 初期メニューのID

	MenuSelectComponent(const int _nMax, const int _nNextKey, const int _nNextJoyKey, 
		const int _nPrevKey, const int _nPrevJoyKey, const bool _bWrap, const int _nCurrent = START_MENU) :

		nCurrent(_nCurrent), nMax(_nMax), nNextKey(_nNextKey),
		nNextJoyKey(_nNextJoyKey), nPrevKey(_nPrevKey), nPrevJoyKey(_nPrevJoyKey),
		bWrap(_bWrap), bActive(false)
	{}

	int nCurrent;			// 現在の選択中のメニュー
	int nMax;				// メニューの最後尾

	int nNextKey;			// 次のメニューを選択するキー
	int nNextJoyKey;		// 次のメニューを選択するジョイパッドのキー

	int nPrevKey;			// 前のメニューを選択するキー
	int nPrevJoyKey;		// 前のメニューを選択するジョイパッドのキー

	bool bWrap;				// ループするかどうか
	bool bActive;			// 選択できるかどうか
};
#endif