//================================================
//
// シーンのゲームの状態の処理 [minigame_state.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "minigame_state.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "world_system_manager.h"
#include "stencil_polygon.h"
#include "hitstop_controller.h"
#include "game_controller.h"
#include "light.h"
#include "scene_loader.h"
#include "camera.h"
#include "check_camera_behind_wall.h"
#include "camera_shake.h"
#include "minigame_manager.h"
#include "tag_component.hpp"
#include "pause_menu.hpp"
#include "sound.h"
#include "scene.h"
#include "wall_constants.hpp"
#include "follow_camera.h"
#include "scene_change_constants.hpp"

// 更新処理
#include "system_manager.h"
#include "player_system.h"
#include "jet_pack_system.h"
#include "player_motion_system.h"
#include "minigame_player_system.h"
#include "update_ball_system.h"
#include "character_system.h"
#include "update_position_3d_system.h"
#include "shadow_system.h"
#include "collider_system.h"
#include "field_collision_system.h"
#include "ball_to_field_collision_system.h"
#include "mesh_wall_collision_system.h"
#include "mesh_wave_system.h"
#include "mesh_sphere_system.h"
#include "effect_system.h"
#include "score_area_system.h"
#include "motion_system.h"
#include "particle_system.h"
#include "update_billboard_system.h"
#include "mesh_circle_system.h"
#include "mesh_orbit_system.h"
#include "mesh_quad_system.h"
#include "update_timer_system.h"
#include "update_sprite_sheet_system.h"
#include "texture_animation_system.h"
#include "calcu_world_matrix.h"
#include "menu_select_system.h"
#include "ui_scaling_system.h"
#include "update_sprite_system.h"
#include "pause_menu_system.h"
#include "collision_effect_wall_system.h"
#include "wall_effect_system.h"
#include "ui_flash_system.h"
#include "texture_mt_manager.h"
#include "ui_wall_controller.h"
#include "ui_fadeout_system.h"
#include "tutorial_ui_system.h"
#include "ui_wall_system.h"
#include "minigame_result_state.h"
#include "effect_wall_loader.h"

// レンダラー
#include "sprite_renderer.h"
#include "HitStop.h"
#include "character_renderer.h"
#include "model_renderer.h"
#include "mesh_renderer_mt.h"
#include "mesh_renderer.h"
#include "mesh_dome_renderer.h"
#include "mesh_sphere_renderer.h"
#include "mesh_quad_renderer.h"
#include "billboard_renderer.h"
#include "timer_renderer_mt.h"
#include "field_effect_renderer.h"
#include "stencil_shadow_renderer.h"
#include "outline_character_renderer.h"
#include "outline_model_renderer.h"
#include "outline_mesh_sphere_renderer.h"
#include "tutorial_ui_renderer_system.h"
#include "shader_mesh_wall_renderer.h"
#include "collider_renderer.h"
#include "sprite_mrt_renderer.h"
#include "mesh_sphere_renderer_mrt.h"
#include "mesh_wall_renderer.h"

// ファクトリー
#include "factory_mesh_quad.h"
#include "factory_system_entity.h"
#include "factory_mesh.h"
#include "factory_2d.h"
#include "factory_character.h"

#include "color_constants.hpp"
#include "ball_constants.hpp"
#include "score_component.hpp"
#include "minigame_manager.h"
#include "entity_name_component.hpp"
#include "mesh_wall_component.hpp"
#include "motion_component.hpp"
#include "mrt_target_component.hpp"

#ifdef _DEBUG
#include "debug_system.h"
#include "title_state.h"
#include "scene_controller.h"
#include "scene.h"
#include "camera_animation_system.h"
#include "transform_component.hpp"

#endif // _DEBUG

//************************************************
// 定数宣言
//************************************************
namespace MinigameState_Const
{
	// ポーズのメニューのテクスチャ
	const char* PAUSE_TEXTURE[PauseMenu::TYPE_MAX]
	{
		"data/TEXTURE/pause_continue.png",
		"data/TEXTURE/pause_retry.png",
		"data/TEXTURE/pause_quit.png",
	};

