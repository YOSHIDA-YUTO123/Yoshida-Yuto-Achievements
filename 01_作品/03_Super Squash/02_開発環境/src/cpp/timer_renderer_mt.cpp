//===================================================
//
// タイマーのMRTの描画 [timer_renderer_mt.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "timer_renderer_mt.h"
#include "texture_manager.h"
#include "renderer_manager.h"
#include "vertex_buffer_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "timer_component.hpp"
#include "transform_component.hpp"
#include "texture_id_component.hpp"
#include "renderer_component.hpp"
#include "tag_component.hpp"
#include "mrt_target_component.hpp"

//===================================================
// 描画処理
//===================================================
void TimerRendererMT::Renderer(entt::registry& registry, const int nType)
{
	// タイマーの取得
	auto view = registry.view<Tag::TimerTag>();

	// コンポーネント分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& timerComp = registry.get<TimerComponent>(entity);
		auto pMRTTargetComp = registry.try_get<MRTTargetComponent>(entity);

		// ターゲットが違うなら処理を飛ばす
		if (pMRTTargetComp != nullptr && pMRTTargetComp->nTarget != nType)
		{
			continue;
		}

		// 分の数分回す
		for (const auto& minute : timerComp.aMinuteID)
		{
			// 分の描画
			Draw(registry,minute);
		}

		// 秒の数分回す
		for (const auto& second : timerComp.aSecondID)
		{
			// 分の描画
			Draw(registry,second);
		}
	}
}

//===================================================
// 描画の設定処理
//===================================================
void TimerRendererMT::Draw(entt::registry& registry,const entt::entity time_entity)
{
	// 使用できないentityなら
	if (!registry.valid(time_entity))
	{
		//　処理をしない
		return;
	}

	// テクスチャマネージャの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "TimerManager");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// コンポーネントの取得
	auto& transformComp = registry.get<Transform3DComponent>(time_entity);
	auto& textureIDComp = registry.get<TextureIDComponent>(time_entity);
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(time_entity);
	auto pRendererComp = registry.try_get<RendererComponent>(time_entity);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &transformComp.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, vertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(textureIDComp.nTextureID));

	// レンダーステートの設定
	RendererManager::SetRenderState(pDevice, pRendererComp);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// レンダーステートのリセット
	RendererManager::ResetRenderState(pDevice, pRendererComp);
}