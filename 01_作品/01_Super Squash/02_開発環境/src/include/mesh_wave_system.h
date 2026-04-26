//===================================================
//
// メッシュの波の処理 [mesh_wave_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WAVE_SYSTEM_H_
#define _MESH_WAVE_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct MeshWaveComponent; // 波

//***************************************************
// メッシュの波の処理の定義
//***************************************************
struct MeshWaveSystem : SystemBase
{
	MeshWaveSystem() = default;
	~MeshWaveSystem() = default;

	void Update(entt::registry& registry) override;
private:
	bool WaveXY(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave);
	bool WaveXZ(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave);

	bool WaveMult_XY(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave);
	bool WaveMult_XZ(entt::registry& registry, MeshWaveComponent& waveComp, entt::entity wave);
};
#endif