	const D3DXVECTOR2 PAUSE_MENU_POS	= { 640.0f,360.0f };	// ポーズのコンティニューのメニュー
	const D3DXVECTOR2 PAUSE_MENU_SIZE	= { 150.0f,50.0f };		// ポーズのメニューの大きさ
	constexpr float PAUSE_MENU_OFFSET	= { 100.0f };			// ポーズのメニューのオフセット
}

//************************************************
// 静的メンバ変数宣言
//************************************************
std::unique_ptr<CMiniGameManager> CMinigameState::m_pManager = nullptr; // ミニゲームの補助クラス

//================================================
// コンストラクタ
//================================================
CMinigameState::CMinigameState() : CSceneState(STATE_MINIGAME)
{
}

//================================================
// デストラクタ
//================================================
CMinigameState::~CMinigameState()
{
}

//================================================
// 初期化処理
//================================================
HRESULT CMinigameState::Init(void)
{
	// オーナーの取得
	CScene* pScene = CSceneState::GetOwner();

	// レジストリーの取得
	auto& registry = pScene->GetRegistry();

	// 名前空間の使用
	using namespace MinigameState_Const;

	// ゲームのコントローラーの生成
	CGameController::Create();

	// エフェクトの壁の読み込み処理
	CEffectWallLoader::Load(registry);

	// 基盤のシーンのロード
	Loader::Scene::BaseScene(registry);

	// ミニゲームの読み込み処理
	Loader::Scene::Minigame(registry);

	// UV座標の設定
	UIWallController::SetTutorialWall(registry);

	// チュートリアルの処理の生成
	FactorySystemEntity::CreateTutorial(registry);

	// ステンシルのポリゴンの生成
	Factory2D::Create::StencilPolygon(registry);

	// カメラのシステムの追加
	SetCameraSystem(registry);

	// ポーズメニューの生成
	CreatePauseMenu(registry);

	// ミニゲームのマネージャーの生成処理
	m_pManager = CMiniGameManager::Create(registry);

	// スコアの生成
	registry.emplace<ScoreComponent>(registry.create());

	// 更新処理の追加
	CreateSystem(pScene);

	// 描画処理の追加
	CreateRendererSysytem(pScene);

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_BGM_TUTORIAL, 0.5f);
	}

	return S_OK;
}

//================================================
// 更新処理
//================================================
void CMinigameState::Update(void)
{
	if (m_pManager != nullptr)
	{
		// 更新処理
		m_pManager->Update();
	}

#ifdef _DEBUG

	// オーナーの取得
	CScene* pScene = CSceneState::GetOwner();

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F5))
	{
		// シーンの操作クラスの取得
		CSceneController* pSceneController = pScene->GetController();

		pSceneController->ChangeScene(std::make_shared<CMinigameResultState>(), CCameraAnimationSystem::TYPE_GAME_TO_RESULT, SceneChangeConst::GAME_TO_RESULT);
	}

#endif // _DEBUG

}

//================================================
// 終了処理
//================================================
void CMinigameState::Uninit(void)
{
	// ゲームの操作クラスの取得
	auto pController = CGameController::GetInstance();

	if (pController != nullptr)
	{
		// 自分の破棄
		pController->Uninit();
	}

	// マネージャーの破棄
	m_pManager.reset();
}

//===============================================
// ポーズメニューの生成
//===============================================
void CMinigameState::CreatePauseMenu(entt::registry& registry)
{
	// 名前空間の使用
	using namespace MinigameState_Const;

	// タイトルのメニューの生成
	auto menuSelectID = FactorySystemEntity::CreateMenuSelect(registry, PauseMenu::TYPE_QUIT, DIK_DOWN, CInputJoypad::JOYKEY_DOWN, true);
	FactorySystemEntity::BuildMenuSelect(registry, menuSelectID, DIK_UP, CInputJoypad::JOYKEY_UP);

	// 2Dのパラメータ
	Factory2D::Param param;

	// ポーズのメニュー分回す
	for (int nCnt = 0; nCnt < PauseMenu::TYPE_MAX; nCnt++)
	{
		// パラメータの設定
		param.pos = { PAUSE_MENU_POS.x, PAUSE_MENU_POS.y + nCnt * PAUSE_MENU_OFFSET };
		param.size = PAUSE_MENU_SIZE;
		param.texturePath = PAUSE_TEXTURE[nCnt];

		// メニューの生成
		auto menuID = Factory2D::Create::Menu(registry, param, menuSelectID, nCnt);
		Factory2D::Build::PauseMenu(registry, menuID);
	}
}

