//================================================
//
// ミニゲームのリザルトの状態 [minigame_result_state.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "minigame_result_state.h"
#include "system_manager.h"
#include "scene_controller.h"
#include "input.h"
#include "camera_animation_system.h"
#include "tag_component.hpp"
#include "entity_name_component.hpp"
#include "fstream"
#include "result_loader.h"
#include "sound.h"
#include "texture_mt_manager.h"
#include "scene_loader.h"
#include "ui_wall_controller.h"

// 更新処理
#include "calcu_world_matrix.h"
#include "score_system.h"
#include "update_sprite_sheet_system.h"
#include "ranking_highlight_system.h"
#include "result_ball_system.h"
#include "update_position_3d_system.h"
#include "mesh_wall_collision_system.h"
#include "update_ball_system.h"
#include "update_ball_system.h"
#include "update_sprite_system.h"
#include "ui_slide_system.h"
#include "ui_wall_system.h"

// レンダラー
#include "mesh_renderer_mt.h"
#include "mesh_renderer.h"
#include "mesh_sphere_renderer.h"
#include "scene.h"
#include "manager.h"
#include "title_state.h"
#include "mesh_wall_renderer.h"
#include "sprite_mrt_renderer.h"
#include "model_renderer_mrt.h"

// ファクトリー
#include "factory_system_entity.h"
#include "factory_2d.h"
#include "factory_model.h"
#include "factory_mesh.h"

#include "texture_id_component.hpp"
#include "texture_manager.h"
#include "result_system_component.hpp"
#include "ui_slide_component.hpp"
#include "result_system.h"

//************************************************
// 定数宣言
//************************************************
namespace MinigameResultStateConst
{
	constexpr const char* UI_000_TEXTURE			= "data/TEXTURE/result_ui.png";			// UIのテクスチャ
	constexpr const char* UI_SCORE_TEXTURE			= "data/TEXTURE/number001.png";			// スコアのテクスチャ
	constexpr const char* UI_RANKING_TEXTURE		= "data/TEXTURE/number001.png";			// ランキングのテクスチャ
	constexpr const char* RANKING_TEXTURE			= "data/TEXTURE/number001.png";			// スコアのテクスチャ
	constexpr const char* SUPPORT_TEXTURE			= "data/TEXTURE/result_support.png";	// リザルトのサポートのテクスチャ

	const D3DXVECTOR2 RESULT_SUPPORT_POS			= { 1050.0f,630.0f };					// リザルトのサポートUIの位置
	const D3DXVECTOR2 RESULT_SUPPORT_SIZE			= { 170.0f,80.0f };						// リザルトのサポートUIの大きさ

	const D3DXVECTOR2 UI_POS						= { 1580.0f,200.0f };					// UIの位置
	const D3DXVECTOR2 UI_DEST_POS					= { 640.0f,200.0f };					// UIの目的位置
	constexpr int UI_SLIDE_TIME						= 60;									// UIのスライド時間

	const D3DXVECTOR2 UI_SCORE_POS					= { 880.0f,400.0f };					// スコアの位置
	const D3DXVECTOR2 UI_RANKING_POS				= { 1000.0f, 250.0f };					// ランキングの位置

	const D3DXVECTOR2 UI_000_SIZE					= { 300.0f, 100.0f };					// UIの大きさ
	const D3DXVECTOR2 UI_SCORE_SIZE					= { 300.0f,100.0f };					// スコアの大きさ
	const D3DXVECTOR2 UI_RANKING_SIZE				= { 80.0f,30.0f };						// ランキングの大きさ

	constexpr int CHANGE_SCENE_TIME					= 30;									// シーンの変更時間
	constexpr int BALL_CREATE_TIME					= 100;									// ボールの生成時間

	constexpr int MAX_RANK							= 5;									// 順位の数
	const D3DXVECTOR2 RANK_TOP_POS					= { 400.0f, 100.0f };					// ランキングの1位の位置
	const D3DXVECTOR2 RANK_SIZE						= { 50.0f, 50.0f };						// ランキングの順位の大きさ
	const D3DXVECTOR2 RANKING_POS					= { 950.0f,100.0f };					// ランキングの位置
	const D3DXVECTOR2 RANKING_SIZE					= { 250.0f,50.0f };						// ランキングの大きさ

	// ランキングのテクスチャ
	const char* RANK_TEXTURE[MAX_RANK] =
	{
		"data/TEXTURE/rank_1.png",
		"data/TEXTURE/rank_2.png",
		"data/TEXTURE/rank_3.png",
		"data/TEXTURE/rank_4.png",
		"data/TEXTURE/rank_5.png",
	};
}

//================================================
// コンストラクタ
//================================================
CMinigameResultState::CMinigameResultState() : CSceneState(STATE_MINIGAME_RESULT)
{
}

//================================================
// デストラクタ
//================================================
CMinigameResultState::~CMinigameResultState()
{
}

