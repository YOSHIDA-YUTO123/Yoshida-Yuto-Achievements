//===================================================
//
// ミニゲームのプレイヤーの更新処理 [minigame_player_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "minigame_player_system.h"
#include "tag_component.hpp"
#include "character_collision_spec.h"
#include "sphere_collider_component.hpp"
#include "capsule_collider_component.hpp"
#include "collision.h"
#include "motion_manager.h"
#include "motion_component.hpp"
#include "utility_math.h"
#include "transform_component.hpp"
#include "character_component.hpp"
#include "velocity_component.hpp"
#include "ball_component.hpp"
#include "ball_constants.hpp"
#include "manager.h"
#include "input.h"
#include "factory_billboard.h"
#include "color_constants.hpp"
#include "character_motion_spec.h"
#include "spec_combinators.h"
#include "player_command_component.hpp"
#include "field_collision_component.hpp"
#include "player_constants.hpp"
#include "minigame_player_component.hpp"
#include "player_spec.h"
#include "size_component.hpp"
#include "sound.h"
#include "minigame_manager.h"
#include "score_manager.h"

#ifdef _DEBUG
#include "debug_proc.h"
#endif // _DEBUG
#include "outline_shader_component.hpp"
#include "parent_component.hpp"
#include "minigame_state.h"

//***************************************************
// 定数宣言
//***************************************************
namespace MinigamePlayerConst
{
	constexpr int COMBO_LEVEL = 0;		// コンボのレベル0
	constexpr int COMBO_LEVEL_1 = 3;	// コンボのレベル1
	constexpr int COMBO_LEVEL_2 = 6;	// コンボのレベル2
	constexpr int COMBO_LEVEL_3 = 9;	// コンボのレベル3
	constexpr int COMBO_LEVEL_4 = 12;	// コンボのレベル4

	constexpr float OUTLINE_SIZE_LEVEL_1 = 0.3f;	// アウトラインの大きさレベル1
	constexpr float OUTLINE_SIZE_LEVEL_2 = 0.6f;	// アウトラインの大きさレベル2
	constexpr float OUTLINE_SIZE_LEVEL_3 = 0.9f;	// アウトラインの大きさレベル3
	constexpr float OUTLINE_SIZE_LEVEL_4 = 1.3f;	// アウトラインの大きさレベル4
}

//===================================================
// 更新処理
//===================================================
void MinigamePlayerSystem::Update(entt::registry& registry)
{
	// プレイヤーの取得
	auto view = registry.view<Tag::PlayerTag>();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr) return;

	// プレイヤー分回す
	for (auto player : view)
	{
		// カプセルコライダーの取得
		auto& capsuleColliderComp = registry.get<CapsuleColliderComponent>(player);
		auto& motionComp = registry.get<MotionComponent>(player);
		auto& playerTransfromComp = registry.get<Transform3DComponent>(player);
		auto& playerVelocityComp = registry.get<VelocityComponent>(player);
		auto& minigameComp = registry.get<MinigamePlayerComponent>(player);

		// インターバルを減らす
		minigameComp.nQuickMoveInterval--;
		minigameComp.nQuickMoveTime--;

		// コンボのリセットまでの時間を減らす
		minigameComp.nComboResetTime--;

		if (minigameComp.nComboResetTime <= 0)
		{
			minigameComp.nCombo = 0;
		}

		// コンボ数に応じたエフェクトの設定処理
		SetComboEffect(registry, player);

		// ジェットパックの設定処理
		SetJetPack(registry, player, playerVelocityComp, motionComp);

		// ボールに当たったら吹き飛ぶ
		CollisionBall(registry, capsuleColliderComp, playerTransfromComp);
	}
}

