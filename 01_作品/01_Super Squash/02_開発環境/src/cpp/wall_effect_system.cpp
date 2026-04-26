//===================================================
//
// 壁のエフェクトの更新処理 [wall_effect_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "wall_effect_system.h"
#include "tag_component.hpp"
#include "wall_effect_component.hpp"
#include "mesh_vtx_component.hpp"
#include "vertex_buffer_component.hpp"
#include "utility_math.h"
#include "color_constants.hpp"
#include "transform_component.hpp"

//===================================================
// 更新処理
//===================================================
void WallEffectSystem::Update(entt::registry& registry)
{
	// 壁のエフェクトの取得
	auto view = registry.view<WallEffectComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& effectComp = registry.get<WallEffectComponent>(entity);

		// IDが無いなら
		if (!registry.valid(effectComp.wallID))
		{
			continue;
		}

		auto& meshVtxComp = registry.get<MeshVtxComponent>(effectComp.wallID);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(effectComp.wallID);
		auto& transformComp = registry.get<Transform3DComponent>(effectComp.wallID);

		// 頂点バッファへのポインタ
		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファのロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		float fRate = effectComp.nTime / static_cast<float>(effectComp.nMaxTime);

		// 頂点数分回す
		for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
		{
			D3DXVECTOR3 vtxPos = pVtx[nCnt].pos;

			D3DXVec3TransformCoord(&vtxPos, &vtxPos, &transformComp.mtxWorld);

			// 距離を求める
			float fDistance = math::GetDistance(effectComp.epicenter - vtxPos);

			// 範囲外だったら
			if (fDistance >= effectComp.fRadius) continue;
			
			// 確率を求める
			float fProbAlpha = math::Random(0.0f, 1.0f);

			// 色
			D3DXCOLOR col = meshVtxComp.col;

			// effectComp.fRoughの確率で光らせる
			float fAlpha = (fProbAlpha > effectComp.fRough) ? math::Random(col.a, fRate) : col.a;

			// 発生時間終了なら
			if (effectComp.nTime <= 0)
			{
				// もとに戻す
				pVtx[nCnt].col = D3DXCOLOR(
					col.r,
					col.g,
					col.b,
					col.a);

				continue;
			}

			pVtx[nCnt].col = D3DXCOLOR(
				col.r,
				col.g,
				col.b,
				fAlpha);
			
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pVtxBuffer->Unlock();

		if (effectComp.nTime <= 0)
		{
			registry.destroy(entity);
			continue;
		}

		effectComp.nTime--;
	}
}