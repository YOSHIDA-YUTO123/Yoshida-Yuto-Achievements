//===================================================
//
// カメラが壁の裏側にいるか判定する処理 [check_camera_behind_wall.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "check_camera_behind_wall.h"
#include "tag_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"
#include "vertex_buffer_component.hpp"
#include "mesh_vtx_component.hpp"
#include "renderer_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace CheckWallConst
{
	constexpr float BEHIND_ALPHA = 0.1f;	// 後ろに回ったときの透明度
}

//===================================================
// コンストラクタ
//===================================================
CCheckCameraBehindWall::CCheckCameraBehindWall(entt::registry& registry) :
	m_registry(registry)
{
}

//===================================================
// 更新処理
//===================================================
void CCheckCameraBehindWall::Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot)
{	
	// 名前空間の使用
	using namespace CheckWallConst;

	// 壁のエンティティ
	auto wallView = m_registry.view<Tag::MeshWallTag>();

	// 壁の取得
	for (auto wall : wallView)
	{
		// 頂点バッファのコンポーネント
		auto& vertexBufferComp = m_registry.get<VertexBufferComponent>(wall);

		// 頂点のコンポーネント
		auto& meshVtxComp = m_registry.get<MeshVtxComponent>(wall);

		// メッシュの位置の取得
		auto& meshTransform = m_registry.get<Transform3DComponent>(wall);

		// 頂点バッファへのポインタ
		VERTEX_3D_MULT* pVtx = nullptr;

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

		// 内積で壁の裏側に行ったかどうか判定
		float fDot = math::Dot(posV - meshTransform.pos, normal);

		// レンダラーのコンポーネントの取得
		auto& rendererComp = m_registry.get<RendererComponent>(wall);

		// 裏側に行ったら
		if (fDot <= 0.0f)
		{
			// 表側カリングをオン,Zバッファをオン
			rendererComp.nFlag |= RendererComponent::TYPE_CULL_FRONT;
			rendererComp.nFlag |= RendererComponent::TYPE_ZTEST;

			// 透明度を下げる
			meshVtxComp.col.a = BEHIND_ALPHA;
		}
		else
		{
			// もとに戻す
			rendererComp.nFlag = RendererComponent::TYPE_NONE;
			meshVtxComp.col.a = 1.0f;
		}

		// 頂点数分回す
		for (int nCntVtx = 0; nCntVtx < meshVtxComp.nNumVertex; nCntVtx++)
		{
			// 色を設定
			pVtx[nCntVtx].col = meshVtxComp.col;
		}

		// 頂点バッファのアンロック
		vertexBufferComp.pIdxBuffer->Unlock();
	}
	
	// 使用していない
	(void)posR;
	(void)rot;
}