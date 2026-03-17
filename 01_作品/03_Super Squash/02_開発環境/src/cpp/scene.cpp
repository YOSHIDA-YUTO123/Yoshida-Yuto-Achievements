//===================================================
//
// シーンの基底クラス [scene.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "scene.h"
#include "system_base.h"
#include "renderer_system_base.h"
#include "system_manager.h"
#include "debug_info.h"
#include "input.h"
#include "manager.h"
#include "texture_mrt_manager.h"
#include "scene_controller.h"
#include "light.h"
#include "camera.h"
#include "statebase.h"
#include "sound.h"

#include "factory_mesh.h"
#include "entity_name_component.hpp"
#include "ui_scaling_system.h"
#include "update_sprite_system.h"
#include "title_menu_system.h"
#include "menu_select_system.h"
#include "calcu_world_matrix.h"
#include "mesh_renderer_mt.h"
#include "sprite_renderer.h"
#include "scene_state_base.h"
#include "title_state.h"
#include "tag_component.hpp"
#include "scene_loader.h"

#ifdef _DEBUG
#include "line.h"
#include "color_constants.hpp"
#endif // _DEBUG

//***************************************************
// 定数宣言
//***************************************************
namespace SceneConst
{
	const D3DXVECTOR3 CAMERA_POS_V	= { 145.0f,139.0f,-28.0f };					// 視点の位置
	const D3DXVECTOR3 CAMERA_POS_R	= { -168.0f,149.0f,-19.0f };				// 注視点の位置
	const D3DXVECTOR3 CAMERA_ROT	= { D3DX_PI * 0.5f,-D3DX_PI * 0.5f,0.0f };	// 向き
}

//===================================================
// コンストラクタ
//===================================================
CScene::CScene() : 
	m_registry(),
	m_nFps(0),
	m_pController(nullptr),
	m_pState(nullptr),
	m_pNextState(nullptr),
	m_nNextStateTime(0),
	m_pSystemManager(nullptr)
{
}

