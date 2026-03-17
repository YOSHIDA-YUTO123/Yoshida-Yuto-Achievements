//===================================================
//
// マルチレンダーターゲットのスプライト [sprite_mrt_renderer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "sprite_mrt_renderer.h"
#include "tag_component.hpp"
#include "renderer.h"
#include "manager.h"
#include "vertex_buffer_component.hpp"
#include "texture_id_component.hpp"
#include "renderer_component.hpp"
#include "renderer_manager.h"
#include "texture_manager.h"
#include "mrt_target_component.hpp"
#include "entity_manager.h"

//===================================================
// 描画処理
//===================================================
void SpriteMRTRenderer::Renderer(entt::registry& registry, const int nType)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	// 取得できなかったら
	assert(pRenderer != nullptr && "Sprite_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// レイヤー順にソートしたenityの取得
	auto EntityList = EntityManager::GetSortLayerEntity<RendererTag::SpriteMRTTag>(registry);

	// コンポーネント分回す
	for (auto entity : EntityList)
	{
		// コンポーネントの取得
		auto& VertexBufferComp = registry.get<VertexBufferComponent>(entity);
		auto& textureIDComp = registry.get<TextureIDComponent>(entity);
		auto pRendererComp = registry.try_get<RendererComponent>(entity);
		auto pMRTTargetComp = registry.try_get<MRTTargetComponent>(entity);

		// ターゲットが違うなら
		if (pMRTTargetComp != nullptr && pMRTTargetComp->nTarget != nType)
		{
			continue;
		}

		// 描画オフなら処理を飛ばす
		if (pRendererComp != nullptr && pRendererComp->nFlag & RendererComponent::TYPE_NO_DISP)
		{
			continue;
		}

		// レンダーステートの設定
		RendererManager::SetRenderState(pDevice, pRendererComp);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VertexBufferComp.pVtxBuffer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(textureIDComp.nTextureID));
	
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類

		// レンダーステートのリセット
		RendererManager::ResetRenderState(pDevice, pRendererComp);
	}
}