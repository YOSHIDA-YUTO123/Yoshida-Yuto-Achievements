//===================================================
//
// メッシュの波の処理 [mesh_wave_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_wave_system.h"
#include "mesh_wave_component.hpp"
#include "mesh_vtx_component.hpp"
#include "transform_component.hpp"
#include "vertex_buffer_component.hpp"

//===================================================
// 更新処理
//===================================================
void MeshWaveSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<MeshWaveComponent>();

	// エンティティの数分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& meshWaveComp = registry.get<MeshWaveComponent>(entity);

		// 波の発生時間が0以下だったら処理を飛ばす
		if (meshWaveComp.param.nTime <= 0)
		{
			continue;
		}

		// 種類の遷移
		switch (meshWaveComp.type)
		{
		case MeshWaveComponent::Type::XY:
			WaveXY(registry, meshWaveComp, entity);
			break;
		case MeshWaveComponent::Type::XZ:
			WaveXZ(registry, meshWaveComp, entity);
			break;
		case MeshWaveComponent::Type::MULT_XY:
			WaveMult_XY(registry, meshWaveComp, entity);
			break;
		case MeshWaveComponent::Type::MULT_XZ:
			WaveMult_XZ(registry, meshWaveComp, entity);
			break;
		default:
			break;
		}
	}
}

//===================================================
// 波の処理(XY平面)
//===================================================
bool MeshWaveSystem::WaveXY(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave)
{
	// メッシュの頂点の取得
	auto& meshVtxComp = registry.get<MeshVtxComponent>(waveComp.meshID);
	auto& meshTransform = registry.get<Transform3DComponent>(waveComp.meshID);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(waveComp.meshID);

	// 波のカウンターを進める
	waveComp.nCounter++;

	// 相対値を求める
	float fRate = (float)waveComp.nCounter / (float)waveComp.param.nTime;

	// 波の高さをだんだん0に近づける
	waveComp.param.fHeight = waveComp.fStartHeight + (0.0f - waveComp.fStartHeight) * fRate;

	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
	{
		// 頂点座標の取得
		D3DXVECTOR3 pos = pVtx[nCnt].pos;

		D3DXVECTOR3 epicenter;
		D3DXMATRIX mtxInverse;

		// 逆行列を求める(回転を無視して計算するため)
		D3DXMatrixInverse(&mtxInverse, nullptr, &meshTransform.mtxWorld);
		D3DXVec3TransformCoord(&epicenter, &waveComp.param.epicenter, &mtxInverse);

		// 震源地から頂点までの差分
		D3DXVECTOR3 diff = epicenter - pos;

		// 距離をもとめる
		float fDistance = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

		// 時間に応じた距離を設定
		float fTimeInRadius = waveComp.param.fInRadius + waveComp.fTime;
		float fTimeOutRadius = waveComp.param.fOutRadius + waveComp.fTime;

		// 範囲内だったら
		if (fDistance >= fTimeInRadius && fDistance <= fTimeOutRadius)
		{
			D3DXCOLOR col = pVtx[nCnt].col;

			col.r = waveComp.col.r;
			col.g = waveComp.col.g;
			col.b = waveComp.col.b;

			// 色の設定
			pVtx[nCnt].col = col;

			// 高さの設定
			float dest = waveComp.param.fHeight + sinf(fDistance * waveComp.param.fCoef);

			// 目的の高さに近づける
			pos.z += (dest - pos.z) * fRate;
		}
		// 外側の半径以内だったら
		else if (fDistance <= fTimeOutRadius)
		{
			D3DXCOLOR col = pVtx[nCnt].col;

			col.r = Const::WHITE.r;
			col.g = Const::WHITE.g;
			col.b = Const::WHITE.b;

			// 色の設定
			pVtx[nCnt].col = col;

			// 目的の高さに近づける
			pos.z += (0.0f - pos.z) * fRate;
		}

		// 頂点座標の設定
		pVtx[nCnt].pos.z = pos.z;
	}

	// 速さに応じた波の幅を設定
	waveComp.fTime += waveComp.param.fSpeed;

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 最大値だったら
	if (waveComp.nCounter >= waveComp.param.nTime)
	{
		// カウンターをリセット
		waveComp.nCounter = 0;

		// 破棄する
		registry.destroy(wave);

		return false;
	}

	return true;
}

