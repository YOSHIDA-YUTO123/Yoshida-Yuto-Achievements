//===================================================
//
// デバッグ表示用コライダーの描画処理 [collider_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "collider_renderer.h"
#include "box_collider_component.hpp"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "transform_component.hpp"
#include "color_constants.hpp"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ColliderConst
{
	constexpr int SPHERE_SEGMENT	= 24;		// 球のコライダーの分割数
	constexpr int BOX_VERTEX		= 24;		// 矩形のコライダーの頂点数
	constexpr int CAPSULE_SEGMENT	= 16;		// カプセルのコライダーの分割数
}

//===================================================
// 描画処理
//===================================================
void ColliderRenderer::Renderer(entt::registry& registry)
{
	// 表示するかどうか
	static bool bShow = true;

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		bShow = bShow ? false : true;
	}

	if (!bShow)
	{
		return;
	}
	// 矩形コライダーの取得
	auto aabbView = registry.view<BoxColliderComponent>();

	// 取得した分回す
	for (auto entity : aabbView)
	{
		auto& aabbComp = registry.get<BoxColliderComponent>(entity);

		SetBoxVertex(registry, aabbComp);

	}

	// カプセルコライダーの取得
	auto capsuleView = registry.view<CapsuleColliderComponent>();

	// 取得した分回す
	for (auto entity : capsuleView)
	{
		auto& capsuleComp = registry.get<CapsuleColliderComponent>(entity);

		SetCapsuleVertex(registry, capsuleComp);
	}

	// 球コライダーの取得
	auto sphereView = registry.view<SphereColliderComponent>();

	// 取得した分回す
	for (auto entity : sphereView)
	{
		auto& sphereComp = registry.get<SphereColliderComponent>(entity);

		SetSphereVertex(registry, sphereComp, entity);
	}
}

//===================================================
// 描画処理
//===================================================
void ColliderRenderer::Draw(std::vector<LINE_LIST>& lineList)
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライティングOFF
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			// ZバッファをOFF

	// SetTransformのマトリックスのリセット用変数
	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);
	pDevice->SetTransform(D3DTS_WORLD, &Identity);

	// 頂点フォーマットの設定
	pDevice->SetFVF(D3DFVF_VERTEX);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineList.size() / 2, lineList.data(), sizeof(LINE_LIST));

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライティングON
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// ZバッファをON
}

//===================================================
// ボックスの頂点座標の設定
//===================================================
void ColliderRenderer::SetBoxVertex(entt::registry& registry, BoxColliderComponent& aabb)
{
	std::vector<LINE_LIST> line_vertex;

	// 配列の確保
	line_vertex.resize(24);

	// 空間情報の取得
	auto& transform = registry.get<Transform3DComponent>(aabb.ownerID);

	D3DXVECTOR3 pos = transform.pos;
	D3DXVECTOR3 size = aabb.size * 0.5f;
	D3DXCOLOR col = Color::GREENYELLOW;

	line_vertex[0] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };
	line_vertex[1] = { pos + D3DXVECTOR3( size.x,-size.y,-size.z),col };
	line_vertex[2] = { pos + D3DXVECTOR3( size.x,-size.y,-size.z),col };
	line_vertex[3] = { pos + D3DXVECTOR3( size.x, size.y,-size.z),col };
	line_vertex[4] = { pos + D3DXVECTOR3( size.x, size.y,-size.z),col };
	line_vertex[5] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[6] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[7] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };

	line_vertex[8] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };
	line_vertex[9] = { pos + D3DXVECTOR3( size.x,-size.y,size.z),col };
	line_vertex[10] = { pos + D3DXVECTOR3( size.x,-size.y,size.z),col };
	line_vertex[11] = { pos + D3DXVECTOR3( size.x, size.y,size.z),col };
	line_vertex[12] = { pos + D3DXVECTOR3( size.x, size.y,size.z),col };
	line_vertex[13] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };
	line_vertex[14] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };
	line_vertex[15] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };

	line_vertex[16] = { pos + D3DXVECTOR3(-size.x,-size.y,-size.z),col };
	line_vertex[17] = { pos + D3DXVECTOR3(-size.x,-size.y,size.z),col };

	line_vertex[18] = { pos + D3DXVECTOR3(size.x,-size.y,-size.z),col };
	line_vertex[19] = { pos + D3DXVECTOR3(size.x, -size.y,size.z),col };

	line_vertex[20] = { pos + D3DXVECTOR3(size.x, size.y,-size.z),col };
	line_vertex[21] = { pos + D3DXVECTOR3(size.x, size.y,size.z),col };

	line_vertex[22] = { pos + D3DXVECTOR3(-size.x, size.y,-size.z),col };
	line_vertex[23] = { pos + D3DXVECTOR3(-size.x, size.y,size.z),col };

	// 描画処理
	Draw(line_vertex);
}

