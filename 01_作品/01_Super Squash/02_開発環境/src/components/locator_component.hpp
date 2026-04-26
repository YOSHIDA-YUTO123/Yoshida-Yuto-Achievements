//===================================================
//
// ロケーターのデータ [locator_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LOCATOR_COMPONENT_HPP_
#define _LOCATOR_COMPONENT_HPP_

//***************************************************
// ロケーターのデータの定義
//***************************************************
struct LocatorComponent
{
	LocatorComponent(const entt::entity _locator) :
		locatorID(_locator) {}

	entt::entity locatorID; // ロケーターのID
};
#endif