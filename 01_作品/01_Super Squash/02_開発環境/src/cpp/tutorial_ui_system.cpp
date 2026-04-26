//===================================================
//
// チュートリアルのUIの更新処理 [tutorial_ui_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "tutorial_ui_system.h"
#include "tag_component.hpp"
#include "tutorial_system_component.hpp"
#include "ui_fadeout_component.hpp"
#include "character_motion_spec.h"
#include "factory_2d.h"
#include "spec_combinators.h"
#include "factory_mesh.h"
#include "ball_constants.hpp"
#include "character_collision_spec.h"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "sphere_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "collision.h"
#include "ball_spec.h"
#include "tutorial_ui_component.hpp"
#include "velocity_component.hpp"
#include "transform_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "factory_mesh_quad.h"
#include "factory_system_entity.h"
#include "entity_name_component.hpp"
#include "sound.h"
#include "manager.h"
#include "input.h"
#include "minigame_player_component.hpp"
#include "texture_mrt_manager.h"
#include "mrt_target_component.hpp"
#include "entity_name_component.hpp"
#include "ui_wall_controller.h"

//***************************************************
// チュートリアルの定数
//***************************************************
namespace TutorialUIConst
{
	constexpr const char* MOVE_TASK_PATH		= "tutorial_move.png";			// 移動のタスクのテクスチャパス
	constexpr const char* SUCCESS_PATH			= "tutorial_success.png";		// 成功時のテクスチャパス
	constexpr const char* JETPACK_TASK_PATH		= "tutorial_jetpack.png";		// ジェットパックのタスクのテクスチャパス
	constexpr const char* SHOT_TASK_PATH		= "tutorial_shot.png";			// ショットのタスクのテクスチャパス
	constexpr const char* QUICK_TASK_PATH		= "tutorial_quick.png";			// 素早い移動のタスクのテクスチャパス
	constexpr const char* SMALL_TASK_PATH		= "tutorial_small.png";			// 速度中のタスクのテクスチャパス
	constexpr const char* MEDIUM_TASK_PATH		= "tutorial_medium.png";		// 速度大のタスクのテクスチャパス
	constexpr const char* TIMER_TEXTURE_PATH	= "number001.png";				// タイマーのテクスチャパス
	constexpr const char* SKIP_PATH				= "tutorial_dec_skip.png";		// スキップのテクスチャパス

	const D3DXVECTOR2 TIMER_POS					= { 1090.0f,360.0f };			// タイマーの位置
	const D3DXVECTOR2 TIMER_SIZE				= { 300.0f, 200.0f };			// タイマーの大きさ
	const D3DXVECTOR3 BALL_POS					= { 0.0f,150.0f,0.0f };			// ボールの位置
	const D3DXVECTOR3 SCORE_AREA_POS			= { 0.0f,120.0f,330.0f };		// スコアのエリアの位置
	const D3DXVECTOR3 BALL_MOVE					= { 5.0f,2.5f,5.0f };			// ボールの移動量
	constexpr float BALL_RESUTITUTION			= 0.98f;						// ボールの反発係数
	constexpr int TIMER_VALUE					= 120;							// ゲームの時間
	constexpr int INTERVAL						= 180;							// タスクを進ませない時間
}

