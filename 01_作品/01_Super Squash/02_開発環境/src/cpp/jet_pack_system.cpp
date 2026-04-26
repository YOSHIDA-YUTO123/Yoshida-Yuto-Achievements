//===================================================
//
// ジェットパックの更新処理の定義 [jet_pack_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "jet_pack_system.h"
#include "jet_pack_component.hpp"
#include "transform_component.hpp"
#include "velocity_component.hpp"
#include "character_component.hpp"
#include "utility_math.h"
#include "player_component.hpp"
#include "factory_billboard.h"
#include "particle_helper.h"
#include "manager.h"
#include "effect_component.hpp"
#include "character_motion_spec.h"

#ifdef _DEBUG
#include "line.h"
#endif // _DEBUG
#include "player_command_component.hpp"
#include "minigame_player_component.hpp"

//===================================================
// 更新処理
//===================================================
void JetPackSystem::Update(entt::registry& registry)
{
	// ジェットパックの取得
	auto view = registry.view<JetPackComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// 空間情報の取得
		auto& jetPackComp = registry.get<JetPackComponent>(entity);
		auto& transformComp = registry.get<Transform3DComponent>(entity);
	
		// オーナーがいないなら処理を飛ばす
		if (jetPackComp.ownerID == entt::null) continue;

		// 移動量の取得
		auto& minigamePlayerComp = registry.get<MinigamePlayerComponent>(jetPackComp.ownerID);
		
		CharacterSpec::Motion::CJetPack is_Jetpack; // ジェットパックモーションかどうか

		// プレイヤーがジェットパックのエフェクトを出せるか
		if (minigamePlayerComp.bJetPackEffect)
		{
			// パーティクルの補助クラスの取得
			CParticleHelper* pParticleHelper = CManager::GetInstance()->GetParticleHelper();

			// 取得できないなら処理しない
			if (pParticleHelper == nullptr) continue;

			// パーティクルのパラメータ
			ParticleComponent::Param param;

			// エフェクトのロケータ分回す
			for (const auto& locator : jetPackComp.effectLocator)
			{
				auto& locatorTransform = registry.get<Transform3DComponent>(locator);
				D3DXVECTOR3 worldPos = math::GetPositionFromMatrix(locatorTransform.mtxWorld);

				pParticleHelper->GetParam(param, worldPos, "JetPack", EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);
				//param.move = -ownerVelocity.move;

				FactoryBillboard::Create::Particle(registry, param);
			}
		}

		D3DXQUATERNION quatDest;

		// 回転軸、回転量からクォータニオンの作成
		D3DXQuaternionRotationAxis(&quatDest, &Const::AXIS_Z, jetPackComp.fRotValue);

		D3DXQuaternionSlerp(&transformComp.quaternion, &transformComp.quaternion, &quatDest, 0.1f);
	}
}