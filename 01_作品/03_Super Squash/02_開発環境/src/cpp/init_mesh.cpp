//===================================================
//
// メッシュの初期化処理 [init_mesh.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "init_mesh.h"
#include "vertex_manager.h"
#include "vertex_buffer_component.hpp"
#include "size_component.hpp"
#include "mesh_vtx_component.hpp"
#include "sphere_component.hpp"
#include "mesh_cylinder_component.hpp"
#include "transform_component.hpp"
#include "utility_math.h"
#include "mesh_dome_component.hpp"
#include "mesh_circle_component.hpp"
#include "mesh_orbit_component.hpp"
#include "mesh_sphere_component.hpp"
#include "factory_model.h"
#include "init_mesh_field.h"

//***************************************************
// 定数宣言
//***************************************************
namespace FactoryMeshConst
{
	constexpr int SEGMENT_V_OFFSET = 1;	// 縦の分割数のオフセット
}

//===================================================
// メッシュシリンダーの初期化処理
//===================================================
HRESULT InitMesh::Cylinder(entt::registry& registry, entt::entity entity)
{
	// コンポーネントの取得
	auto& sphereComp = registry.get<SphereComponent>(entity);
	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& cylinderComp = registry.get<MeshCylinderComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity)))
	{
		return E_FAIL;
	}

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / meshVtx.nSegmentU;
	float fTexPosY = 1.0f / meshVtx.nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= meshVtx.nSegmentV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= meshVtx.nSegmentU; nCntX++)
		{
			// 円周の割合を求める
			float fAngle = (D3DX_PI * 2.0f) / meshVtx.nSegmentU * nCntX;

			// 位置の設定
			posWk.x = sinf(fAngle) * sphereComp.fRadius;
			posWk.y = cylinderComp.fHeight - (cylinderComp.fHeight / meshVtx.nSegmentV * nCntZ);
			posWk.z = cosf(fAngle) * sphereComp.fRadius;

			pVtx[nCntVtx].pos = posWk;

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = math::GetVector(posWk, transformComp.pos);

			//頂点カラーの設定
			pVtx[nCntVtx].col = meshVtx.col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}

