//================================================
//
// シーンのタイトルの状態の処理 [title_state.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "title_state.h"
#include "manager.h"
#include "input.h"
#include "tItle_menu.hpp"
#include "tag_component.hpp"
#include "entity_name_component.hpp"
#include "color_constants.hpp"
#include "sound.h"
#include "scene.h"
#include "texture_mrt_manager.h"
#include "wall_constants.hpp"
#include "camera.h"
#include "ui_wall_controller.h"
#include "camera_animation_system.h"
#include "rotation_move_component.hpp"
#include "mrt_target_component.hpp"
#include "menu_select_component.hpp"
#include "menu_component.hpp"
#include "titlemenu_animation_component.hpp"
#include "scene_loader.h"
#include "mesh_sky_loader.h"

// 更新処理
#include "system_manager.h"
#include "ui_scaling_system.h"
#include "update_sprite_system.h"
#include "menu_select_system.h"
#include "title_menu_system.h"
#include "mesh_sphere_system.h"
#include "ui_flash_system.h"
#include "mesh_sphere_renderer_mrt.h"
#include "rotation_move_system.h"
#include "titlemenu_animation_system.h"
#include "ui_wall_system.h"
#include "title_wall_system.h"

// レンダラー
#include "sprite_renderer.h"
#include "mesh_sphere_renderer.h"
#include "shader_mesh_wall_renderer.h"
#include "model_renderer.h"
#include "model_renderer_mrt.h"
#include "mesh_quad_renderer.h"
#include "sprite_mrt_renderer.h"
#include "mesh_renderer.h"
#include "mesh_wall_renderer.h"
#include "outline_model_renderer.h"
#include "shadow_model_renderer.h"
#include "shadowmap_recieve_field_renderer.h"

// ファクトリー
#include "factory_2d.h"
#include "factory_system_entity.h"
#include "calcu_world_matrix.h"
#include "mesh_renderer_mt.h"
#include "field_effect_renderer.h"
#include "factory_mesh.h"
#include "factory_model.h"
#include "factory_mesh_quad.h"

#ifdef _DEBUG
#include "scene_controller.h"
#include "minigame_result_state.h"
#include "scene.h"
#include "transform_component.hpp"
#endif // _DEBUG


//***************************************************
// 定数宣言
//***************************************************
namespace Title_Constants
{
	constexpr int WALL_CREATE_TIME			= 180;							// 壁を生成する時間
	constexpr int SET_TITLE_LOGO_TIME		= 420;							// タイトルロゴを生成する時間

	const D3DXVECTOR3 TITLE_LOGO_POS		= { 0.0f,150.0f,0.0f };			// タイトルロゴの位置
	const D3DXVECTOR3 TITLE_START_POS		= { 0.0f,100.0f,0.0f };			// タイトルロゴの位置
	const D3DXVECTOR3 TITLE_QUIT_POS		= { 0.0f,50.0f,0.0f };			// タイトルロゴの位置

	constexpr const char* TITLE_LOGO_PATH	= "title_logo.x";				// タイトルロゴのパス
	constexpr const char* TITLE_START_PATH	= "title_start.x";				// タイトルの選択肢スタートのパス
	constexpr const char* TITLE_QUIT_PATH	= "title_quit.x";				// タイトルの選択肢やめるのパス
}

//================================================
// コンストラクタ
//================================================
CTitleState::CTitleState() :
	CSceneState(STATE_TITLE),
	nWallCreateTime(Title_Constants::WALL_CREATE_TIME),
	nSetTitlteLogoTime(Title_Constants::SET_TITLE_LOGO_TIME)
{
}

//================================================
// デストラクタ
//================================================
CTitleState::~CTitleState()
{
}

//================================================
// 初期化処理
//================================================
HRESULT CTitleState::Init(void)
{
	// 定数の名前空間の使用
	using namespace Title_Constants;

	// オーナーの取得
	CScene* pScene = CSceneState::GetOwner();

	// レジストリーの取得
	auto& registry = pScene->GetRegistry();

	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 取得できないなら処理しない
	if (pCamera == nullptr) return E_FAIL;

	// カメラのアニメーションの再生
	pCamera->PlayAnimation(CCameraAnimationSystem::TYPE_CREATE);

	// 処理のマネージャークラスの取得
	CSystemManager* pSystemManager = pScene->GetSystemManager();

	// 空の読み込み
	CMeshSkyLoader::Load(registry);

	// 更新処理の生成
	CreateSystem(pSystemManager);

	// 描画処理の生成
	CreateRendererSystem(pSystemManager);

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_BGM_CREATE_WALL);
	}

	return S_OK;
}

//================================================
// 更新処理
//================================================
void CTitleState::Update(void)
{
	// オーナーの取得
	CScene* pScene = CSceneState::GetOwner();

	// レジストリーの取得
	auto& registry = pScene->GetRegistry();

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// 壁の生成処理
	CreateWall(registry, pSound);

	// タイトルロゴの生成処理
	CreateSetTitleLogo(registry, pSound);

#ifdef _DEBUG
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F5))
	{
		// シーンの操作クラスの取得
		CSceneController* pSceneController = pScene->GetController();

		pSceneController->ChangeScene(std::make_shared<CMinigameResultState>(), CCameraAnimationSystem::TYPE_GAME_TO_RESULT, 120);
	}
