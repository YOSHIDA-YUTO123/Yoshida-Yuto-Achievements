//===================================================
//
// デバッグ情報の更新処理 [debug_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "debug_system.h"
#include "input.h"
#include "manager.h"
#include "tag_component.hpp"
#include "factory_mesh.h"
#include "velocity_component.hpp"
#include "utility_math.h"
#include "ball_component.hpp"
#include "factory_billboard.h"
#include "ball_constants.hpp"
#include "game_controller.h"
#include "hitstop_controller.h"
#include "color_constants.hpp"
#include "texture_id_component.hpp"
#include "texture_manager.h"
#include "camera_shake.h"
#include "texture_mt_component.hpp"
#include "texture_mt_manager.h"
#include "timer_component.hpp"
#include "debug_proc.h"
#include "effect_constants.hpp"
#include "effect_component.hpp"
#include "factory_2d.h"
#include "entity_name_component.hpp"
#include "transform_component.hpp"

//***************************************************
// 更新処理
//***************************************************
void DebugSystem::Update(entt::registry& registry)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();
	CInputMouse* pMouse = pManager->GetInputMouse();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pMouse == nullptr) return;

	// 球の発射処理
	if (pMouse->OnMouseTriggerDown(CInputMouse::LEFT) && pKeyboard->GetPress(DIK_LSHIFT))
	{
		D3DXVECTOR3 rayOrigin, rayDir;

		math::GetMouseRay(&rayOrigin, &rayDir);

		auto entity = FactoryMesh::Create::Ball(registry, rayOrigin,Const::VEC3_NULL, BallConstants::RADIUS, BallConstants::TEXTURE_PATH, BallConstants::SEGMENT);

		auto& velocity = registry.get<VelocityComponent>(entity);

		velocity.move = rayDir * 180.0f;
	}
}
