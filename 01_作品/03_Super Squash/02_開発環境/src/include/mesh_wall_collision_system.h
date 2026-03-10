//===================================================
//
// メッシュウォールの処理 [mesh_wall_collision_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHWALL_COLLISION_SYSTEM_H_
#define _MESHWALL_COLLISION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct Transform3DComponent;

//***************************************************
// メッシュウォールの処理の定義
//***************************************************
struct MeshWallCollisionSystem : SystemBase
{
	MeshWallCollisionSystem() = default;
	~MeshWallCollisionSystem() = default;

	void Update(entt::registry & registry) override;
private:
	void CheckCollision(entt::registry& registry, const entt::entity collisionID, const entt::entity wallID, bool* pHit);

	bool CheckClosestPosInTriangle(
		const D3DXVECTOR3& sphere, const D3DXVECTOR3& vtx0  , const D3DXVECTOR3& vtx1,
		const D3DXVECTOR3& vtx2  , const D3DXVECTOR3& normal, const float fRadius,
		D3DXVECTOR3& push,entt::registry& reg);

	bool CheckEdgeOnPos(
		const D3DXVECTOR3& sphere, const D3DXVECTOR3& vtx0,
		const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2,
		const float fRadius, D3DXVECTOR3& push, entt::registry& reg, const D3DXVECTOR3& normal);

	bool CheckVtxOnPos(
		const D3DXVECTOR3& sphere, const D3DXVECTOR3& vtx0,
		const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2,
		const float fRadius, const D3DXVECTOR3& normal, D3DXVECTOR3& push,
		entt::registry& reg);

	D3DXVECTOR3 ClosestPointOnSegment(const D3DXVECTOR3& spherePos, const D3DXVECTOR3& startVtx, const D3DXVECTOR3& endVtx);
};
#endif