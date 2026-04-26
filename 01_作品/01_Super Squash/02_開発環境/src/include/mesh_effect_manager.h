//===================================================
//
// メッシュの演出のマネージャー [mesh_effect_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_EFFECT_MANAGER_H_
#define _MESH_EFFECT_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_wave_component.hpp"

//***************************************************
// メッシュの演出のマネージャー
//***************************************************
namespace MeshEffectManager
{
	entt::entity CreateWave(MeshWaveComponent::Param& param, MeshWaveComponent::Type type, entt::registry& registry, entt::entity mesh, const D3DXCOLOR& col);
	entt::entity CreateEffect(entt::registry& registry, entt::entity mesh, const D3DXVECTOR3& epicenter, const float fRadius,const float fRough, const int nTime);
}
#endif