//===================================================
// ジェットパックのモーションの設定処理
//===================================================
void MinigamePlayerSystem::SetJetPack(entt::registry& registry, const entt::entity player, VelocityComponent& velocityComp, MotionComponent& motionComp)
{
	CharacterSpec::Motion::CJetPack					is_Jetpack;				// ジェットパックモーションかどうか
	CharacterSpec::Motion::CCharge					is_charge;				// チャージモーションかどうか
	CharacterSpec::Motion::CJetPackMoveLeft			is_jetpack_MoveLeft;	// ジェットパックの素早い左移動か判定
	CharacterSpec::Motion::CJetPackMoveRight		is_jetpack_MoveRight;	// ジェットパックの素早い右移動か判定
	CharacterSpec::Motion::CCheckJetLeftMoveFrame	is_jet_MoveLeft_Frame;	// ジェットパックの素早い左移動のイベントフレームか判定

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	auto& characterComp = registry.get<CharacterComponent>(player);
	//auto& commandComp = registry.get<PlayerCommandComponent>(player);
	auto& fieldCollisionComp = registry.get<FieldCollisionComponent>(player);
	auto& minigameComp = registry.get<MinigamePlayerComponent>(player);

	// プレイヤーの定数の使用
	using namespace PlayerConstants;

	// インターバルがないなら
	PlayerSec::CQuickMoveInterval	is_quickMove_Interval;	// インターバルがあるかどうか
	PlayerSec::CCheckQuickMoveTime	is_quickMove_Time;		// 素早い移動時間中かどうか

	// 素早い左移動のイベントフレーム内なら
	if (is_quickMove_Time.IsSatisfiedBy(registry,player))
	{
		// 上方向に加算
		velocityComp.move.x += minigameComp.fQuickMoveValue;
		velocityComp.move.y = 0.0f;
		return;
	}

	// 着地しているかどうか
	bool bJetPack =! fieldCollisionComp.bLanding;

	// パットの素早い移動が出せるキー
	const bool bPadQuickMoveLeft = bJetPack && pJoyPad->GetPress(pJoyPad->JOYKEY_LEFT_SHOULDER);
	const bool bPadQuickMoveRight = bJetPack && pJoyPad->GetPress(pJoyPad->JOYKEY_RIGHT_SHOULDER);

	// キーボードの素早い移動が出せるキー
	const bool bKeyQuickMoveLeft = bJetPack && pKeyboard->GetPress(DIK_Q);
	const bool bKeyQuickMoveRight = bJetPack && pKeyboard->GetPress(DIK_E);

	const bool bLeftQuickMove = bPadQuickMoveLeft || bKeyQuickMoveLeft;		 // 左の素早い移動を出せる
	const bool bRiightQuickMove = bPadQuickMoveRight || bKeyQuickMoveRight;	 // 右の素早い移動を出せる

	// インターバルが無いなら
	if ((bLeftQuickMove || bRiightQuickMove) &&
		CNotSpec(is_quickMove_Interval).IsSatisfiedBy(registry,player) && !fieldCollisionComp.bLanding)
	{
		minigameComp.bJetPackEffect = true;

		// 適応時間のっ設定
		minigameComp.nQuickMoveTime = JET_PACK_QUICKMOVE_TIME;
		minigameComp.nQuickMoveInterval = JET_PACK_QUICK_INTERVAL;
		minigameComp.fQuickMoveValue = bLeftQuickMove ? JET_PACK_MOVE_LEFT : JET_PACK_MOVE_RIGHT;

		// チャージモーションじゃないなら
		if (CNotSpec(is_charge).IsSatisfiedBy(registry, player))
		{
			if (bLeftQuickMove)
			{				
				MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_JETPACK_MOVE_LEFT, 10, true);
			}
			else
			{
				MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_JETPACK_MOVE_RIGHT, 10, true);
			}
		}
	}

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 上昇を押していないなら
	if (!pKeyboard->GetPress(DIK_SPACE) && !pJoyPad->GetPress(pJoyPad->JOYKEY_A))
	{
		// ジェットパックのモーションだったら
		if (is_Jetpack.IsSatisfiedBy(registry, player) && fieldCollisionComp.bLanding)
		{
			// ジェットパックのモーション
			MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_LANDING, 10, true);
		}

		// インターバルがないなら
		if (CNotSpec(is_quickMove_Time).IsSatisfiedBy(registry, player))
		{
			// ジェットパック発動できる
			minigameComp.bJetPackEffect = false;
		}

		// 処理を飛ばす
		return;
	}

	// ジェットパックが使用されていないなら
	if (!minigameComp.bJetPackEffect)
	{
		// ジェットパック発動できる
		minigameComp.bJetPackEffect = true;

		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_JET_PACK);
		}
	}

	// 移動状態にする
	characterComp.state = CharacterComponent::State::Jump;
	
	// 右,左移動のモーションだったら
	if (COrSpec(is_jetpack_MoveLeft, is_jetpack_MoveRight).IsSatisfiedBy(registry, player))
	{
		return;
	}

	// チャージモーションじゃない
	if (CNotSpec(is_charge).IsSatisfiedBy(registry, player))
	{
		// ジェットパックのモーション
		MotionManager::Helper::Play(motionComp, MotionComponent::MOTIONTYPE_JET_PACK, 10, true);
	}

	if (pKeyboard->GetPress(DIK_W))
	{
		// 上方向に加算
		velocityComp.move.y = JET_PACK_MOVE_UP;
	}
	else if (pKeyboard->GetPress(DIK_X))
	{
		// 上方向に加算
		velocityComp.move.y = JET_PACK_MOVE_DOWN;
	}
	else
	{
		// 上方向に加算
		velocityComp.move.y = JET_PACK_MOVE_UP;
	}
}

