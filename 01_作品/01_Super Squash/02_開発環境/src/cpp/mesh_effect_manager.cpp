//===================================================
//
// メッシュの波のマネージャー [mesh_effect_manager.cpp]
// Author:YUTO YOSHIDAe
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_effect_manager.h"
#include "wall_effect_component.hpp"

//===================================================
// 波の生成処理
//===================================================
entt::entity MeshEffectManager::CreateWave(MeshWaveComponent::Param& param, MeshWaveComponent::Type type, entt::registry& registry, entt::entity mesh, const D3DXCOLOR& col)
{
    // エンティティの生成
    const entt::entity entity = registry.create();

    auto& wave = registry.emplace<MeshWaveComponent>(entity, type);

    wave.param = param;
    wave.meshID = mesh;
    wave.fStartHeight = param.fHeight;
    wave.col = col;

    return entity;
}

//===================================================
// エフェクト生成処理
//===================================================
entt::entity MeshEffectManager::CreateEffect(entt::registry& registry, entt::entity mesh, const D3DXVECTOR3& epicenter, const float fRadius, const float fRough, const int nTime)
{
    // エンティティの生成
    const entt::entity entity = registry.create();

    registry.emplace<WallEffectComponent>(entity, mesh, epicenter, fRadius, fRough, nTime);

    return entity;
}