//===================================================
// コンストラクタ(オーバーロード)
//===================================================
CScene::CScene(const Mode mode) :
	m_mode(mode),
	m_registry(),
	m_nFps(0),
	m_pController(nullptr),
	m_pState(nullptr),
	m_pNextState(nullptr),
	m_nNextStateTime(0),
	m_pSystemManager(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CScene::~CScene()
{
	m_registry.clear();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CScene::Init(void)
{
	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->Init();
	pCamera->SetCamera(SceneConst::CAMERA_POS_V, SceneConst::CAMERA_POS_R, SceneConst::CAMERA_ROT);

	if (m_pSystemManager == nullptr)
	{
		// システムを管理するクラスの生成
		m_pSystemManager = std::make_unique<CSystemManager>();
	}

	if (m_pController == nullptr)
	{
		// 制御クラスの生成
		m_pController = CSceneController::Create(this);
	}

	if (m_pState == nullptr)
	{
		m_pState = std::make_unique<CStateMachine>();

		// 初期状態の設定
		auto pTitleState = std::make_shared<CTitleState>();
		pTitleState->SetOwner(this);
		m_pState->Change(std::move(pTitleState));
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CScene::Uninit(void)
{
	// 制御クラスの破棄
	if (m_pController != nullptr)
	{
		m_pController->Uninit();
		m_pController.reset();
	}

	// 状態の破棄
	if (m_pState != nullptr)
	{
		m_pState->Uninit();
		m_pState.reset();
	}

	if (m_pSystemManager != nullptr)
	{
		// systemの破棄処理
		m_pSystemManager->ReleaseAll();
		m_pSystemManager.reset();
	}

	m_registry.clear();
}

//===================================================
// 更新処理
//===================================================
void CScene::Update(void)
{
	// 処理できないなら
	if (m_pSystemManager == nullptr)
	{
		return;
	}

	// シーンの変更処理
	SetChangeState();

	// ポーズ中かどうか
	bool bPause = false;

	if (m_pController != nullptr)
	{
		// ポーズ状態の取得
		bPause = m_pController->GetPauseState();
	}

	// ポーズ中なら動かさない
	if (!bPause)
	{
		// systemの更新処理
		m_pSystemManager->UpdateAll(m_registry);
	}

	// ポーズの影響を受けない更新処理
	m_pSystemManager->UnPauseUpdateAll(m_registry);

	// 更新処理
	if (m_pState != nullptr && m_pController != nullptr)
	{
		// シーンの変更中じゃないなら
		if (m_pController->GetState() != CSceneController::State::ChangeScene)
		{
			m_pState->Update();
		}
	}

#ifdef _DEBUG
	auto pLine = CLineRenderer::GetInstance();
	if (pLine != nullptr)
	{
		pLine->AddLine({ 0.0f,1.0f,0.0f }, D3DXVECTOR3(0.0f, 1000.0f, 0.0f), Color::GREENYELLOW);
		pLine->AddLine({ 0.0f,1.0f,0.0f }, D3DXVECTOR3(1000.0f, 0.0f, 0.0f), Color::CRIMSON);
		pLine->AddLine({ 0.0f,1.0f,0.0f }, D3DXVECTOR3(0.0f, 0.0f, 1000.0f), Color::DARKBLUE);
	}
#endif // _DEBUG
}

//===================================================
// 描画処理
//===================================================
void CScene::Draw(void)
{
	// 処理できないなら
	if (m_pSystemManager == nullptr)
	{
		return;
	}

	// systemの描画処理
	m_pSystemManager->DrawAll(m_registry);

#ifdef _DEBUG
	auto pLine = CLineRenderer::GetInstance();
	if (pLine != nullptr)
	{
		pLine->Draw();
	}
	// デバッグ情報の描画
	CDebugInfo::Debug(m_registry, m_nFps);
#endif // _DEBUG
}

//===================================================
// マルチレンダーターゲットの描画処理
//===================================================
void CScene::DrawMRT(void)
{
	// 処理できないなら
	if (m_pSystemManager == nullptr)
	{
		return;
	}

	// systemの描画処理
	m_pSystemManager->DrawMRTAll(m_registry);
}

//===================================================
// ポーズの影響を受けない更新処理
//===================================================
void CScene::UnPauseUpdate(void)
{

}

//===================================================
// ポストプロセスを適応しない描画
//===================================================
void CScene::DrawPostProcessExcluded(void)
{
	if (m_pSystemManager == nullptr)
	{
		return;
	}
	
	// ポストプロセスを適応しない描画
	m_pSystemManager->DrawPostProcessExcluded(m_registry);
}

//===================================================
// 状態の変更
//===================================================
void CScene::ChangeState(std::shared_ptr<CSceneState> pNewState, const int nTime)
{
	if (m_pState == nullptr || pNewState == nullptr) return;

	if (m_nNextStateTime < 0)
	{
		// 次の状態の設定
		m_pNextState = pNewState;
		m_nNextStateTime = nTime;
	}
}

//===================================================
// エンティティの生成処理
//===================================================
entt::entity CScene::CreateEntity(void)
{
	// 生成処理
	return m_registry.create();
}

//===================================================
// シーンの生成
//===================================================
void CScene::CreateScene(void)
{

}

//===================================================
// シーンの変更の設定処理
//===================================================
void CScene::SetChangeState(void)
{
	m_nNextStateTime--;

	// カウンターが0になったら
	if (m_pNextState != nullptr && m_nNextStateTime <= 0)
	{
		// 音の取得
		CSound* pSound = CManager::GetInstance()->GetSound();

		if (pSound != nullptr)
		{
			// 音の停止
			pSound->StopSound();
		}

		// systemの破棄処理
		m_pSystemManager->ReleaseAll();

		m_registry.clear();

		if (m_pController != nullptr)
		{
			// シーンの変更が終わった
			m_pController->FInishChangeScene();
		}

		// レジストリーの設定
		m_pNextState->SetOwner(this);
		
		// 状態の変更
		m_pState->Change(std::move(m_pNextState));
	}
}