//===================================================
// 更新処理
//===================================================
void TutorialUISystem::Update(entt::registry& registry)
{
	// チュートリアルのシステムの取得
	const auto tutorial_system = registry.view<TutorialSystemComponent>().front();

	// 使えなかったら
	if (!registry.valid(tutorial_system))
	{
		return;
	}

	// チュートリアルの取得
	auto& tutorialComp = registry.get<TutorialSystemComponent>(tutorial_system);

	// 最初のUIの使用時間
	tutorialComp.nFirstuiDisplayTime--;

	// 状態が無しなら
	if (tutorialComp.currentState == TutorialSystemComponent::State::None && tutorialComp.nFirstuiDisplayTime <= 0)
	{
		tutorialComp.currentState = TutorialSystemComponent::State::Move;
		UIWallController::SetTutorialTask(registry, TutorialUIConst::MOVE_TASK_PATH);
		tutorialComp.nInterval = TutorialUIConst::INTERVAL;
	}

	tutorialComp.nInterval--;

	// 成功判定を受け付けない時間
	if (tutorialComp.nInterval >= 0)
	{
		return;
	}

	// プレイヤーの取得
	auto playerID = registry.view<Tag::PlayerTag>().front();

	// 使えなかったら
	if (!registry.valid(playerID))
	{
		return;
	}

	// 状態の遷移
	switch (tutorialComp.currentState)
	{
	case TutorialSystemComponent::State::Move:
		SetMoveTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::jetPack:
		SetJetPackTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::QuickMove:
		SetQuickTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::Shot:
		SetShotTask(registry, playerID, tutorialComp);
		break;
	case TutorialSystemComponent::State::BallShot_Small:
		SetBallSpeedSmallTask(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::BallShot_Medium:
		SetBallSpeedMediumTask(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::MinigameRule:
		SetRuleUI(registry, tutorialComp);
		break;
	case TutorialSystemComponent::State::MinigameStart:
		SetGameStartTask(registry, tutorialComp);
		break;
	default:
		break;
	}

	// スキップ処理
	Skip(registry, tutorialComp);
}

//===================================================
// タスクの設定処理
//===================================================
void TutorialUISystem::SetTask(entt::registry& registry, const char* pTexturePath, TutorialSystemComponent& tutorialComp)
{
	if (!tutorialComp.bSetTask)
	{
		// チュートリアルのテクスチャの設定
		UIWallController::SetTutorialTask(registry, pTexturePath);
		tutorialComp.bSetTask = true;
	}
}

//===================================================
// ボールの設定処理
//===================================================
void TutorialUISystem::SetBall(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	if (!tutorialComp.bSetBall)
	{
		// ボールの生成
		auto ball = FactoryMesh::Create::Ball(registry, TutorialUIConst::BALL_POS, TutorialUIConst::BALL_MOVE, BallConstants::RADIUS * 2.0f, BallConstants::TEXTURE_PATH, BallConstants::SEGMENT);
		FactoryMesh::Build::Ball(registry, ball, TutorialUIConst::BALL_RESUTITUTION);
		tutorialComp.bSetBall = true;
	}
}

//===================================================
// 成功UIの生成
//===================================================
void TutorialUISystem::SetSuccess(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_SE_TASK_OK);
	}

	UIWallController::SetTutorialTask(registry, TutorialUIConst::SUCCESS_PATH);
	tutorialComp.nInterval = TutorialUIConst::INTERVAL;
	tutorialComp.bSetTask = false;
}

//===================================================
// 移動が成功したら
//===================================================
void TutorialUISystem::SetMoveTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CMove is_move; // 移動モーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::Move &&
		is_move.IsSatisfiedBy(registry, playerID))
	{
		// 成功の設定
		SetSuccess(registry, tutorialComp);

		tutorialComp.currentState = TutorialSystemComponent::State::jetPack;
	}
}

//===================================================
// ジェットパックが成功したら
//===================================================
void TutorialUISystem::SetJetPackTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CJetPack is_jetpack; // ジェットパックモーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::jetPack)
	{
		// タスクの設定
		SetTask(registry, TutorialUIConst::JETPACK_TASK_PATH, tutorialComp);

		if (is_jetpack.IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::QuickMove;
		}
	}
}

//===================================================
// ショットが成功したら
//===================================================
void TutorialUISystem::SetShotTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CCheckShot is_shot; // ショットモーションどうか

	if (tutorialComp.currentState == TutorialSystemComponent::State::Shot)
	{
		// ボールの設定処理
		SetBall(registry, tutorialComp);

		// タスクの設定
		SetTask(registry, TutorialUIConst::SHOT_TASK_PATH, tutorialComp);

		if (is_shot.IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::BallShot_Small;
		}
	}
}

//===================================================
// クイック移動が成功したら
//===================================================
void TutorialUISystem::SetQuickTask(entt::registry& registry, const entt::entity playerID, TutorialSystemComponent& tutorialComp)
{
	CharacterSpec::Motion::CJetPackMoveLeft is_quickMove_left;		// 左クイック移動
	CharacterSpec::Motion::CJetPackMoveRight is_quickMove_right;	// 右クイック移動

	if (tutorialComp.currentState == TutorialSystemComponent::State::QuickMove)
	{
		// タスクの設定
		SetTask(registry, TutorialUIConst::QUICK_TASK_PATH, tutorialComp);

		if (COrSpec(is_quickMove_left, is_quickMove_right).IsSatisfiedBy(registry, playerID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::Shot;
		}
	}
}

//===================================================
// ボールの速さが小以上に出来たら
//===================================================
void TutorialUISystem::SetBallSpeedSmallTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	BallSpec::CSpeedLevelAboveSmall is_aboveSmall; // 速度の判定

	if (tutorialComp.currentState == TutorialSystemComponent::State::BallShot_Small)
	{
		// タスクの設定
		SetTask(registry, TutorialUIConst::SMALL_TASK_PATH, tutorialComp);

		if (is_aboveSmall.IsSatisfiedBy(registry, ballID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			tutorialComp.currentState = TutorialSystemComponent::State::BallShot_Medium;
		}
	}
}

//===================================================
// ボールの速さが大以上に出来たら
//===================================================
void TutorialUISystem::SetBallSpeedMediumTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	BallSpec::CSpeedLevelAboveMedium is_aboveMedium; // 速度の判定

	if (tutorialComp.currentState == TutorialSystemComponent::State::BallShot_Medium)
	{
		// タスクの設定
		SetTask(registry, TutorialUIConst::MEDIUM_TASK_PATH, tutorialComp);

		if (is_aboveMedium.IsSatisfiedBy(registry, ballID))
		{
			// 成功の設定
			SetSuccess(registry, tutorialComp);

			auto& transformComp = registry.get<Transform3DComponent>(ballID);
			auto& velocityComp = registry.get<VelocityComponent>(ballID);

			transformComp.pos = Const::VEC3_NULL;
			velocityComp.move = Const::VEC3_NULL;

			tutorialComp.currentState = TutorialSystemComponent::State::MinigameRule;
		}
	}
}

