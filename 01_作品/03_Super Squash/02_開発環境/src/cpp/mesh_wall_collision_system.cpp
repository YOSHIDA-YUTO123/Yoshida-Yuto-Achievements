//===================================================
//
// メッシュウォールの処理 [mesh_wall_collision_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_wall_collision_system.h"
#include "mesh_wall_collision_component.hpp"
#include "mesh_vtx_component.hpp"
#include "size_component.hpp"
#include "vertex_buffer_component.hpp"
#include "transform_component.hpp"
#include "tag_component.hpp"
#include "sphere_component.hpp"
#include "debug_proc.h"
#include "sphere_collider_component.hpp"
#include "factory_billboard.h"
#include "utility_math.h"
#include "collision.h"
#include "mesh_wall_component.hpp"

#ifdef _DEBUG
#include "line.h"
#include "color_constants.hpp"
#endif

//===================================================
// 更新処理
//===================================================
void MeshWallCollisionSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<ColliderTag::Wall>();

	// 指定したコンポーネントを持つエンティティの取得
	auto wallView = registry.view<Tag::MeshWallTag>();

	bool bHit = false;

	// 壁の数分回す
	for (auto wall : wallView)
	{
		// 当たり判定コンポーネント分回す
		for (auto collision : view)
		{
			// 判定処理
			CheckCollision(registry, collision, wall, &bHit);
		}
	}
}

//===================================================
// 判定処理
//===================================================
void MeshWallCollisionSystem::CheckCollision(entt::registry& registry, const entt::entity collisionID, const entt::entity wallID, bool* pHit)
{
	// コンポーネントを取得
	auto& colliderSphere = registry.get<SphereColliderComponent>(collisionID);
	auto& collisionComp = registry.get<MeshWallCollisionComponent>(colliderSphere.ownerID);
	auto& otherTransformComp = registry.get<Transform3DComponent>(colliderSphere.ownerID);
	auto& meshVtxComp = registry.get<MeshVtxComponent>(wallID);
	auto& sizeComp = registry.get<Size3DComponent>(wallID);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(wallID);
	auto& transformComp = registry.get<Transform3DComponent>(wallID);

	// 当たってないなら //out
	if (*pHit == false)
	{
		collisionComp.bCollision = false;
	}

	int nSegH = meshVtxComp.nSegmentU;
	int nSegV = meshVtxComp.nSegmentV;

	// 1マスのサイズ
	float GridSizeX = sizeComp.size.x / (float)nSegH;
	float GridSizeY = sizeComp.size.y / (float)nSegV;

	D3DXVECTOR3 localPos;

	// 逆行列
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, &transformComp.mtxWorld); // ワールド行列の逆行列

	// 回転を打ち消した座標
	D3DXVec3TransformCoord(&localPos, &otherTransformComp.pos, &invWorld);

	D3DXVECTOR3 fieldPos;
	D3DXVECTOR3 halfSize = sizeComp.size * 0.5f;

	float X = localPos.x + halfSize.x;
	float Y = halfSize.y - localPos.y;

	// 何番目のポリゴンか
	int polyX = (int)(X / GridSizeX);
	int polyY = (int)(Y / GridSizeY);

	// 現在のポリゴンのインデックス番号
	int polyIndex = ((polyY * (nSegH - 1) + polyX) * 2) + (polyY * 6);

	// ポリゴン数の設定
	int nNumPolygon = ((nSegH * nSegV) * 2) + (4 * (nSegV - 1));

	// インデックス数の設定
	int nNumIndex = nNumPolygon + 2;

	VERTEX_3D_MULT* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	vertexBufferComp.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	// 判定するポリゴンの数
	const int judgPolygon = 2;

	// すべてのインデックスを調べる
	for (int nCnt = 0; nCnt < judgPolygon; nCnt++)
	{
		// 頂点のインデックス
		int nCntVertex = (polyIndex + nCnt);

		nCntVertex = math::Clamp(nCntVertex, 0, nNumIndex - 2);

		// インデックスを取得
		int nIdx0 = pIdx[nCntVertex];
		int nIdx1 = pIdx[nCntVertex + 1];
		int nIdx2 = pIdx[nCntVertex + 2];

		// 頂点の位置を取得
		D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
		D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

		// 頂点の位置をワールド座標にする
		D3DXVec3TransformCoord(&vtx0, &pVtx[nIdx0].pos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&vtx1, &pVtx[nIdx1].pos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&vtx2, &pVtx[nIdx2].pos, &transformComp.mtxWorld);

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

		D3DXVECTOR3 normal = {};

		if (nCnt % 2 == 0)
		{
			// 偶数番目の三角形
			normal = math::Cross(edge0, edge1);
		}
		else
		{
			// 奇数番目の三角形（順序が逆になっている）
			normal = math::Cross(edge1, edge0);
		}

		// 地面の法線を求める
		D3DXVec3Normalize(&normal, &normal);

		// 判定回数
		const int MAX_CHECK = 5;

		D3DXVECTOR3 push = Const::VEC3_NULL;
		D3DXVECTOR3 Finalpush = Const::VEC3_NULL;
		float fMaxLength = 0.0f;
		bool bCollision = false;

		// 球体コライダーの半径
		float fRadius = colliderSphere.fRadius;

		// 移動量を幅を求める
		D3DXVECTOR3 vecMove = otherTransformComp.posOld - otherTransformComp.pos;
		float fMoveLength = D3DXVec3Length(&vecMove);

		// 移動幅が自分の半径を超えていたら
		if (fMoveLength >= fRadius)
		{
			// 半径を拡大(速すぎて裏側に行き抜けるため
			fRadius *= 3.0f;
		}

		// 押し出し処理
		auto tryPush = [&](const D3DXVECTOR3& push)
		{
			bCollision = true;

			// 押し出し量を求める
			float fLength = D3DXVec3Length(&push);

			// 押し出し量がいちばん大きいなら
			if (fLength > fMaxLength)
			{
				// 押し出し量を設定
				Finalpush = push;

				// 最大量を更新
				fMaxLength = fLength;

				collisionComp.bCollision = true;
				collisionComp.meshID = wallID;
				collisionComp.normal = normal;
				*pHit = true;
			}
		};

		// 三角形内にいるか判定
		if (CheckClosestPosInTriangle(
			otherTransformComp.pos,
			vtx0, vtx1, vtx2,
			normal,
			fRadius,
			push, registry))
		{
			tryPush(push);
		}

		// エッジの判定
		if (CheckEdgeOnPos(
			otherTransformComp.pos,
			vtx0, vtx1, vtx2,
			fRadius,
			push, registry, normal))
		{
			tryPush(push);
		}

		// 頂点の判定
		if (CheckVtxOnPos(
			otherTransformComp.pos,
			vtx0, vtx1, vtx2,
			fRadius,
			normal,
			push,
			registry))
		{
			tryPush(push);
		}

		if (bCollision)
		{
			// 押し出す
			otherTransformComp.pos += Finalpush;
			otherTransformComp.pos += normal * fRadius * 0.1f;

			// 衝突点の設定
			collisionComp.hitPos = otherTransformComp.pos;
		}

		// 当たっているなら処理を飛ばす
		if (bCollision)
		{
			continue;
		}

		// 頂点からプレイヤーまでのベクトルの計算
		D3DXVECTOR3 vecToPlayerVtx = otherTransformComp.pos - vtx0;
		D3DXVec3Normalize(&vecToPlayerVtx, &vecToPlayerVtx);

		// 三角形の中にいないなら処理を飛ばす
		if (!math::InTriangle(otherTransformComp.pos, vtx0, vtx1, vtx2))
		{
			continue;
		}
		
		// プレイヤーまでのベクトルと法線の内積
		float dot = math::Dot(vecToPlayerVtx, normal);

		// ポリゴンの裏側に行っているなら
		if (dot < 0.0f)
		{
			// 平面を求める
			float D = math::Dot(normal, vtx0);

			// 法線方向にどれだけ離れているか - 平面で距離を求める
			float fDistance = math::Dot(normal, otherTransformComp.pos) - D;

			// 押し出す
			otherTransformComp.pos -= normal * fDistance;
			otherTransformComp.pos += normal * fRadius;

			collisionComp.hitPos = otherTransformComp.pos;
			collisionComp.bCollision = true;
			collisionComp.meshID = wallID;
			collisionComp.normal = normal;
			*pHit = true;
		}
		
	}

	// インデックスバッファのアンロック
	vertexBufferComp.pIdxBuffer->Unlock();

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();
}