#endif // _DEBUG
}

//================================================
// 終了処理
//================================================
void CTitleState::Uninit(void)
{
}

//===================================================
// タイトルメニューの作成
//===================================================
void CTitleState::CreateTitleMenu(entt::registry& registry)
{
	// 定数の使用
	using namespace Title_Constants;

	// タイトルのマルチレンダーターゲットの生成
	FactorySystemEntity::CreateTitleMRT(registry);

	// タイトルのメニューの生成
	auto menuSelectID = FactorySystemEntity::CreateMenuSelect(registry, TItleMenu::TYPE_QUIT, DIK_UP, CInputJoypad::JOYKEY_UP, true);
	FactorySystemEntity::BuildMenuSelect(registry, menuSelectID, DIK_DOWN, CInputJoypad::JOYKEY_DOWN);

	// タイトルロゴの生成
	FactoryModel::CreateOutLineModel(registry, TITLE_LOGO_POS, Const::VEC3_NULL, TITLE_LOGO_PATH);

	// タイトルの選択肢の生成
	auto titleMenu = FactoryModel::CreateModel(registry, TITLE_START_POS, Const::VEC3_NULL, TITLE_START_PATH);
	registry.emplace<Tag::TitleMenuTag>(titleMenu);
	registry.emplace<RendererTag::ShadowMapModel>(titleMenu);
	registry.emplace<TitleMenuAnimationComponent>(titleMenu);
	registry.emplace<MenuComponent>(titleMenu, menuSelectID, TItleMenu::TYPE_MINIGAME);

	// タイトルの選択肢の生成
	titleMenu = FactoryModel::CreateModel(registry, TITLE_QUIT_POS, Const::VEC3_NULL, TITLE_QUIT_PATH);
	registry.emplace<Tag::TitleMenuTag>(titleMenu);
	registry.emplace<RendererTag::ShadowMapModel>(titleMenu);
	registry.emplace<TitleMenuAnimationComponent>(titleMenu);
	registry.emplace<MenuComponent>(titleMenu, menuSelectID, TItleMenu::TYPE_QUIT);
}

//===================================================
// 更新処理の生成
//===================================================
void CTitleState::CreateSystem(CSystemManager* pSystemManager)
{
	// UIの壁の更新処理
	pSystemManager->AddSystem(std::make_unique<TitleWallSystem>());

	// UIの壁の更新処理
	pSystemManager->AddSystem(std::make_unique<UIWallSystem>());

	// タイトルのメニューの更新処理
	pSystemManager->AddSystem(std::make_unique<TitleMenuAnimationSystem>());

	pSystemManager->AddSystem(std::make_unique<RotationMoveSystem>());

	// タイトルのメニューの更新処理
	pSystemManager->AddSystem(std::make_unique<UIScalingSystem>());

	// スプライトの更新処理
	pSystemManager->AddSystem(std::make_unique<UpdateSpriteSystem>());

	// タイトルのメニューの更新処理
	pSystemManager->AddSystem(std::make_unique<TitleMenuSystem>());

	// メニューの選択処理
	pSystemManager->AddSystem(std::make_unique<MenuSelectSystem>());

	// メッシュの球体の更新処理
	pSystemManager->AddSystem(std::make_unique<MeshSphereSystem>());

	// ワールドマトリックスの計算処理
	pSystemManager->AddSystem(std::make_unique<CalcuWorldMatrix>());
}

//===================================================
// 描画処理の生成
//===================================================
void CTitleState::CreateRendererSystem(CSystemManager* pSystemManager)
{
	// メッシュの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<ShadowMapRecieveFIeldRenderer>());

	// メッシュの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshRenderer>());

	// MRTのメッシュの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshRendererMT>());

	// メッシュの壁の描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshWallRenderer>());

	// メッシュの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<OutLineModelRenderer>());

	// モデルの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<ModelRenderer>());

	// 3Dポリゴンの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshQuadRenderer>());

	// メッシュの球体の描画
	pSystemManager->AddRendererSystem(std::make_unique<MeshSphereRenderer>());

	// シャドウマップのモデルの描画
	pSystemManager->AddRendererMRTSystem(std::make_unique<ShadowModelRenderer>(), CTextureMRTManager::TYPE_SHADOW_MAP);
}

//===================================================
// 壁の生成
//===================================================
void CTitleState::CreateWall(entt::registry& registry, CSound* pSound)
{
	nWallCreateTime--;

	// 0になったら
	if (nWallCreateTime == 0)
	{
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_CREATE_WALL);
		}

		// 基盤のシーンのロード
		Loader::Scene::FirstScene(registry);
	}
}

//===================================================
// タイトルロゴの生成処理
//===================================================
void CTitleState::CreateSetTitleLogo(entt::registry& registry, CSound* pSound)
{
	// タイトルロゴの設定時間
	nSetTitlteLogoTime--;

	if (nSetTitlteLogoTime == 0)
	{
		if (pSound != nullptr)
		{
			// BGMを停止
			pSound->StopSound(CSound::LABEL_BGM_CREATE_WALL);
			pSound->Play(CSound::LABEL_BGM_TITLE);
		}

		// タイトルメニューの生成
		CreateTitleMenu(registry);
		UIWallController::SetTitleWall(registry);
	}
}