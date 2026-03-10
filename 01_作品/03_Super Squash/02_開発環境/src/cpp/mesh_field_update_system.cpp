//===================================================
//
// メッシュフィールドの更新処理 [mesh_field_update_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_field_update_system.h"
#include "tag_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"

//===================================================
// 更新処理
//===================================================
void MeshFieldUpdateSystem::Update(entt::registry& registry)
{
	// 指定したデータを持つエンティティの取得
	auto view = registry.view<Tag::MeshFieldTag>();

	// 指定したデータを持つエンティティ分回す
	for (auto entity : view)
	{
		// 法線の再設定
		ResetNormal(entity, registry);
	}
}

//===================================================
// 法線の再設定処理
//===================================================
void MeshFieldUpdateSystem::ResetNormal(entt::entity entity, entt::registry& registry)
{
	// コンポーネントの取得
	auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
	auto& vtxBufferComp = registry.get<VertexBufferComponent>(entity);

	int nCnt = 0;
	int nSegH = meshVtxComp.nSegmentU;
	int nSegV = meshVtxComp.nSegmentV;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vtxBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点数分調べる
	for (int nCntZ = 0; nCntZ <= nSegV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= nSegH; nCntX++)
		{
			// 計算用頂点0,1,2,3,4
			D3DXVECTOR3 vtx0, vtx1, vtx2, vtx3, vtx4;

			// 計算用頂点を初期化
			vtx0 = vtx1 = vtx2 = vtx3 = vtx4 = Const::VEC3_NULL;

			// 計算用ベクトル0,1,2,3
			D3DXVECTOR3 vec0, vec1, vec2, vec3;

			// 計算用ベクトルの初期化
			vec0 = vec1 = vec2 = vec3 = Const::VEC3_NULL;

			// 計算用インデックス0,1,2,3の宣言
			int nIdx0 = 0;
			int nIdx1 = 0;
			int nIdx2 = 0;
			int nIdx3 = 0;

			// 計算用法線0,1,2,3
			D3DXVECTOR3 nor0, nor1, nor2, nor3;

			// 計算用法線の初期化
			nor0 = nor1 = nor2 = nor3 = Const::VEC3_NULL;

			// 計算後の法線格納用変数
			D3DXVECTOR3 normal = Const::VEC3_NULL;

			// 左の上だったら
			if (nCntX == 0)
			{
				// 左上だったら
				if (nCntZ == 0)
				{
					nIdx0 = 0;
					nIdx1 = 1;
					nIdx2 = nSegH + 1;

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;

					vec0 = vtx1 - vtx0;
					vec1 = vtx2 - vtx0;

					D3DXVec3Cross(&normal, &vec0, &vec1);
				}
				// 左下だったら
				else if (nCntZ == nSegV)
				{
					nIdx0 = (nSegH + 1) * nSegV;
					nIdx1 = (nSegH + 1) * (nSegV - 1);
					nIdx2 = ((nSegH + 1) * nSegV) + 1;

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;

					vec0 = vtx1 - vtx0;
					vec1 = vtx2 - vtx0;

					D3DXVec3Cross(&normal, &vec0, &vec1);
				}
				// 左の辺(角以外)だったら
				else if (nCnt == (nSegH + 1) * nCntZ)
				{
					nIdx0 = nCnt - (nSegH + 1);
					nIdx1 = nCnt + 1;
					nIdx2 = nCnt + (nSegH + 1);

					vtx0 = pVtx[nIdx0].pos;
					vtx1 = pVtx[nIdx1].pos;
					vtx2 = pVtx[nIdx2].pos;
					vtx3 = pVtx[nCnt].pos;

					vec0 = vtx0 - vtx3;
					vec1 = vtx1 - vtx3;
					vec2 = vtx2 - vtx3;

					D3DXVec3Cross(&nor0, &vec0, &vec1);
					D3DXVec3Cross(&nor1, &vec1, &vec2);

					normal = (nor0 + nor1) * 0.5f;
				}
			}
			// 上の辺だったら
			else if (nCntZ == 0 && nCnt < nSegH)
			{
				nIdx0 = (nSegH + 1) + nCntX;
				nIdx1 = nCnt - 1;
				nIdx2 = nCnt + 1;

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;
				vtx3 = pVtx[nIdx0].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;
				vec2 = vtx3 - vtx0;

				D3DXVec3Cross(&nor0, &vec2, &vec0);
				D3DXVec3Cross(&nor1, &vec1, &vec2);

				normal = (nor0 + nor1) * 0.5f;
			}
			// 右上だったら
			else if (nCntX == nSegH && nCntZ == 0)
			{
				nIdx0 = nSegH;
				nIdx1 = nSegH - 1;
				nIdx2 = nSegH + 1 + nCntX;

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&normal, &vec1, &vec0);
			}
			// 下の辺だったら
			else if (nCntZ == nSegV && nCnt < ((nSegH + 1) * (nSegV + 1)) - 1)
			{
				nIdx0 = nCnt - 1;
				nIdx1 = nCnt - (nSegH + 1);
				nIdx2 = nCnt + 1;

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx0].pos;
				vtx2 = pVtx[nIdx1].pos;
				vtx3 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;
				vec2 = vtx3 - vtx0;

				D3DXVec3Cross(&nor0, &vec0, &vec1);
				D3DXVec3Cross(&nor1, &vec1, &vec2);

				normal = (nor0 + nor1) * 0.5f;
			}
			// 右下だったら
			else if (nCnt == ((nSegH + 1) * (nSegV + 1)) - 1)
			{
				nIdx0 = ((nSegH + 1) * (nSegV + 1)) - 1;
				nIdx1 = nIdx0 - (nSegH + 1);
				nIdx2 = nIdx0 - 1;

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&normal, &vec1, &vec0);
			}
			// 右の辺(角以外)だったら
			else if (nCntX == nSegH && nCnt == (nCntX * (nCntZ + 1)) + nCntZ)
			{
				nIdx0 = nCnt - (nSegH + 1);
				nIdx1 = nCnt - 1;
				nIdx2 = nCnt + (nSegH + 1);

				vtx0 = pVtx[nIdx0].pos;
				vtx1 = pVtx[nIdx1].pos;
				vtx2 = pVtx[nIdx2].pos;
				vtx3 = pVtx[nCnt].pos;

				vec0 = vtx0 - vtx3;
				vec1 = vtx1 - vtx3;
				vec2 = vtx2 - vtx3;

				D3DXVec3Cross(&nor0, &vec1, &vec0);
				D3DXVec3Cross(&nor1, &vec2, &vec1);

				normal = (nor0 + nor1) * 0.5f;
			}
			// それ以外(端っこでも角でもない)
			else
			{
				nIdx0 = nCnt - 1;
				nIdx1 = nCnt - (nSegH + 1);
				nIdx2 = nCnt + 1;
				nIdx3 = nCnt + (nSegH + 1);

				vtx0 = pVtx[nCnt].pos;
				vtx1 = pVtx[nIdx0].pos;
				vtx2 = pVtx[nIdx1].pos;
				vtx3 = pVtx[nIdx2].pos;
				vtx4 = pVtx[nIdx3].pos;

				vec0 = vtx1 - vtx0; // 左
				vec1 = vtx2 - vtx0; // 上

				vec2 = vtx3 - vtx0; // 右
				vec3 = vtx4 - vtx0; // 下
				
				D3DXVec3Cross(&nor0, &vec0, &vec1);
				D3DXVec3Cross(&nor1, &vec3, &vec0);
				D3DXVec3Cross(&nor2, &vec1, &vec2);
				D3DXVec3Cross(&nor3, &vec2, &vec3);

				// 平均値を求める
				normal = (nor0 + nor1 + nor2 + nor3) * 0.25f;
			}

			// 正規化
			D3DXVec3Normalize(&normal, &normal);

			// 法線の再設定
			pVtx[nCnt].nor = normal;

			nCnt++;
		}
	}

	// 頂点バッファをアンロック
	vtxBufferComp.pVtxBuffer->Unlock();
}