//===================================================
// ボールとの当たり判定
//===================================================
bool MinigamePlayerSystem::CollisionBall(entt::registry& registry, CapsuleColliderComponent& capsuleComp, Transform3DComponent& trasnformComp)
{
	// ボールの取得
	auto ballView = registry.view<ColliderTag::BallSphere>();

	// ボール分回す
	for (auto ball : ballView)
	{
		// ボールのコライダーの取得
		auto& ballSphereColliderComp = registry.get<SphereColliderComponent>(ball);
		auto& ballTransfromComp = registry.get<Transform3DComponent>(ballSphereColliderComp.ownerID);
		auto& ballVelocityComp = registry.get<VelocityComponent>(ballSphereColliderComp.ownerID);
		auto& ballComp = registry.get<BallComponent>(ballSphereColliderComp.ownerID);

		// 最近接点
		D3DXVECTOR3 closePos = Const::VEC3_NULL;

		// 当たっていないなら処理を飛ばす
		if (!Collision::Capsule::CollisionSphere(registry, capsuleComp, ballSphereColliderComp, &closePos)) continue;

		// 敵の方向を求める
		D3DXVECTOR3 dir = closePos - ballTransfromComp.pos;

		// 距離を求める
		float fDistance = D3DXVec3Length(&dir);

		// 方向ベクトルを正規化
		D3DXVec3Normalize(&dir, &dir);

		// 二つの半径を足す
		float fRadius = capsuleComp.fRadius + ballSphereColliderComp.fRadius;

		// どのくらい埋まったか
		float fDepth = fRadius - fDistance;

		// 押し戻す
		trasnformComp.pos += dir * fDepth;

		if (ballVelocityComp.fMoveLength >= BallConstants::MOVE_THRESHOLD_LEVEL_SMALL)
		{
			// 目的の向きを求める
			math::GetTargetAngle(ballTransfromComp.pos, trasnformComp.pos);

			// スピードを初期化
			ballComp.fSpeed = BallConstants::INIT_SPEED;

			dir = math::GetVector(trasnformComp.pos, ballTransfromComp.pos);

			// バウンドさせる
			ballVelocityComp.move = math::Bound(ballVelocityComp.move, dir, 0.1f);

			return true;
		}
	}

	return false;
}

//===================================================
// プレイヤーのコンボ数に応じたエフェクトの設定
//===================================================
void MinigamePlayerSystem::SetComboEffect(entt::registry& registry, const entt::entity player)
{
	// 名前空間の使用
	using namespace MinigamePlayerConst;

	// コンポーネントの取得
	auto& childrenComp = registry.get<ChildrenComponent>(player);
	auto& minigameComp = registry.get<MinigamePlayerComponent>(player);


	// モデルの数分回す
	for (const auto& children : childrenComp.aChildrenID)
	{
		// アウトラインシェーダーの取得
		auto& outLineShaderComp = registry.get<OutLineShaderComponent>(children);
		
		// コンボ数が0だったら
		if (minigameComp.nCombo <= COMBO_LEVEL)
		{
			outLineShaderComp.fSize = OUTLINE_SIZE_LEVEL_1;
			outLineShaderComp.col = Color::AQUA;
		}
		else if (math::IsInRange(minigameComp.nCombo, COMBO_LEVEL_1, COMBO_LEVEL_2))
		{
			outLineShaderComp.fSize = OUTLINE_SIZE_LEVEL_2;
			outLineShaderComp.col = Color::MAGENTA;
		}
		else if (math::IsInRange(minigameComp.nCombo, COMBO_LEVEL_2, COMBO_LEVEL_3))
		{
			outLineShaderComp.fSize = OUTLINE_SIZE_LEVEL_3;
			outLineShaderComp.col = Color::GOLD;
		}
		else if (math::IsInRange(minigameComp.nCombo, COMBO_LEVEL_3, COMBO_LEVEL_4))
		{
			outLineShaderComp.fSize = OUTLINE_SIZE_LEVEL_4;
			outLineShaderComp.col = Color::CRIMSON;
		}
	}
}