//===============================================
// カメラのシステムの追加
//===============================================
void CMinigameState::SetCameraSystem(entt::registry& registry)
{
	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (pCamera != nullptr)
	{
		// カメラの処理の追加
		pCamera->AddSystem(std::make_unique<CCheckCameraBehindWall>(registry));

		// カメラの揺れの追加
		auto pCameraShake = std::make_unique<CCameraShake>();
		CGameController::GetInstance()->RegisterCameraShake(pCameraShake.get());

		pCamera->AddSystem(std::move(pCameraShake));
	}
}

//===============================================
// 更新処理の生成
//================================================
void CMinigameState::CreateSystem(CScene* pScene)
{
	// 処理のマネージャークラスの取得
	CSystemManager* pSystemManager = pScene->GetSystemManager();

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	//// カメラの取得
	//CCamera* pCamera = pManager->GetCamera();

	//// カメラの処理の追加
	//pCamera->AddSystem(std::make_unique<CFollowCamera>(registry));

	// 処理の補助クラスの取得
	CWorldSystemManager* pWorldSystemManager = pManager->GetWorldSystemManager();

	// ヒットストップの生成
	auto pHitStop = std::make_unique<CHitStop>();
	CGameController::GetInstance()->RegisterHitStop(pHitStop.get());

	// ヒットストップ
	pWorldSystemManager->Register(std::move(pHitStop));

	// 更新システムの追加
	{
		pSystemManager->AddSystem(std::make_unique<UIWallSystem>());

		// プレイヤーの更新
		pSystemManager->AddSystem(std::make_unique<PlayerSystem>());

		// ジェットパックの更新処理
		pSystemManager->AddSystem(std::make_unique<JetPackSystem>());

		// プレイヤーのモーション毎の制御の追加
		pSystemManager->AddSystem(std::make_unique<PlayerMotionSystem>());

		// プレイヤーのミニゲーム時の制御の追加
		pSystemManager->AddSystem(std::make_unique<MinigamePlayerSystem>());

		// ボールの更新
		pSystemManager->AddSystem(std::make_unique<UpdateBallSystem>());

		// プレイヤーの更新
		pSystemManager->AddSystem(std::make_unique<CharacterSystem>());

		// 位置の更新システムの追加
		pSystemManager->AddSystem(std::make_unique<UpdatePosition3DSystem>());

		// 影の更新処理
		pSystemManager->AddSystem(std::make_unique<ShadowSystem>());

		{
			// フィールドの当たり判定の更新システムの追加
			pSystemManager->AddSystem(std::make_unique<FieldCollisionSystem>());

			// ボールとフィールドの当たり判定の更新システムの追加
			pSystemManager->AddSystem(std::make_unique<BallToCollisionSystem>());

			// 壁の当たり判定の更新システムの追加
			pSystemManager->AddSystem(std::make_unique<MeshWallCollisionSystem>());

			// エフェクトの壁の当たり判定
			pSystemManager->AddSystem(std::make_unique<CollisionEffectWallSystem>());

			// 壁のエフェクトの更新処理
			pSystemManager->AddSystem(std::make_unique<WallEffectSystem>());

			// メッシュの波の更新システムの追加
			pSystemManager->AddSystem(std::make_unique<MeshWaveSystem>());

			// メッシュの球体の更新処理
			pSystemManager->AddSystem(std::make_unique<MeshSphereSystem>());
		}

		// コライダーの更新処理
		pSystemManager->AddSystem(std::make_unique<ColliderSystem>());

		pSystemManager->AddSystem(std::make_unique<EffectSystem>());

		// スコアのエリアのシステム
		pSystemManager->AddSystem(std::make_unique<ScoreAreaSystem>());

		// モーションの更新システムの追加
		pSystemManager->AddSystem(std::make_unique<MotionSystem>());

		// パーティクルの更新システムの追加
		pSystemManager->AddSystem(std::make_unique<ParticleSystem>());

		// ビルボードの更新システムの追加
		pSystemManager->AddSystem(std::make_unique<UpdateBillboardSystem>());

		// メッシュサークルの更新システムの追加
		pSystemManager->AddSystem(std::make_unique<MeshCircleSystem>());

		// メッシュの軌道の更新システムの追加
		pSystemManager->AddSystem(std::make_unique<MeshOrbitSystem>());

		// 3Dポリゴンの更新処理
		pSystemManager->AddSystem(std::make_unique<MeshQuadSystem>());

		// タイマーの更新処理
		pSystemManager->AddSystem(std::make_unique<UpdateTimerSystem>());

		// スプライトシートの更新処理
		pSystemManager->AddSystem(std::make_unique<UpdateSpriteSheetSystem>());

		// チュートリアルのUIの更新処理
		pSystemManager->AddSystem(std::make_unique<TutorialUISystem>());

		// UIの点滅処理
		pSystemManager->AddSystem(std::make_unique<UIFlashSystem>());

		// フェードアウトの更新処理
		pSystemManager->AddSystem(std::make_unique<UIFadeOutSystem>());

		// テクスチャアニメーションの更新処理
		pSystemManager->AddSystem(std::make_unique<TextureAnimationSystem>());
	}

	// ワールドマトリックスの計算処理
	pSystemManager->AddSystem(std::make_unique<CalcuWorldMatrix>());

	// メニュー選択の更新処理
	pSystemManager->AddUnPauseSystem(std::make_unique<MenuSelectSystem>());

	// UIの拡大縮小アニメーション
	pSystemManager->AddUnPauseSystem(std::make_unique<UIScalingSystem>());

	// スプライトの更新処理
	pSystemManager->AddUnPauseSystem(std::make_unique<UpdateSpriteSystem>());

	// ポーズメニューの更新処理
	pSystemManager->AddUnPauseSystem(std::make_unique<PauseMenuSystem>());
}

