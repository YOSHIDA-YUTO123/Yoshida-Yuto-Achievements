//===================================================
//
// シーンを操作するクラス [scene_controller.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "scene_controller.h"
#include "camera.h"
#include "camera_system_base.h"
#include "camera_animation_system.h"
#include "manager.h"
#include "light.h"
#include "scene.h"
#include "title_state.h"
#include "minigame_state.h"
#include "minigame_result_state.h"
#include "game_controller.h"

//===================================================
// コンストラクタ
//===================================================
CSceneController::CSceneController(CScene *pScene) : 
	m_state(State::Normal),
	m_pScene(pScene)
{
}

//===================================================
// デストラクタ
//===================================================
CSceneController::~CSceneController()
{
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CSceneController> CSceneController::Create(CScene* pScene)
{
	auto pInstance = std::make_unique<CSceneController>(pScene);

	// 初期化に失敗したら
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
	}

	return pInstance;
}

//===================================================
// シーンの変更
//===================================================
void CSceneController::ChangeScene(std::shared_ptr<CSceneState> pNewScene, const int nAnimType, const int nTime)
{
	// 変更している状態だったら
	if (m_state == State::ChangeScene) return;

	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 取得できないなら処理しない
	if (pCamera == nullptr) return;

	// アニメーションの取得
	CCameraAnimationSystem* pCameraAnimation = pCamera->GetAnimation();

	// 取得できないなら処理しない
	if (pCameraAnimation == nullptr) return;

	// カメラの状態の取得
	CCameraAnimationSystem::State cameraState = pCameraAnimation->GetState();

	// 状態が再生だったら処理を飛ばす
	if (cameraState == CCameraAnimationSystem::State::Play) return;

	pCamera->PlayAnimation(nAnimType);
	m_pScene->ChangeState(std::move(pNewScene), nTime);

	m_state = State::ChangeScene;
}

//===================================================
// ポーズ中かどうか
//===================================================
bool CSceneController::GetPauseState(void)
{
	// ゲームのコントローラーの取得
	CGameController* pGameController = CGameController::GetInstance();

	bool bPause = false;

	if (pGameController != nullptr)
	{
		// ポーズの設定処理
		pGameController->SetPause();

		// ポーズの取得
		bPause = pGameController->GetPause();
	}

	return bPause;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CSceneController::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CSceneController::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CSceneController::Update(void)
{
}