//================================================
// 初期化処理
//================================================
HRESULT CMinigameResultState::Init(void)
{
	// オーナーの取得
	CScene* pScene = CSceneState::GetOwner();

	// レジストリーの取得
	auto& registry = pScene->GetRegistry();

	// 定数の使用
	using namespace MinigameResultStateConst;

	// 基盤のシーンのロード
	Loader::Scene::BaseScene(registry);

	// 処理のマネージャークラスの取得
	CSystemManager* pSystemManager = pScene->GetSystemManager();

	// スコアのロード
	int nScore = Loader::Result::GetScore();

	// リザルトのUIの設定
	UIWallController::SetResultUI(registry);

	// 2Dのパラメータ
	Factory2D::Param param;

	// パラメータの設定
	param.pos = UI_POS;
	param.size = UI_000_SIZE;
	param.texturePath = UI_000_TEXTURE;

	// スコアの描画
	auto resultUI = Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RESULT);
	registry.emplace<UISlideComponent>(resultUI, UI_DEST_POS, UI_DEST_POS, UI_SLIDE_TIME, UI_POS);

	// パラメータの設定
	param.pos = Const::CENTER_POS_2D;
	param.size = { SCREEN_WIDTH,SCREEN_HEIGHT };
	param.texturePath = "";
	param.nPriority = -1;

	// スプライトの生成
	Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RESULT);

	// スコアの生成
	Factory2D::Create::ScoreMRTResult(registry, nScore);

	// パラメータの設定
	param.pos = RANKING_POS;
	param.size = RANKING_SIZE;
	param.texturePath = RANKING_TEXTURE;

	// ランキングの生成
	Factory2D::Create::RankingMRT(registry, param, nScore);

	// パラメータの設定
	param.pos = Const::CENTER_POS_2D;
	param.size = { SCREEN_WIDTH,SCREEN_HEIGHT };
	param.texturePath = "";
	param.nPriority = -1;

	// 背景の生成
	Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RANKING);

	// ランキングの順位数分回す
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		// yの位置のオフセットを求める
		float fOffsetHeight = (RANK_SIZE.y * 2.0f) * nCnt;

		// パラメータの設定
		param.pos = { RANK_TOP_POS.x,RANK_TOP_POS.y + fOffsetHeight };
		param.size = RANK_SIZE;
		param.texturePath = RANK_TEXTURE[nCnt];
		param.nPriority = 0;

		// 順位の生成
		Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RANKING);
	}

	// パラメータの設定
	param.pos = RESULT_SUPPORT_POS;
	param.size = RESULT_SUPPORT_SIZE;
	param.texturePath = SUPPORT_TEXTURE;

	// リザルトのサポートUIの生成
	Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RESULT);
	Factory2D::Create::SpriteMRT(registry, param, MRTType::TYPE_RANKING);

	// リザルトのコンポーネントの生成
	auto resultSystemID = registry.create();
	registry.emplace<ResultSystemComponent>(resultSystemID, BALL_CREATE_TIME);

	// 更新処理の生成
	CreateSystem(pSystemManager);

	// 描画処理の生成
	CreateRendererSystem(pSystemManager);

	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::LABEL_BGM_RESULT);
	}

	return S_OK;
}

//================================================
// 更新処理
//================================================
void CMinigameResultState::Update(void)
{

}

//================================================
// 終了処理
//================================================
void CMinigameResultState::Uninit(void)
{
	// 音の取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
	{
		// 音の停止
		pSound->StopSound();
	}
}

//================================================
// 更新処理の生成
//================================================
void CMinigameResultState::CreateSystem(CSystemManager* pSystemManager)
{
	// スライドUIの更新処理
	pSystemManager->AddSystem(std::make_unique<UIWallSystem>());

	// スライドUIの更新処理
	pSystemManager->AddSystem(std::make_unique<UISlideSystem>());

	// リザルトの更新処理
	pSystemManager->AddSystem(std::make_unique<ResultSystem>());

	// リザルトのボールの更新処理
	pSystemManager->AddSystem(std::make_unique<ResultBallSystem>());

	// 位置の更新処理
	pSystemManager->AddSystem(std::make_unique<UpdatePosition3DSystem>());

	// スプライトの更新処理
	pSystemManager->AddSystem(std::make_unique<UpdateSpriteSystem>());

	// メッシュの壁の当たり判定
	pSystemManager->AddSystem(std::make_unique<MeshWallCollisionSystem>());

	// スコアの更新処理
	pSystemManager->AddSystem(std::make_unique<ScoreSystem>());

	// ランキングのハイライトの更新処理
	pSystemManager->AddSystem(std::make_unique<RankingHighLightSystem>());


	// スプライトシートの更新処理
	pSystemManager->AddSystem(std::make_unique<CalcuWorldMatrix>());

	// 球体の描画
	pSystemManager->AddSystem(std::make_unique<UpdateSpriteSheetSystem>());
}

//================================================
// 描画処理の生成
//================================================
void CMinigameResultState::CreateRendererSystem(CSystemManager* pSystemManager)
{
	// ワールドマトリックスの計算処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshSphereRenderer>());

	// メッシュの描画(マルチテクスチャ)処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshRendererMT>());

	// メッシュの壁の描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshWallRenderer>());

	// メッシュの描画処理
	pSystemManager->AddRendererSystem(std::make_unique<MeshRenderer>());

	// リザルトのマルチレンダーターゲットのスプライトの描画処理
	pSystemManager->AddRendererMRTSystem(std::make_unique<SpriteMRTRenderer>(), MRTType::TYPE_RESULT);

	// ランキングのマルチレンダーターゲットのスプライトの描画処理
	pSystemManager->AddRendererMRTSystem(std::make_unique<SpriteMRTRenderer>(), MRTType::TYPE_RANKING);
}