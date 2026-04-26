//===================================================
//
// 矩形コライダーのデータ [box_collider_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BOX_COLLIDER_COMPONENT_H_
#define _BOX_COLLIDER_COMPONENT_H_

//***************************************************
// 矩形コライダーのデータの定義
//***************************************************
struct BoxColliderComponent
{
	BoxColliderComponent(const D3DXVECTOR3& _size, const D3DXVECTOR3& _centerPosLocal, const entt::entity _ownerID) :
		centerPosLocal(_centerPosLocal), centerPos(_centerPosLocal), size(_size), ownerID(_ownerID)
	{}

	D3DXVECTOR3 centerPosLocal;		// 中心のローカル座標
	D3DXVECTOR3 centerPos;			// 中心のワールド座標
	D3DXVECTOR3 size;				// 大きさ
	entt::entity ownerID;			// オーナーのID
};
#endif