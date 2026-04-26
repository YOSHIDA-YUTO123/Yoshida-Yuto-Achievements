//===================================================
//
// 影のデータ [shadow_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADOW_COMPONENT_HPP_
#define _SHADOW_COMPONENT_HPP_

//***************************************************
// キャラクターのデータの定義
//***************************************************
struct ShadowComponent
{
	ShadowComponent(const entt::entity _ownerID, const float _fMaxHeight, const float _fRadiusBase) :
		ownerID(_ownerID), fMaxHeight(_fMaxHeight), fRadiusBase(_fRadiusBase)
	{}

	const entt::entity ownerID; // 持ち主のID
	float fRadiusBase;			// 基準の半径
	float fMaxHeight;			// 表示される範囲
};
#endif