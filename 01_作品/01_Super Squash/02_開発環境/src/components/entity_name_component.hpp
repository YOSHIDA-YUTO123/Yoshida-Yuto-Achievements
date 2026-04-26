//===================================================
//
// エンティティの名前のデータ [entity_name_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ENTITY_NAME_COMPONENT_HPP_
#define _ENTITY_NAME_COMPONENT_HPP_

//***************************************************
// エンティティの名前のデータの定義
//***************************************************
struct EntityNameComponent
{
	EntityNameComponent(const std::string& _name) :
		name(_name)
	{}

	const std::string name;	// 自分の名前
};
#endif