//===============================================
// 描画処理の生成
//================================================
void CMinigameState::CreateRendererSysytem(CScene* pScene)
{
	// 処理のマネージャークラスの取得
	CSystemManager* pSystemManager = pScene->GetSystemManager();

#ifdef _DEBUG
	// コライダーの描画処理
	pSystemManager ->AddRendererSystem(std::make_unique<ColliderRenderer>());

#endif // _DEBUG

	// 描画システムの追加
	{
		// キャラクターの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<CharacterRenderer>());

#if !_DEBUG

		// アウトラインシェーダーをキャラクターに適応する描画処理
		pSystemManager->AddRendererSystem(std::make_unique<OutLineCharacterRenderer>());
#endif // !_DEBUG

		// モデルの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<ModelRenderer>());

#if !_DEBUG

		// アウトラインシェーダーをモデルに適応する描画処理
		pSystemManager->AddRendererSystem(std::make_unique<OutLineModelRenderer>());
#endif // !_DEBUG

		// メッシュの描画(マルチテクスチャ)処理
		pSystemManager->AddRendererSystem(std::make_unique<MeshRendererMT>());

		// メッシュの描画(マルチテクスチャ)処理
		pSystemManager->AddRendererSystem(std::make_unique<MeshWallRenderer>());

		// メッシュの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<MeshRenderer>());

		// 球体の描画
		pSystemManager->AddRendererSystem(std::make_unique<MeshSphereRenderer>());

		// 3Dポリゴンの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<MeshQuadRenderer>());

		// メッシュドームの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<MeshDomeRenderer>());

#if !_DEBUG

		// 球体にアウトラインシェーダーを適応する描画処理
		pSystemManager->AddRendererSystem(std::make_unique<OutLineMeshSphereRenderer>());
#endif // !_DEBUG

		// ビルボードの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<BillboardRenderer>());

		// ステンシルシャドウの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<StencilShadowRenderer>());

		// ステンシルのポリゴンの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<StencilPolygonRenderer>());

		// スプライトの描画処理
		pSystemManager->AddRendererSystem(std::make_unique<SpriteRenderer>());
	}

	// MRTのスプライトの描画処理
	pSystemManager->AddRendererMRTSystem(std::make_unique<SpriteMRTRenderer>(), MRTType::TYPE_TIMER);
}