//===================================================
// ルールのUIの表示
//===================================================
void TutorialUISystem::SetRuleUI(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// 現在の状態が一致したら
	if (tutorialComp.currentState == TutorialSystemComponent::State::MinigameRule)
	{
		// ルール説明まで来たらスキップをしたのと同じ状態にする
		tutorialComp.bSkip = true;

		if (!tutorialComp.bSetTask)
		{
			UIWallController::SetTutorialGameRule(registry);
			tutorialComp.bSetTask = true;
		}

		tutorialComp.nRuleDisplayTime--;

		if (tutorialComp.nRuleDisplayTime <= 0)
		{
			tutorialComp.currentState = TutorialSystemComponent::State::MinigameStart;

			UIWallController::SetTutotialGameStart(registry);

			tutorialComp.nInterval = TutorialUIConst::INTERVAL;
		}
	}
}

//===================================================
// ゲーム開始の設定処理
//===================================================
void TutorialUISystem::SetGameStart(entt::registry& registry, UIFadeOutComponent& fadeOutComp, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	auto& transformComp = registry.get<Transform3DComponent>(ballID);
	auto& velocityComp = registry.get<VelocityComponent>(ballID);
	velocityComp.move = Const::VEC3_NULL;
	transformComp.pos = Const::VEC3_NULL;

	tutorialComp.currentState = TutorialSystemComponent::State::MinigameStart;
	fadeOutComp.bPlay = true;
}

//===================================================
// ゲームスタートが成功したときの処理
//===================================================
void TutorialUISystem::SetGameStartTask(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// ボールのID
	const auto ballID = registry.view<Tag::BallTag>().front();

	// 使えないIDだったら
	if (!registry.valid(ballID))
	{
		return;
	}

	if (tutorialComp.currentState != TutorialSystemComponent::State::MinigameStart) return;

	// メッシュウォールの当たり判定
	auto& meshWallCollisionComp = registry.get<MeshWallCollisionComponent>(ballID);

	// 当たってないなら
	if (!meshWallCollisionComp.bCollision) return;

	// パラメータ
	Factory2D::Param param;

	param.pos = TutorialUIConst::TIMER_POS;
	param.size = TutorialUIConst::TIMER_SIZE;
	param.texturePath = TutorialUIConst::TIMER_TEXTURE_PATH;

	// タイマーの生成
	Factory2D::Create::Timer(registry, param, TutorialUIConst::TIMER_VALUE);

	param.pos = Const::CENTER_POS_2D;
	param.size = { SCREEN_WIDTH,SCREEN_HEIGHT };
	param.texturePath = "";
	param.nPriority = -1;

	// 背景の生成
	Factory2D::Create::SpriteMRT(registry, param, CTextureMRTManager::TYPE_TIMER);

	// スコアのエリアの生成
	FactorySystemEntity::CreateScoreArea(registry, TutorialUIConst::SCORE_AREA_POS);

	tutorialComp.currentState = TutorialSystemComponent::State::Max;

	// ゲーム開始のUI
	UIWallController::SetGameStart(registry);

	// ミニゲームのプレイヤーの取得
	auto minigamePlayerID = registry.view<MinigamePlayerComponent>().front();

	if (registry.valid(minigamePlayerID))
	{
		auto& minigameComp = registry.get<MinigamePlayerComponent>(minigamePlayerID);
		minigameComp.nCombo = 0;
	}

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->StopSound(CSound::LABEL_BGM_TUTORIAL);
		pSound->Play(CSound::LABEL_BGM_MINIGAME);
	}
}

//===================================================
// スキップを押したら
//===================================================
void TutorialUISystem::Skip(entt::registry& registry, TutorialSystemComponent& tutorialComp)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// スキップしたなら
	if (tutorialComp.bSkip)
	{
		return;
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_BAKC))
	{
		tutorialComp.bSkip = true;

		// ボールの設定
		SetBall(registry, tutorialComp);

		// チュートリアルのタスクの設定
		UIWallController::SetTutorialTask(registry, TutorialUIConst::SKIP_PATH);

		// ステートを変更、インターバルを設定
		tutorialComp.currentState = TutorialSystemComponent::State::MinigameRule;
		tutorialComp.nInterval = TutorialUIConst::INTERVAL;
		tutorialComp.bSetTask = false;
	}
}