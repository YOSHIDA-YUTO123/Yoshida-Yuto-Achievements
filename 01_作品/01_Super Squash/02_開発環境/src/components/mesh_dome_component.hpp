//===================================================
//
// メッシュドームの頂点のデータ [mesh_dome_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_DOME_COMPONENT_HPP_
#define _MESH_DOME_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************
#include "array"

//***************************************************
// メッシュドームの頂点のデータ構造体の定義
//***************************************************
struct MeshDomeComponent
{
	// メッシュのコンポーネントの種類
	enum TYPE
	{
		TYPE_TOP = 0,	// 蓋の部分
		TYPE_SIDE,		// 横の部分
		TYPE_MAX
	};

	MeshDomeComponent(const bool _bRadius = false) :
		bReverse(_bRadius), vtxEntity() {}

	std::array<entt::entity, TYPE_MAX> vtxEntity;	// 頂点のコンポーネントをもつエンティティのID
	bool bReverse;									// 反転するかどうか
};
#endif