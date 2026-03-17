//===================================================
//
// ビルボードのファクトリーの処理 [factory_billboard.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_billboard.h"
#include "transform_component.hpp"
#include "layer_component.hpp"
#include "size_component.hpp"
#include "tag_component.hpp"
#include "texture_id_component.hpp"
#include "manager.h"
#include "texture_manager.h"
#include "vertex_buffer_component.hpp"
#include "vertex_build.h"
#include "sphere_component.hpp"
#include "effect_component.hpp"
#include "renderer_component.hpp"
#include "color_component.hpp"
#include "velocity_component.hpp"
#include "texture_animation_component.hpp"
#include "animation_particle_component.hpp"
#include "sheet_component.hpp"

//===================================================
// ビルボードの生成処理
//===================================================
entt::entity FactoryBillboard::Create::Billboard(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR2& Size, const char* pTexturePath)
{
    // テクスチャマネージャーの取得
    CTextureManager *pTextureManager = CManager::GetInstance()->GetTextureManager();

    // テクスチャのID
    int nTextureID = TextureIDComponent::INVALID_ID;
    
    if (pTextureManager != nullptr)
    {
        // テクスチャのIDの取得
        nTextureID = pTextureManager->Register(pTexturePath);
    }

    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<Transform3DComponent>(entity,pos);
    registry.emplace<Tag::BillboardTag>(entity);
    registry.emplace<LayerComponent>(entity);
    registry.emplace<Size2DComponent>(entity,Size);
    registry.emplace<TextureIDComponent>(entity, nTextureID);
    registry.emplace<VertexBufferComponent>(entity);
    registry.emplace<ColorComponent>(entity, Const::WHITE);
    
    // フラグの設定
    int renderFlag =
        RendererComponent::TYPE_LIGHT_OFF | 
        RendererComponent::TYPE_ZTEST |
        RendererComponent::TYPE_ALPHA_TEST |
        RendererComponent::TYPE_ALPHA_BLEND;

    // レンダラーコンポーネント
    registry.emplace<RendererComponent>(entity, renderFlag);

	if (FAILED(InitBillboard::CreateBillboard(registry, entity)))
	{
		assert(false);
	}
    return entity;
}

//===================================================
// ビルボードの初期化処理
//===================================================
HRESULT InitBillboard::CreateBillboard(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
    // 頂点バッファの生成処理
    if (FAILED(VertexManager::CreateQuadVtxBuffer(registry, entity)))
    {
        return E_FAIL;
    }

	// コンポーネントの取得
	auto& vertexBufferComp = registry.get<VertexBufferComponent>(entity);
	auto& sizeComp = registry.get<Size2DComponent>(entity);
    auto& colorComp = registry.get<ColorComponent>(entity);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	vertexBufferComp.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR2 Size = sizeComp.size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-Size.x, Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Size.x, Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-Size.x, -Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Size.x, -Size.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = colorComp.col;
	pVtx[1].col = colorComp.col;
	pVtx[2].col = colorComp.col;
	pVtx[3].col = colorComp.col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / nSegmentU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / nSegmentV);
	pVtx[3].tex = D3DXVECTOR2(1.0f / nSegmentU, 1.0f / nSegmentV);

	// 頂点バッファのアンロック
	vertexBufferComp.pVtxBuffer->Unlock();

    return S_OK;
}

//===================================================
// エフェクトの生成
//===================================================
entt::entity FactoryBillboard::Create::Effect(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const char* pTexturePath)
{
    // テクスチャマネージャーの取得
    CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

    // テクスチャのID
    int nTextureID = TextureIDComponent::INVALID_ID;

    if (pTextureManager != nullptr)
    {
        // テクスチャのIDの取得
        nTextureID = pTextureManager->Register(pTexturePath);
    }

    // entityの生成
    const entt::entity entity = registry.create();
   
    registry.emplace<Transform3DComponent>(entity, pos);
    registry.emplace<ColorComponent>(entity, col);
    registry.emplace<Tag::BillboardTag>(entity);
    registry.emplace<Tag::EffectTag>(entity);

    registry.emplace<LayerComponent>(entity);
    registry.emplace<Size2DComponent>(entity, D3DXVECTOR2(fRadius, fRadius));
    registry.emplace<TextureIDComponent>(entity, nTextureID);
    registry.emplace<VertexBufferComponent>(entity);

    // フラグの設定
    int renderFlag = 
        RendererComponent::TYPE_LIGHT_OFF | 
        RendererComponent::TYPE_ZTEST | 
        RendererComponent::TYPE_ALPHA_TEST | 
        RendererComponent::TYPE_ALPHA_BLEND;

    registry.emplace<RendererComponent>(entity, renderFlag);

    return entity;
}