//===================================================
// 投影点が三角形の中にいるかどうか
//===================================================
bool MeshWallCollisionSystem::CheckClosestPosInTriangle(
	const D3DXVECTOR3& sphere, const D3DXVECTOR3& vtx0,
	const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2,
	const D3DXVECTOR3& normal,const float fRadius,
	D3DXVECTOR3& push, entt::registry& reg)
{
	(void)reg;

	// 法線と頂点までのベクトルの内積
	float fDist = math::Dot(sphere - vtx0, normal);

	// 三角形の平面に投影した点
	D3DXVECTOR3 closestPos = sphere - normal * fDist;

	// 投影点が三角形の中にいるかどうか
	if (math::InTriangle(closestPos, vtx0, vtx1, vtx2))
	{
		// 投影点と球の中心との距離を求める
		float fDistance = math::GetDistance(sphere - closestPos);

		// 距離が半径より短いなら
		if (fDistance <= fRadius)
		{
#ifdef _DEBUG
#else
			(void)reg;
#endif // _DEBUG

			// めり込んだ深さを求める
			float fDepth = fRadius - fDistance;

			// 押し出し方向の設定
			push = fDepth * normal;

			return true;
		}
	}

	return false;
}

//===================================================
// 辺に当たっているかどうか
//===================================================
bool MeshWallCollisionSystem::CheckEdgeOnPos(
	const D3DXVECTOR3& sphere,const D3DXVECTOR3& vtx0,
	const D3DXVECTOR3& vtx1,const D3DXVECTOR3& vtx2,
	const float fRadius,D3DXVECTOR3& push, entt::registry& reg,
	const D3DXVECTOR3& Normal)
{
	(void)reg;

	struct CheckEdge_Info
	{
		float fDistance = FLT_MAX;
		D3DXVECTOR3 nearEdgePos;
	};

	const int NUM_EDGE = 3;

	CheckEdge_Info check_info[NUM_EDGE];

	// 最近接点を求める
	D3DXVECTOR3 nearEdgePos1 = ClosestPointOnSegment(sphere, vtx0, vtx1);
	D3DXVECTOR3 nearEdgePos2 = ClosestPointOnSegment(sphere, vtx1, vtx2);
	D3DXVECTOR3 nearEdgePos3 = ClosestPointOnSegment(sphere, vtx2, vtx0);

	// 距離を求める
	float fDistance1 = math::GetDistance(sphere - nearEdgePos1);
	float fDistance2 = math::GetDistance(sphere - nearEdgePos2);
	float fDistance3 = math::GetDistance(sphere - nearEdgePos3);

	check_info[0].fDistance = fDistance1;
	check_info[0].nearEdgePos = nearEdgePos1;

	check_info[1].fDistance = fDistance2;
	check_info[1].nearEdgePos = nearEdgePos2;

	check_info[2].fDistance = fDistance3;
	check_info[2].nearEdgePos = nearEdgePos3;

	// 最小の距離
	float fDistanceMin = check_info[0].fDistance;
	D3DXVECTOR3 nearPos = check_info[0].nearEdgePos;

	// いちばん近い辺を求める
	for (int nCntEdge = 1; nCntEdge < NUM_EDGE; nCntEdge++)
	{
		// いちばん近い辺の情報を保存
		if (check_info[nCntEdge].fDistance < fDistanceMin)
		{
			fDistanceMin = check_info[nCntEdge].fDistance;
			nearPos = check_info[nCntEdge].nearEdgePos;;
		}
	}

	// 距離が半径以内だったら当たっている
	if (fDistanceMin <= fRadius)
	{
		// めり込んだ深さを求める
		float fDepth = fRadius - fDistanceMin;

		D3DXVECTOR3 dir = Const::VEC3_NULL;

		// 押し出し方向の設定
		push = fDepth * Normal;

		return true;
	}

	return false;
}

