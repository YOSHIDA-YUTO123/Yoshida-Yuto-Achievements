//===================================================
//
// メッシュの球体の更新処理 [mesh_sphere_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_sphere_system.h"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "transform_component.hpp"
#include "mesh_sphere_component.hpp"
#include "utility_math.h"

//===================================================
// 更新処理
//===================================================
void MeshSphereSystem::Update(entt::registry& registry)
{
	// メッシュの球体のコンポーネントの取得
	auto view = registry.view<Tag::MeshSphereTag>();

	// メッシュの球体分回す
	for (auto entity : view)
	{
		// メッシュの頂点のコンポーネント
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

		// 頂点バッファの取得
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		// 球体のコンポーネント
		auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);

		// 空間情報のコンポーネント
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// 頂点のカウント
		int nCntVtx = 0;

		// 計算用の位置
		D3DXVECTOR3 posWk;

		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファをロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	
		// 反転するなら裏返す
		float fHeight = meshSphereComp.bReverse ? -meshSphereComp.fHeight : meshSphereComp.fHeight;

		pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);
		pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
		pVtx[nCntVtx].col = meshVtxComp.col;

		nCntVtx++;

		// 反転するなら裏返す
		float fRadius = meshSphereComp.bReverse ? -meshSphereComp.fRadius : meshSphereComp.fRadius;

		// 横の分割数分回す
		for (int nCntU = 0; nCntU <= meshVtxComp.nSegmentU; nCntU++)
		{
			// 半周の割合(緯度)
			float fAngleX = D3DX_PI / meshVtxComp.nSegmentV;

			// 一周の割合を求める
			float fAngleY = (D3DX_PI * 2.0f) / meshVtxComp.nSegmentU * nCntU;

			posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
			posWk.y = cosf(fAngleX) * fHeight;
			posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

			pVtx[nCntVtx].pos = posWk;
			pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
			pVtx[nCntVtx].col = meshVtxComp.col;

			nCntVtx++;
		}

		// 半周の割合(緯度)
		float fOffset = D3DX_PI / meshVtxComp.nSegmentV;

		// 縦の分割数
		for (int nCntV = 0; nCntV <= meshVtxComp.nSegmentV; nCntV++)
		{
			// 半周の割合(緯度)
			float fAngleX = ((D3DX_PI - fOffset * 2.0f) / meshVtxComp.nSegmentV * nCntV) + fOffset;

			// 横の分割数
			for (int nCntU = 0; nCntU <= meshVtxComp.nSegmentU; nCntU++)
			{
				// 一周の割合を求める
				float fAngleY = (-D3DX_PI * 2.0f) / meshVtxComp.nSegmentU * nCntU;

				posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
				posWk.y = cosf(fAngleX) * fHeight;
				posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

				pVtx[nCntVtx].pos = posWk;
				pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
				pVtx[nCntVtx].col = meshVtxComp.col;

				nCntVtx++;
			}
		}

		// 横の分割数分回す
		for (int nCntU = 0; nCntU <= meshVtxComp.nSegmentU; nCntU++)
		{
			// 半周の割合(緯度)
			float fAngleX = D3DX_PI - (D3DX_PI / meshVtxComp.nSegmentV);

			// 一周の割合を求める
			float fAngleY = (D3DX_PI * 2.0f) / meshVtxComp.nSegmentU * nCntU;

			posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
			posWk.y = cosf(fAngleX) * fHeight;
			posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

			pVtx[nCntVtx].pos = posWk;
			pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
			pVtx[nCntVtx].col = meshVtxComp.col;

			nCntVtx++;
		}

		pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, -fHeight, 0.0f);
		pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
		pVtx[nCntVtx].col = meshVtxComp.col;

		nCntVtx++;

		// 頂点バッファをアンロック
		vertexBufferComp.pVtxBuffer->Unlock();
	}
}