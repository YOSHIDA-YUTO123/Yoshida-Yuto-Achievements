//===================================================
//
// パーティクルの処理 [particle_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "particle_system.h"
#include "particle_component.hpp"
#include "tag_component.hpp"
#include "factory_billboard.h"
#include "utility_math.h"
#include "effect_component.hpp"
#include "manager.h"
#include "animation_particle_component.hpp"

//===================================================
// 更新処理
//===================================================
void ParticleSystem::Update(entt::registry& registry)
{
	// 指定したコンポーネントを持つエンティティの取得
	auto view = registry.view<Tag::ParticleTag>();

	// エンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& particleComp = registry.get<ParticleComponent>(entity);

		// パラメータ構造体のアドレスの取得
		ParticleComponent::Param* pParam = &particleComp.param;

		// 発生させる粒子の数分回す
		for (int nCnt = 0; nCnt < pParam->nNum; nCnt++)
		{
			// 角度の選出
			float fAngleX = (math::Random(pParam->fAngleMin, pParam->fAngleMax) * 0.01f);
			float fAngleY = (math::Random(pParam->fAngleMin, pParam->fAngleMax) * 0.01f);

			D3DXVECTOR3 move;

			// 移動用の選出
			move.x = math::Random(pParam->move.x * 0.5f, pParam->move.x);
			move.y = math::Random(pParam->move.y * 0.5f, pParam->move.y);
			move.z = math::Random(pParam->move.z * 0.5f, pParam->move.z);

			D3DXVECTOR3 moveWk;

			// 移動量の設定
			moveWk.x = sinf(fAngleX) * sinf(fAngleY) * move.x;
			moveWk.y = cosf(fAngleX) * move.y;
			moveWk.z = sinf(fAngleX) * cosf(fAngleY) * move.z;

			// 半径
			float fRadius = math::Random(pParam->fRadius * 0.5f, pParam->fRadius);

			// 寿命の設定
			int nLife = math::Random(pParam->nLife / 2, pParam->nLife);

			// パーティクルのアニメーションの取得
			auto pAnimation = registry.try_get<AnimationParticleComponent>(entity);

			if (pAnimation == nullptr)
			{
				// エフェクトの生成
				auto effect = FactoryBillboard::Create::Effect(registry, pParam->pos, pParam->col, fRadius, pParam->texturePath.c_str());
				FactoryBillboard::Build::Effect(registry, effect, nLife, moveWk, pParam->nEffectFlag);
			}
			else
			{
				// エフェクトの生成
				auto effect = FactoryBillboard::Create::Effect(registry, pParam->pos, pParam->col, fRadius, pParam->texturePath.c_str());
				FactoryBillboard::Build::Effect(registry, effect, nLife, moveWk, pParam->nEffectFlag);
				FactoryBillboard::Build::AnimationEffect(registry, effect, pAnimation->nSpeed, pAnimation->bLoop, pAnimation->nSegmentU, pAnimation->nSegmentV);
			}
		}

		pParam->nSpawnTime--;

		// 発生時間が終了したら
		if (pParam->nSpawnTime <= 0)
		{
			// 自分の破棄
			registry.destroy(entity);
		}
	}
}