//===================================================
// カプセルの頂点座標の設定
//===================================================
void ColliderRenderer::SetCapsuleVertex(entt::registry& registry, CapsuleColliderComponent& capsuleComp)
{
	std::vector<LINE_LIST> lineVertex;

	// 空間情報の取得
	auto& transformComp = registry.get<Transform3DComponent>(capsuleComp.ownerID);

	D3DXCOLOR col = Color::RED;

	// 高さを求める
	float fHeight = capsuleComp.endPosLocal.y - capsuleComp.startPosLocal.y;

	// 上円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::CAPSULE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::CAPSULE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::CAPSULE_SEGMENT;

		D3DXVECTOR3 startPos(
			cosf(fCurrentAngle) * capsuleComp.fRadius,
			fHeight,
			sinf(fCurrentAngle) * capsuleComp.fRadius);

		D3DXVECTOR3 endPos(
			cosf(fNextAngle) * capsuleComp.fRadius,
			fHeight,
			sinf(fNextAngle) * capsuleComp.fRadius);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &transformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// 下円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::CAPSULE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::CAPSULE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::CAPSULE_SEGMENT;

		D3DXVECTOR3 startPos(
			cosf(fCurrentAngle) * capsuleComp.fRadius,
			0.0f,
			sinf(fCurrentAngle) * capsuleComp.fRadius);

		D3DXVECTOR3 endPos(
			cosf(fNextAngle) * capsuleComp.fRadius,
			0.0f,
			sinf(fNextAngle) * capsuleComp.fRadius);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &transformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// yzの縦の円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::CAPSULE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::CAPSULE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::CAPSULE_SEGMENT;

		D3DXVECTOR3 startPos(
			0.0f,
			fHeight * 0.5f + sinf(fCurrentAngle) * fHeight,
			cosf(fCurrentAngle) * capsuleComp.fRadius
		);
		D3DXVECTOR3 endPos(
			0.0f,
			fHeight * 0.5f + sinf(fNextAngle) * fHeight,
			cosf(fNextAngle) * capsuleComp.fRadius
		);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &transformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// xyの縦の円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::CAPSULE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::CAPSULE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::CAPSULE_SEGMENT;

		D3DXVECTOR3 startPos(
			cosf(fCurrentAngle) * capsuleComp.fRadius,
			fHeight * 0.5f + sinf(fCurrentAngle) * fHeight,
			0.0f
		);
		D3DXVECTOR3 endPos(
			cosf(fNextAngle) * capsuleComp.fRadius,
			fHeight * 0.5f + sinf(fNextAngle) * fHeight,
			0.0f
		);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &transformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &transformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// 描画処理
	Draw(lineVertex);
}

//===================================================
// 球のコライダーの頂点の設定
//===================================================
void ColliderRenderer::SetSphereVertex(entt::registry& registry, SphereColliderComponent& sphereComp, const entt::entity sphereID)
{
	std::vector<LINE_LIST> lineVertex;

	// 空間情報の取得
	auto& ownerTransformComp = registry.get<Transform3DComponent>(sphereComp.ownerID);
	auto& sphereTransformComp = registry.get<Transform3DComponent>(sphereID);

	D3DXCOLOR col = Color::AQUA;

	// 円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::SPHERE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::SPHERE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::SPHERE_SEGMENT;

		D3DXVECTOR3 startPos(
			cosf(fCurrentAngle) * sphereComp.fRadius,
			sphereTransformComp.pos.y,
			sinf(fCurrentAngle) * sphereComp.fRadius);

		D3DXVECTOR3 endPos(
			cosf(fNextAngle) * sphereComp.fRadius,
			sphereTransformComp.pos.y,
			sinf(fNextAngle) * sphereComp.fRadius);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &ownerTransformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &ownerTransformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// yzの縦の円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::SPHERE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::SPHERE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::SPHERE_SEGMENT;

		D3DXVECTOR3 startPos(
			0.0f,
			sphereTransformComp.pos.y + sinf(fCurrentAngle) * sphereComp.fRadius,
			cosf(fCurrentAngle) * sphereComp.fRadius
		);

		D3DXVECTOR3 endPos(
			0.0f,
			sphereTransformComp.pos.y + sinf(fNextAngle) * sphereComp.fRadius,
			cosf(fNextAngle) * sphereComp.fRadius
		);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &ownerTransformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &ownerTransformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// xyの縦の円の生成
	for (int nCntVtx = 0; nCntVtx < ColliderConst::SPHERE_SEGMENT; nCntVtx++)
	{
		// 次のカウント
		int nNextCnt = nCntVtx + 1;

		// 現在の角度・次の角度の割合を求める
		float fCurrentAngle = Const::TWO_PI * nCntVtx / ColliderConst::SPHERE_SEGMENT;
		float fNextAngle = Const::TWO_PI * nNextCnt / ColliderConst::SPHERE_SEGMENT;

		D3DXVECTOR3 startPos(
			cosf(fCurrentAngle) * sphereComp.fRadius,
			sphereTransformComp.pos.y + sinf(fCurrentAngle) * sphereComp.fRadius,
			0.0f
		);

		D3DXVECTOR3 endPos(
			cosf(fNextAngle) * sphereComp.fRadius,
			sphereTransformComp.pos.y + sinf(fNextAngle) * sphereComp.fRadius,
			0.0f
		);

		// ワールド変換
		D3DXVec3TransformCoord(&startPos, &startPos, &ownerTransformComp.mtxWorld);
		D3DXVec3TransformCoord(&endPos, &endPos, &ownerTransformComp.mtxWorld);

		lineVertex.push_back({ startPos, col });
		lineVertex.push_back({ endPos, col });
	}

	// 描画処理
	Draw(lineVertex);
}