//===================================================
//
//  エフェクトの壁の当たり判定の処理 [collision_effect_wall_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "collision_effect_wall_system.h"
#include "tag_component.hpp"
#include "utility_math.h"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "transform_component.hpp"
#include "collision_effect_wall_component.hpp"
#include "factory_billboard.h"
#include "sphere_collider_component.hpp"

#ifdef _DEBUG
#include "line.h"
#include "color_constants.hpp"
#endif // _DEBUG

//===================================================
// 更新処理
//===================================================
void CollisionEffectWallSystem::Update(entt::registry& registry)
{
	// エフェクトの壁の取得
	auto view = registry.view<Tag::EffectMeshWallTag>();

	// 取得した分回す
	for (auto entity : view)
	{
		Collision(registry, entity);
	}
}

//===================================================
// 当たり判定処理
//===================================================
bool CollisionEffectWallSystem::Collision(entt::registry& registry, const entt::entity wall)
{
	bool bCollision = false;

	VERTEX_3D* pVtx = nullptr;

	// エフェクトの壁の当たり判定の取得
	auto view = registry.view<ColliderTag::Wall>();

	// 頂点バッファのコンポーネント
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(wall);

	// 頂点のコンポーネント
	auto& meshVtxComp = registry.get<MeshVtxComponent>(wall);

	// メッシュの位置の取得
	auto& meshTransform = registry.get<Transform3DComponent>(wall);

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& sphereColliderComp = registry.get<SphereColliderComponent>(entity);
		auto& collisionComp = registry.get<CollisionEffectWallComponent>(sphereColliderComp.ownerID);
		auto& otherTransformComp = registry.get<Transform3DComponent>(sphereColliderComp.ownerID);

		// 頂点バッファをロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点左上
		D3DXVECTOR3 vtxTopLeft = pVtx[0].pos;

		// 頂点右上
		D3DXVECTOR3 vtxTopRight = pVtx[meshVtxComp.nSegmentU].pos;

		// 左下の頂点のインデックス
		const int nBottomLeftIndex = meshVtxComp.nNumVertex - meshVtxComp.nSegmentU - 1;

		// 左下の位置
		D3DXVECTOR3 vtxBottomLeft = pVtx[nBottomLeftIndex].pos;

		// ワールド座標に変換
		D3DXVec3TransformCoord(&vtxTopLeft, &vtxTopLeft, &meshTransform.mtxWorld);
		D3DXVec3TransformCoord(&vtxTopRight, &vtxTopRight, &meshTransform.mtxWorld);
		D3DXVec3TransformCoord(&vtxBottomLeft, &vtxBottomLeft, &meshTransform.mtxWorld);

		// 法線を求める
		D3DXVECTOR3 normal = math::Cross((vtxTopLeft - vtxTopRight), (vtxTopLeft - vtxBottomLeft));
		D3DXVec3Normalize(&normal, &normal);

		// 面の方向にどれだけ進んでるかを求める
		float fProjDistance = math::Dot(otherTransformComp.pos - vtxTopLeft, normal);

		// 投影した位置を求める
		D3DXVECTOR3 projPos = otherTransformComp.pos - normal * fProjDistance;
																																																																																																																																																																																																									
		// 内積で壁の裏側に行ったかどうか判定
		float dot = math::Dot(otherTransformComp.pos - meshTransform.pos, normal);

		// 頂点バッファをアンロック
		vertexBufferComp.pVtxBuffer->Unlock();

		// 球体コライダーの半径
		float fRadius = sphereColliderComp.fRadius;

		// 移動量を幅を求める
		D3DXVECTOR3 vecMove = otherTransformComp.posOld - otherTransformComp.pos;
		float fMoveLength = D3DXVec3Length(&vecMove);

		// 移動幅が自分の半径を超えていたら
		if (fMoveLength >= fRadius)
		{
			// 半径を拡大(速すぎて裏側に行き抜けるため
			fRadius *= 3.0f;
		}

		// 投影した位置との距離を求める
		float fDistance = math::GetDistance(projPos - otherTransformComp.pos);

		// 距離が半径以下だったら or 裏側に行ったら
		if (fDistance <= fRadius || dot <= 0.0f)
		{
			// IDの取得
			collisionComp.wallID = wall;
			collisionComp.bCollision = true;
			collisionComp.hitPos = projPos;
			bCollision = true;
		}
	}

	return bCollision;
}