//===================================================
// 波の処理(XZ平面)
//===================================================
bool MeshWaveSystem::WaveXZ(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave)
{
	// メッシュの頂点の取得
	auto& meshVtxComp = registry.get<MeshVtxComponent>(waveComp.meshID);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(waveComp.meshID);

	// 波のカウンターを進める
	waveComp.nCounter++;

	// 相対値を求める
	float fRate = (float)waveComp.nCounter / (float)waveComp.param.nTime;

	// 速さに応じた波の幅を設定
	waveComp.fTime += waveComp.param.fSpeed;

	// 波の高さをだんだん0に近づける
	waveComp.param.fHeight = waveComp.fStartHeight + (0.0f - waveComp.fStartHeight) * fRate;

	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
	{
		// 頂点座標の取得
		D3DXVECTOR3 pos = pVtx[nCnt].pos;

		// 震源地から頂点までの差分
		D3DXVECTOR3 diff = waveComp.param.epicenter - pos;

		// 距離をもとめる
		float fDistance = sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

		// 時間に応じた距離を設定
		float fTimeInRadius = waveComp.param.fInRadius + waveComp.fTime;
		float fTimeOutRadius = waveComp.param.fOutRadius + waveComp.fTime;

		// 範囲内だったら
		if (fDistance >= fTimeInRadius && fDistance <= fTimeOutRadius)
		{
			// 高さの設定
			float dest = waveComp.param.fHeight + sinf(fDistance * waveComp.param.fCoef);

			// 目的の高さに近づける
			pos.y += (dest - pos.y) * 0.1f;
		}
		else
		{
			// 目的の高さに近づける
			pos.y += (0.0f - pos.y) * 0.05f;
		}

		// 頂点座標の設定
		pVtx[nCnt].pos = pos;
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 最大値だったら
	if (waveComp.nCounter >= waveComp.param.nTime)
	{
		// カウンターをリセット
		waveComp.nCounter = 0;

		// 破棄する
		registry.destroy(wave);

		return false;
	}

	return true;
}

//===================================================
// 波の処理(XY平面)マルチテクスチャ
//===================================================
bool MeshWaveSystem::WaveMult_XY(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave)
{
	// メッシュの頂点の取得
	auto& meshVtxComp = registry.get<MeshVtxComponent>(waveComp.meshID);
	auto& meshTransform = registry.get<Transform3DComponent>(waveComp.meshID);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(waveComp.meshID);

	// 相対値を求める
	float fRate = (float)waveComp.nCounter / (float)waveComp.param.nTime;

	// 波の高さをだんだん0に近づける
	waveComp.param.fHeight = waveComp.fStartHeight + (0.0f - waveComp.fStartHeight) * fRate;

	VERTEX_3D_MULT* pVtx = NULL;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点数分回す
	for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
	{
		int nVtxCountU = meshVtxComp.nSegmentU + 1;

		int nCntX = nCnt % nVtxCountU;
		int nCntY = nCnt / nVtxCountU;

		// エッジは処理を飛ばす
		if (nCntX == 0 || nCntX == meshVtxComp.nSegmentU || nCntY == 0 || nCntY == meshVtxComp.nSegmentV)
		{
			continue;
		}

		// 頂点座標の取得
		D3DXVECTOR3 pos = pVtx[nCnt].pos;

		D3DXVECTOR3 epicenter;
		D3DXMATRIX mtxInverse;

		// 逆行列を求める(回転を無視して計算するため)
		D3DXMatrixInverse(&mtxInverse, nullptr, &meshTransform.mtxWorld);
		D3DXVec3TransformCoord(&epicenter, &waveComp.param.epicenter, &mtxInverse);

		// 震源地から頂点までの差分
		D3DXVECTOR3 diff = epicenter - pos;

		// 距離をもとめる
		float fDistance = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

		// 時間に応じた距離を設定
		float fTimeInRadius = waveComp.param.fInRadius + waveComp.fTime;
		float fTimeOutRadius = waveComp.param.fOutRadius + waveComp.fTime;

		// 範囲内だったら
		if (fDistance >= fTimeInRadius && fDistance <= fTimeOutRadius)
		{
			D3DXCOLOR col = pVtx[nCnt].col;

			col.r = waveComp.col.r;
			col.g = waveComp.col.g;
			col.b = waveComp.col.b;

			// 色の設定
			pVtx[nCnt].col = col;

			// 高さの設定
			float dest = waveComp.param.fHeight + sinf(fDistance * waveComp.param.fCoef);

			// 目的の高さに近づける
			pos.z += (dest - pos.z) * fRate;
		}
		// 外側の半径以内だったら
		else if(fDistance <= fTimeOutRadius)
		{
			D3DXCOLOR col = pVtx[nCnt].col;

			col.r = Const::WHITE.r;
			col.g = Const::WHITE.g;
			col.b = Const::WHITE.b;

			// 色の設定
			pVtx[nCnt].col = col;

			// 目的の高さに近づける
			pos.z += (0.0f - pos.z) * fRate;
		}

		// 頂点座標の設定
		pVtx[nCnt].pos.z = pos.z;
	}

	// 速さに応じた波の幅を設定
	waveComp.fTime += waveComp.param.fSpeed;

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 最大値だったら
	if (waveComp.nCounter >= waveComp.param.nTime)
	{
		// カウンターをリセット
		waveComp.nCounter = 0;

		// 破棄する
		registry.destroy(wave);

		return false;
	}

	// 波のカウンターを進める
	waveComp.nCounter++;

	return true;
}

//===================================================
// 波の処理(XZ平面)マルチテクスチャ
//===================================================
bool MeshWaveSystem::WaveMult_XZ(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave)
{
	// メッシュの頂点の取得
	auto& meshVtxComp = registry.get<MeshVtxComponent>(waveComp.meshID);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(waveComp.meshID);

	// 波のカウンターを進める
	waveComp.nCounter++;

	// 相対値を求める
	float fRate = (float)waveComp.nCounter / (float)waveComp.param.nTime;

	// 速さに応じた波の幅を設定
	waveComp.fTime += waveComp.param.fSpeed;

	// 波の高さをだんだん0に近づける
	waveComp.param.fHeight = waveComp.fStartHeight + (0.0f - waveComp.fStartHeight) * fRate;

	VERTEX_3D_MULT* pVtx = NULL;

	// 頂点バッファをロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < meshVtxComp.nNumVertex; nCnt++)
	{
		// 頂点座標の取得
		D3DXVECTOR3 pos = pVtx[nCnt].pos;

		// 震源地から頂点までの差分
		D3DXVECTOR3 diff = waveComp.param.epicenter - pos;

		// 距離をもとめる
		float fDistance = sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));

		// 時間に応じた距離を設定
		float fTimeInRadius = waveComp.param.fInRadius + waveComp.fTime;
		float fTimeOutRadius = waveComp.param.fOutRadius + waveComp.fTime;

		// 範囲内だったら
		if (fDistance >= fTimeInRadius && fDistance <= fTimeOutRadius)
		{
			// 高さの設定
			float dest = waveComp.param.fHeight + sinf(fDistance * waveComp.param.fCoef);

			// 目的の高さに近づける
			pos.y += (dest - pos.y) * 0.1f;
		}
		else
		{
			// 目的の高さに近づける
			pos.y += (0.0f - pos.y) * 0.05f;
		}

		// 頂点座標の設定
		pVtx[nCnt].pos = pos;
	}

	// 頂点バッファをアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

	// 最大値だったら
	if (waveComp.nCounter >= waveComp.param.nTime)
	{
		// カウンターをリセット
		waveComp.nCounter = 0;

		// 破棄する
		registry.destroy(wave);

		return false;
	}

	return true;
}