//===================================================
// 頂点との判定
//===================================================
bool MeshWallCollisionSystem::CheckVtxOnPos(
	const D3DXVECTOR3& sphere, const D3DXVECTOR3& vtx0,
	const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2,
	const float fRadius, const D3DXVECTOR3& normal,
	D3DXVECTOR3& push, entt::registry& reg)
{
	(void)reg;

	// 頂点数
	const int NUM_VERTEX = 3;

	// 三角形の頂点
	D3DXVECTOR3 vtx[NUM_VERTEX] = { vtx0,vtx1,vtx2 };
	float fDistanceVtx[NUM_VERTEX] = {};

	// 頂点数分回す
	for (int nCntVtx = 0; nCntVtx < NUM_VERTEX; nCntVtx++)
	{
		// 距離の設定
		fDistanceVtx[nCntVtx] = math::GetDistance(sphere - vtx[nCntVtx]);
	}

	// 最小の距離
	float fDistanceMin = fDistanceVtx[0];

	// 最近位置
	D3DXVECTOR3 nearVtx = vtx[0];

	// 頂点数分回す
	for (int nCntVtx = 1; nCntVtx < NUM_VERTEX; nCntVtx++)
	{
		// 最小の距離、位置を求める
		if (fDistanceVtx[nCntVtx] < fDistanceMin)
		{
			fDistanceMin = fDistanceVtx[nCntVtx];
			nearVtx = vtx[nCntVtx];
		}
	}

	// 距離が半径以下だったら当たってる
	if (fDistanceMin <= fRadius)
	{
		// めり込んだ深さを求める
		float fDepth = fRadius - fDistanceMin;

#ifdef _DEBUG
#else
		(void)reg;
#endif

		// 押し出し方向の設定
		push = fDepth * normal;

		return true;
	}

	return false;
}

//===================================================
// 射影点が外なら、各辺に対しての最短距離を取る
//===================================================
D3DXVECTOR3 MeshWallCollisionSystem::ClosestPointOnSegment(const D3DXVECTOR3& spherePos, const D3DXVECTOR3& startVtx, const D3DXVECTOR3& endVtx)
{
	// 辺を求める
	D3DXVECTOR3 segment = endVtx - startVtx;

	float fLengthsq = D3DXVec3LengthSq(&segment);

	// 同じ点だったら
	if (fLengthsq == 0.0f) return startVtx;

	// 始点から球の中心までのベクトル
	D3DXVECTOR3 StartToSphereVec = spherePos - startVtx;

	// 長さで割る
	float t = math::Dot(StartToSphereVec, segment) / fLengthsq;

	t = max(0.0f, min(1.0f, t));

	return startVtx + segment * t;
}