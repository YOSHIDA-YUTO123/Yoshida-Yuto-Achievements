//===================================================
//
// メッシュサークルの処理 [mesh_circle_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_circle_system.h"
#include "mesh_circle_component.hpp"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "utility_math.h"

//===================================================
// 更新処理
//===================================================
void MeshCircleSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<MeshCircleComponent>();

	// エンティティの数分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshCircleComp = registry.get<MeshCircleComponent>(entity);
		auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);
		auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

		// 頂点のカウント
		int nCntVtx = 0;

		// 計算用の位置
		D3DXVECTOR3 posWk;

		VERTEX_3D* pVtx = nullptr;

		// 頂点バッファをロック
		vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 内側の色の設定
		D3DXCOLOR Incol = meshVtxComp.col;
		Incol.a *= 0.5f;

		// 横の分割数分回す
		for (int nCntU = 0; nCntU <= meshVtxComp.nSegmentU; nCntU++)
		{
			// 一周の割合をもとめる
			float fAngle = (D3DX_PI * 2.0f) / meshVtxComp.nSegmentU * nCntU;

			// 位置の設定
			posWk.x = cosf(fAngle) * meshCircleComp.fInRadius;
			posWk.y = meshCircleComp.fHeight;;
			posWk.z = sinf(fAngle) * meshCircleComp.fInRadius;

			// 位置の設定
			pVtx[nCntVtx].pos = posWk;

			// 内側の色の設定
			pVtx[nCntVtx].col = meshCircleComp.inColor;

			nCntVtx++;
		}

		// 横の分割数分回す
		for (int nCntU = 0; nCntU <= meshVtxComp.nSegmentU; nCntU++)
		{
			// 一周の割合をもとめる
			float fAngle = (D3DX_PI * 2.0f) / meshVtxComp.nSegmentU * nCntU;

			// 位置の設定
			posWk.x = cosf(fAngle) * meshCircleComp.fOutRadius;
			posWk.y = 0.0f;
			posWk.z = sinf(fAngle) * meshCircleComp.fOutRadius;

			// 位置の設定
			pVtx[nCntVtx].pos = posWk;

			// 内側の色の設定
			pVtx[nCntVtx].col = meshCircleComp.outColor;

			nCntVtx++;
		}

		// 頂点バッファをアンロック
		vertexBufferComp.pVtxBuffer->Unlock();

		// 半径を拡大する
		meshCircleComp.fInRadius += meshCircleComp.fSpeed;
		meshCircleComp.fOutRadius += meshCircleComp.fSpeed;

		// アルファ値を減少させる
		meshCircleComp.inColor.a -= meshCircleComp.fDecAlphaLv;
		meshCircleComp.outColor.a -= meshCircleComp.fDecAlphaLv;

		meshCircleComp.nLife--;

		// 範囲制限
		meshCircleComp.inColor.a = math::Clamp(meshCircleComp.inColor.a, 0.0f, 1.0f);

		// 寿命が尽きた
		if (meshCircleComp.nLife <= 0)
		{
			// 破棄
			registry.destroy(entity);
		}
	}
}
