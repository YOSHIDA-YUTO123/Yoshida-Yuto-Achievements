//===================================================
//
// ジェットパックモデルのデータ [jet_pack_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _JET_PACK_COMPONENT_HPP_
#define _JET_PACK_COMPONENT_HPP_

//***************************************************
// ジェットパックモデルのデータ構造体の定義
//***************************************************
struct JetPackComponent
{
	static constexpr int NUM_LOCATOR = 2; // ロケーターの数

	JetPackComponent(entt::entity _ownerID) :
		ownerID(_ownerID), fRotValue(0.0f)
	{}

	std::array<entt::entity, NUM_LOCATOR> effectLocator;	// エフェクトの位置
	entt::entity ownerID;									// オーナーのID
	float fRotValue;										// 回転量
};
#endif