//===================================================
// パーティクルの生成
//===================================================
entt::entity FactoryBillboard::Create::Particle(entt::registry& registry, ParticleComponent::Param& param)
{
    // entityの生成
    const entt::entity entity = registry.create();

    registry.emplace<Tag::ParticleTag>(entity);
    registry.emplace<ParticleComponent>(entity, param);

    return entity;
}

//===================================================
// エフェクトの構築処理(entity指定)
//===================================================
void FactoryBillboard::Build::Effect(entt::registry& registry, entt::entity entity, const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const char* pTexturePath)
{
    // テクスチャマネージャーの取得
    CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

    // テクスチャのID
    int nTextureID = TextureIDComponent::INVALID_ID;

    if (pTextureManager != nullptr)
    {
        // テクスチャのIDの取得
        nTextureID = pTextureManager->Register(pTexturePath);
    }

    registry.emplace<Transform3DComponent>(entity, pos);
    registry.emplace<ColorComponent>(entity, col);
    registry.emplace<Tag::BillboardTag>(entity);
    registry.emplace<Tag::EffectTag>(entity);

    registry.emplace<LayerComponent>(entity);
    registry.emplace<Size2DComponent>(entity, D3DXVECTOR2(fRadius, fRadius));
    registry.emplace<TextureIDComponent>(entity, nTextureID);
    registry.emplace<VertexBufferComponent>(entity);

    // フラグの設定
    int renderFlag = 
        RendererComponent::TYPE_LIGHT_OFF | 
        RendererComponent::TYPE_ZTEST | 
        RendererComponent::TYPE_ALPHA_TEST | 
        RendererComponent::TYPE_ALPHA_BLEND;

    registry.emplace<RendererComponent>(entity, renderFlag);
}

//===================================================
// エフェクトの構築処理
//===================================================
void FactoryBillboard::Build::Effect(entt::registry& registry, entt::entity entity, const int nLife, const D3DXVECTOR3 move, const unsigned int flag)
{
    // 大きさの取得
    auto& sizeComp = registry.get<Size2DComponent>(entity);

    // 半径の取得
    float fRadius = sizeComp.size.x;

    // フラグのチェック
    bool bDecAlv = flag & EffectComponent::FLAG_DEC_ALPHA;
    bool bDecRadius = flag & EffectComponent::FLAG_DEC_RADIUS;

    float fDecALv = bDecAlv ? 1.0f / nLife : 0.0f;
    float fDecRadius = bDecRadius ? fRadius / nLife : 0.0f;

    registry.emplace<EffectComponent>(entity, flag, nLife, fDecRadius, fDecALv);
    registry.emplace<VelocityComponent>(entity, 0.0f, 0.0f, move);

    if (FAILED(InitBillboard::CreateBillboard(registry, entity)))
    {
        assert(false);
    }
}

//===================================================
// エフェクトの構築処理(オーバーロード)
//===================================================
void FactoryBillboard::Build::Effect(entt::registry& registry, entt::entity entity, const float fGravity)
{
    // エフェクトの取得
    auto& effectComp = registry.get<EffectComponent>(entity);

    // 重力をつけるなら
    if (effectComp.nFlag & EffectComponent::FLAG_GRAVITY)
    {
        // 移動量取得
        auto& velocityComp = registry.get<VelocityComponent>(entity);
        velocityComp.fGravity = fGravity;
    }
}

//===================================================
// アニメーションするエフェクトの構築処理
//===================================================
void FactoryBillboard::Build::AnimationEffect(entt::registry& registry, entt::entity entity, const int nSpeed, const bool bLoop, const int nSegmentU, const int nSegmentV)
{
    registry.emplace<SheetComponent>(entity, nSegmentU, nSegmentV);
    registry.emplace<TextureAnimationComponent>(entity, nSpeed, bLoop);
}

//===================================================
// アニメーションするパーティクルの構築処理
//===================================================
void FactoryBillboard::Build::AnimationParticle(entt::registry& registry, entt::entity entity, const int nSpeed, const bool bLoop, const int nSegmentU, const int nSegmentV)
{
    registry.emplace<AnimationParticleComponent>(entity, nSpeed, bLoop, nSegmentU, nSegmentV);
}