//===================================================
// メッシュウォールの初期化処理
//===================================================
HRESULT InitMesh::Wall(entt::registry& registry, entt::entity entity)
{
	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& sizeComp = registry.get<Size3DComponent>(entity);
	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / meshVtx.nSegmentU;
	float fTexPosY = 1.0f / meshVtx.nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= meshVtx.nSegmentV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= meshVtx.nSegmentU; nCntX++)
		{
			// 位置の設定
			posWk.x = ((sizeComp.size.x / meshVtx.nSegmentU) * nCntX) - (sizeComp.size.x * 0.5f);
			posWk.y = sizeComp.size.y - ((sizeComp.size.y / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.y * 0.5f);
			posWk.z = sizeComp.size.z - ((sizeComp.size.z / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.z * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = math::GetVector(transformComp.pos, posWk);

			//頂点カラーの設定
			pVtx[nCntVtx].col = meshVtx.col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	// 行列の初期化
	D3DXMatrixIdentity(&mtxWorld);

	// クォータニオンから回転行列を作成
	D3DXMatrixRotationQuaternion(&mtxRot, &transformComp.quaternion);

	// 平行移動行列を作成
	D3DXMatrixTranslation(&mtxTrans, transformComp.pos.x, transformComp.pos.y, transformComp.pos.z);

	// 行列を掛け合わせる
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// 頂点左上
	D3DXVECTOR3 vtxTopLeft = pVtx[0].pos;

	// 頂点右上
	D3DXVECTOR3 vtxTopRight = pVtx[meshVtx.nSegmentU].pos;

	// 左下の頂点のインデックス
	const int nBottomLeftIndex = meshVtx.nNumVertex - meshVtx.nSegmentU - 1;

	// 左下の位置
	D3DXVECTOR3 vtxBottomLeft = pVtx[nBottomLeftIndex].pos;

	// ワールド座標に変換
	D3DXVec3TransformCoord(&vtxTopLeft, &vtxTopLeft, &mtxWorld);
	D3DXVec3TransformCoord(&vtxTopRight, &vtxTopRight, &mtxWorld);
	D3DXVec3TransformCoord(&vtxBottomLeft, &vtxBottomLeft, &mtxWorld);

	// 法線を求める
	D3DXVECTOR3 normal = math::Cross((vtxTopLeft - vtxTopRight), (vtxTopLeft - vtxBottomLeft));
	D3DXVec3Normalize(&normal, &normal);

	// 頂点数分回す
	for (int nCnt = 0; nCnt < meshVtx.nNumVertex; nCnt++)
	{
		pVtx[nCnt].nor = normal;
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}

//===================================================
// マルチテクスチャの壁の初期化処理
//===================================================
HRESULT InitMesh::WallMT(entt::registry& registry, entt::entity entity, const D3DXVECTOR2& OffPosUVLeft, const D3DXVECTOR2& OffPosUVRight)
{
	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBufferMT(vertexBufferComp, meshVtx)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& sizeComp = registry.get<Size3DComponent>(entity);
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = OffPosUVRight.x / meshVtx.nSegmentU;
	float fTexPosY = OffPosUVRight.y / meshVtx.nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D_MULT* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= meshVtx.nSegmentV; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= meshVtx.nSegmentU; nCntX++)
		{
			// 位置の設定
			posWk.x = ((sizeComp.size.x / meshVtx.nSegmentU) * nCntX) - (sizeComp.size.x * 0.5f);
			posWk.y = sizeComp.size.y - ((sizeComp.size.y / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.y * 0.5f);
			posWk.z = sizeComp.size.z - ((sizeComp.size.z / meshVtx.nSegmentV) * nCntZ) - (sizeComp.size.z * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//頂点カラーの設定
			pVtx[nCntVtx].col = meshVtx.col;

			// 割合を求める
			float fRateU = static_cast<float>(nCntX) / meshVtx.nSegmentU;
			float fRateV = static_cast<float>(nCntZ) / meshVtx.nSegmentV;

			float fPosU = OffPosUVLeft.x + (OffPosUVRight.x - OffPosUVLeft.x) * fRateU;
			float fPosV = OffPosUVLeft.y + (OffPosUVRight.y - OffPosUVLeft.y) * fRateV;

			// テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2(fPosU, fPosV);

			// テクスチャ座標の設定
			pVtx[nCntVtx].texMT = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixRotationQuaternion(&mtxRot, &transformComp.quaternion);
	D3DXMatrixTranslation(&mtxTrans, transformComp.pos.x, transformComp.pos.y, transformComp.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// 頂点左上
	D3DXVECTOR3 vtxTopLeft = pVtx[0].pos;

	// 頂点右上
	D3DXVECTOR3 vtxTopRight = pVtx[meshVtx.nSegmentU].pos;

	// 左下の頂点のインデックス
	const int nBottomLeftIndex = meshVtx.nNumVertex - meshVtx.nSegmentU - 1;

	// 左下の位置
	D3DXVECTOR3 vtxBottomLeft = pVtx[nBottomLeftIndex].pos;

	// ワールド座標に変換
	D3DXVec3TransformCoord(&vtxTopLeft, &vtxTopLeft, &mtxWorld);
	D3DXVec3TransformCoord(&vtxTopRight, &vtxTopRight, &mtxWorld);
	D3DXVec3TransformCoord(&vtxBottomLeft, &vtxBottomLeft, &mtxWorld);

	// 法線を求める
	D3DXVECTOR3 normal = math::Cross((vtxTopLeft - vtxTopRight), (vtxTopLeft - vtxBottomLeft));
	D3DXVec3Normalize(&normal, &normal);

	// 頂点数分回す
	for (int nCnt = 0; nCnt < meshVtx.nNumVertex; nCnt++)
	{
		pVtx[nCnt].nor = normal;
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}

//===================================================
// ドームの初期化処理
//===================================================
HRESULT InitMesh::Dome(entt::registry& registry, entt::entity entity)
{
	// 定数の使用
	using namespace FactoryMeshConst;

	// コンポーネントの取得
	auto& meshDomeComp = registry.get<MeshDomeComponent>(entity);

	auto& topVtx = registry.get<MeshVtxComponent>(meshDomeComp.vtxEntity[meshDomeComp.TYPE_TOP]);
	auto& sideVtx = registry.get<MeshVtxComponent>(meshDomeComp.vtxEntity[meshDomeComp.TYPE_SIDE]);

	// 頂点数の計算
	topVtx.nNumVertex = (topVtx.nSegmentU + 1);
	topVtx.nNumPolygon = topVtx.nSegmentU;
	topVtx.nNumIndex = topVtx.nNumPolygon + 2;

	// 頂点数の設定
	sideVtx.nNumVertex = (sideVtx.nSegmentU + 1) * (sideVtx.nSegmentV + 1);

	// ポリゴン数の設定
	sideVtx.nNumPolygon = ((sideVtx.nSegmentU * sideVtx.nSegmentV) * 2) + (4 * (sideVtx.nSegmentV - 1));

	// インデックス数の設定
	sideVtx.nNumIndex = sideVtx.nNumPolygon + 2;

	// 頂点の総数とインデックスの総数の計算
	int nNumVertex = topVtx.nNumVertex + sideVtx.nNumVertex;
	int nNumIndex = topVtx.nNumIndex + sideVtx.nNumIndex;
	int nSegmentV = topVtx.nSegmentV + sideVtx.nSegmentV;

	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity, nNumVertex, nNumIndex)))
	{
		return E_FAIL;
	}

	// コンポーネントの取得
	auto& sphereComp = registry.get<SphereComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 0.5f / topVtx.nSegmentU;
	float fTexPosY = 0.5f / nSegmentV;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 高さを判定
	float fHeight = meshDomeComp.bReverse ? -sphereComp.fRadius : sphereComp.fRadius;

	pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[nCntVtx].nor = math::GetVector(transformComp.pos, pVtx[nCntVtx].pos);
	pVtx[nCntVtx].col = topVtx.col;
	pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

	nCntVtx++;

	// 縦の角度の割合
	float fAngleV = (D3DX_PI / 2.0f) * ((float)SEGMENT_V_OFFSET / nSegmentV);

	// 横の分割数分回す
	for (int nCnt = nCntVtx; nCnt <= topVtx.nSegmentU; nCnt++)
	{
		float fPi = meshDomeComp.bReverse ? -(D3DX_PI * 2.0f) : (D3DX_PI * 2.0f);

		// 円周の割合を求める
		float fAngle = fPi / topVtx.nSegmentU * nCnt;

		posWk.x = sinf(fAngleV) * sinf(fAngle) * sphereComp.fRadius;
		posWk.y = cosf(fAngleV) * fHeight;
		posWk.z = sinf(fAngleV) * cosf(fAngle) * sphereComp.fRadius;

		pVtx[nCntVtx].pos = posWk;

		// 法線ベクトルの設定
		pVtx[nCntVtx].nor = math::GetVector(transformComp.pos, pVtx[nCntVtx].pos);

		pVtx[nCntVtx].col = topVtx.col;

		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

		nCntVtx++;
	}

	// 頂点のオフセット
	int nOffsetVertex = nCntVtx;

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// インデックスのオフセット
	int nOffsetIndex = -1;

	// 扇型のインデックス指定
	VertexManager::FanIndex(&vertexBufferComp.pIdxBuffer, topVtx.nSegmentU, &nOffsetIndex);

	// テクスチャの座標の割合
	fTexPosX = 1.0f / sideVtx.nSegmentU;
	fTexPosY = 1.0f / nSegmentV;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 高さを判定
	fHeight = meshDomeComp.bReverse ? -sphereComp.fRadius : sphereComp.fRadius;

	for (int nCntZ = 1; nCntZ <= nSegmentV; nCntZ++)
	{
		fAngleV = (D3DX_PI / 2.0f) * ((float)nCntZ / nSegmentV);

		for (int nCntX = 0; nCntX <= sideVtx.nSegmentU; nCntX++)
		{
			// 円周
			float fPi = meshDomeComp.bReverse ? (D3DX_PI * 2.0f) : -(D3DX_PI * 2.0f);

			// 角度の割合を求める
			float fAngle = fPi / sideVtx.nSegmentU * nCntX;

			posWk.x = sinf(fAngleV) * sinf(fAngle) * sphereComp.fRadius;
			posWk.y = cosf(fAngleV) * fHeight;
			posWk.z = sinf(fAngleV) * cosf(fAngle) * sphereComp.fRadius;

			pVtx[nCntVtx].pos = posWk;

			// 法線ベクトルの設定
			pVtx[nCntVtx].nor = math::GetVector(transformComp.pos, pVtx[nCntVtx].pos);

			// 頂点カラーの設定
			pVtx[nCntVtx].col = sideVtx.col;

			// テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// グリッドのインデックス
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, sideVtx.nSegmentU, sideVtx.nSegmentV, nOffsetIndex, nOffsetVertex);

	return S_OK;
}

//===================================================
// サークルの初期化処理
//===================================================
HRESULT InitMesh::Circle(entt::registry& registry, entt::entity entity)
{
	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity)))
	{
		return E_FAIL;
	}

	auto& meshVtx = registry.get<MeshVtxComponent>(entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& circleComp = registry.get<MeshCircleComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / meshVtx.nSegmentU;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 内側の色の設定
	D3DXCOLOR Incol = meshVtx.col;
	Incol.a *= 0.5f;

	// 横の分割数分回す
	for (int nCntU = 0; nCntU <= meshVtx.nSegmentU; nCntU++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / meshVtx.nSegmentU * nCntU;

		// 位置の設定
		posWk.x = cosf(fAngle) * circleComp.fInRadius;
		posWk.y = circleComp.fHeight;;
		posWk.z = sinf(fAngle) * circleComp.fInRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 内側の色の設定
		pVtx[nCntVtx].col = Incol;

		circleComp.inColor = Incol;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, 1.0f);

		nCntVtx++;
	}

	// 横の分割数分回す
	for (int nCntU = 0; nCntU <= meshVtx.nSegmentU; nCntU++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / meshVtx.nSegmentU * nCntU;

		// 位置の設定
		posWk.x = cosf(fAngle) * circleComp.fOutRadius;
		posWk.y = 0.0f;
		posWk.z = sinf(fAngle) * circleComp.fOutRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 内側の色の設定
		pVtx[nCntVtx].col = meshVtx.col;

		circleComp.outColor = meshVtx.col;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, 1.0f);

		nCntVtx++;
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// グリッドの設定
	VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtx.nSegmentU, meshVtx.nSegmentV);

	return S_OK;
}

//===================================================
// メッシュの軌跡の初期化処理
//===================================================
HRESULT InitMesh::Orbit(entt::registry& registry, entt::entity entity)
{
	auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

	// 頂点の総数
	meshVtxComp.nNumVertex = (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);

	// インデックスの総数
	meshVtxComp.nNumIndex = meshVtxComp.nNumVertex;

	// ポリゴンの総数
	meshVtxComp.nNumPolygon = meshVtxComp.nNumVertex - 2;

	// 頂点バッファの生成処理
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity, meshVtxComp.nNumVertex, meshVtxComp.nNumIndex)))
	{
		return E_FAIL;
	}

	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& meshOrbitComp = registry.get<MeshOrbitComponent>(entity);

	// テクスチャ座(横)
	float fPosTexH = 1.0f / meshVtxComp.nSegmentU;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	vertexBufferComp.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	auto& ownerTransformComp = registry.get<Transform3DComponent>(meshOrbitComp.ownerID);

	// ワールド座標に変換
	D3DXVec3TransformCoord(&meshOrbitComp.top, &meshOrbitComp.topLocal, &ownerTransformComp.mtxWorld);
	D3DXVec3TransformCoord(&meshOrbitComp.bottom, &meshOrbitComp.bottomLocal, &ownerTransformComp.mtxWorld);

	// 頂点数分回す
	for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
	{
		// アルファ値の設定
		float fAlv = meshVtxComp.col.a * (1.0f - static_cast<float>(nCnt) / meshVtxComp.nNumVertex);

		// 頂点の設定(いったん一か所に集める)
		pVtx[nCnt].pos = meshOrbitComp.top;

		// 法線、色、テクスチャの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCnt].col = D3DXCOLOR(meshVtxComp.col.r, meshVtxComp.col.g, meshVtxComp.col.b, fAlv);
		pVtx[nCnt].tex = D3DXVECTOR2(fPosTexH * nCnt, 1.0f);

		// インデックスバッファの設定
		pIdx[nCnt] = static_cast<WORD>(nCnt);
	}

	//インデックスバッファのアンロック
	vertexBufferComp.pIdxBuffer->Unlock();

	// 四個前から始める
	for (int nCnt = meshVtxComp.nNumVertex - 4; nCnt >= 0; nCnt -= 2)
	{
		// 前の頂点座標を入れる
		D3DXVECTOR3 OldVtx0 = pVtx[nCnt].pos;
		D3DXVECTOR3 OldVtx1 = pVtx[nCnt + 1].pos;

		// 頂点座標の設定
		pVtx[nCnt + 2].pos = OldVtx0;
		pVtx[nCnt + 3].pos = OldVtx1;
	}

	// 新しい頂点を追加（先頭に入れる）
	pVtx[0].pos = meshOrbitComp.bottom;
	pVtx[1].pos = meshOrbitComp.top;

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// 球体の初期化処理
//===================================================
HRESULT InitMesh::Sphere(entt::registry& registry, entt::entity entity)
{
	// メッシュの頂点のコンポーネント
	auto& meshVtxComp = registry.get<MeshVtxComponent>(entity);

	// 頂点バッファの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);

	// 蓋の部分の頂点の計算
	int nFanVtx = (meshVtxComp.nSegmentU + 1) + 1;
	int nFanPolygon = meshVtxComp.nSegmentU;
	int nFanIndex = nFanPolygon + 2;

	// 側面の部分の頂点
	int nSideVtx = (meshVtxComp.nSegmentU + 1) * (meshVtxComp.nSegmentV + 1);
	int nSidePolygon = ((meshVtxComp.nSegmentU * meshVtxComp.nSegmentV) * 2) + (4 * (meshVtxComp.nSegmentV - 1));
	int nSideIndex = nSidePolygon + 2;

	int nNumFanVtx = nFanVtx * 2;
	int nNumFanPolygon = nFanPolygon * 2;
	int nNumFanIndex = nFanIndex * 2;

	// 頂点の総数とインデックスの総数の計算
	int nNumVertex = nNumFanVtx + nSideVtx;
	int nNumIndex = nNumFanIndex + nSideIndex;
	int nNumPolygon = nNumFanPolygon + nSidePolygon;

	// ポリゴン数の設定
	meshVtxComp.nNumPolygon = nNumPolygon;
	meshVtxComp.nNumVertex = nNumVertex;

	// 頂点バッファの生成
	if (FAILED(VertexManager::CreateMeshVtxBuffer(registry, entity, nNumVertex, nNumIndex)))
	{
		return E_FAIL;
	}

	// 球体のコンポーネント
	auto& meshSphereComp = registry.get<MeshSphereComponent>(entity);

	// 空間情報のコンポーネント
	auto& transformComp = registry.get<Transform3DComponent>(entity);

	// 頂点のカウント
	int nCntVtx = 0;

	// テクスチャの座標の割合
	float fTexPosX = 0.5f / meshVtxComp.nSegmentU;
	float fTexPosY = 0.5f / meshVtxComp.nSegmentV;

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
	pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

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
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, fTexPosY);

		nCntVtx++;
	}

	// テクスチャの座標の割合
	fTexPosX = 1.0f / meshVtxComp.nSegmentU;
	fTexPosY = 1.0f / meshVtxComp.nSegmentV;

	// 開始インデックス
	meshSphereComp.nSideStartIndex = nCntVtx;
	
	int nOffSetIndex = 0; // オフセットのインデックス

	// インデックスの設定
	VertexManager::FanIndex(&vertexBufferComp.pIdxBuffer, meshVtxComp.nSegmentU, &nOffSetIndex);

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

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntU), (fTexPosY * nCntV));

			nCntVtx++;
		}
	}

	int nBottomStartVertex = nCntVtx;

	// インデックスの設定
	nOffSetIndex = VertexManager::GridIndex(&vertexBufferComp.pIdxBuffer, meshVtxComp.nSegmentU, meshVtxComp.nSegmentV, nOffSetIndex, meshSphereComp.nSideStartIndex);

	// 下の蓋の部分の開始頂点
	meshSphereComp.nBottomStartIndex = nOffSetIndex;

	// テクスチャの座標の割合
	fTexPosX = 0.5f / meshVtxComp.nSegmentU;
	fTexPosY = 0.5f / meshVtxComp.nSegmentV;

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
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, fTexPosY);

		nCntVtx++;
	}

	pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, transformComp.pos);
	pVtx[nCntVtx].col = meshVtxComp.col;
	pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

	nCntVtx++;

	// インデックスの設定
	VertexManager::FanIndex(&vertexBufferComp.pIdxBuffer, meshVtxComp.nSegmentU, nBottomStartVertex, nOffSetIndex);

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	return S_OK;
}

