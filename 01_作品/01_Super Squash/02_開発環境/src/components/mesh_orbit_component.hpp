//===================================================
//
// メッシュの軌跡のデータ [mesh_orbit_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_ORBIT_COMPONENT_HPP_
#define _MESH_ORBIT_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// メッシュの軌跡のデータの定義
//***************************************************
struct MeshOrbitComponent
{
	MeshOrbitComponent(const entt::entity _owner, const D3DXVECTOR3& _top, const D3DXVECTOR3& _bottom) :
		ownerID(_owner), topLocal(_top), bottomLocal(_bottom),
		top(_top), bottom(_bottom)
	{}

	entt::entity ownerID;		// オーナーのID		
	D3DXVECTOR3 topLocal;		// 上のローカル位置
	D3DXVECTOR3 bottomLocal;	// 下のローカル位置
	D3DXVECTOR3 top;			// 上の位置
	D3DXVECTOR3 bottom;			// 下の位置
};
#endif