//===================================================
//
// ポーズのメニューの更新処理 [pause_menu_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "pause_menu_system.h"
#include "menu_component.hpp"
#include "menu_select_component.hpp"
#include "tag_component.hpp"
#include "manager.h"
#include "input.h"
#include "pause_menu.hpp"
#include "game_controller.h"
#include "ui_scaling_component.hpp"
#include "renderer_component.hpp"
#include "scene.h"
#include "scene_controller.h"
#include "minigame_state.h"
#include "camera_animation_system.h"
#include "title_state.h"
#include "ui_flash_component.hpp"
#include "sound.h"

//===================================================
// 更新処理
//===================================================
void PauseMenuSystem::Update(entt::registry& registry)
{
	// ゲームの操作クラスの取得
	auto pController = CGameController::GetInstance();

	// 取得できないなら処理しない
	if (pController == nullptr) return;

	// ポーズの取得
	auto view = registry.view<Tag::PauseMenuTag>();

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// ポーズの取得
	bool bPause = pController->GetPause();

	// 取得した分回す
	for (auto entity : view)
	{
		auto& menuComp = registry.get<MenuComponent>(entity);
		auto& rendererComp = registry.get<RendererComponent>(entity);

		if (bPause)
		{
			// 描画する
			rendererComp.nFlag = RendererComponent::TYPE_NONE;
		}
		else
		{
			// 描画OFF
			rendererComp.nFlag = RendererComponent::TYPE_NO_DISP;
			continue;
		}

		// 選択処理がないなら
		if (!registry.valid(menuComp.selectMenuID))
		{
			continue;
		}

		// メニュー選択のコンポーネントの取得
		auto& menuSelectComp = registry.get<MenuSelectComponent>(menuComp.selectMenuID);
		auto& uiScalingComp = registry.get<UIScalingComponent>(entity);
		auto& uiFlashComp = registry.get<UIFlashComponent>(entity);

		menuSelectComp.bActive = true;

		// 選択されていたら
		if (menuSelectComp.nCurrent == menuComp.nID)
		{
			// アニメーション
			uiScalingComp.bPlay = true;
		}
		else
		{
			// アニメーション
			uiScalingComp.bPlay = false;
		}

		// triggerを押してないなら
		if (!pKeyboard->GetTrigger(DIK_RETURN) && !pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)) continue;

		if (menuSelectComp.nCurrent == menuComp.nID)
		{
			// 音の取得
			CSound* pSound = pManager->GetSound();

			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_MENU_DECISION);
			}
			uiFlashComp.bPlay = true;
		}
		else
		{
			uiFlashComp.bPlay = false;
		}
		// メニューの遷移
		switch (menuSelectComp.nCurrent)
		{
		case PauseMenu::TYPE_CONTINUE:
			pController->SetPause(false);
			break;
		case PauseMenu::TYPE_RETRY:
		{
			// シーンの取得
			CScene* pScene = pManager->GetScene();

			CSceneController* pSceneController = pScene->GetController();

			// カメラのアニメーションの取得
			pSceneController->ChangeScene(std::make_shared<CMinigameState>(), CCameraAnimationSystem::TYPE_RETRY, 120);
		}
			break;
		case PauseMenu::TYPE_QUIT:
		{
			// シーンの取得
			CScene* pScene = pManager->GetScene();

			CSceneController* pSceneController = pScene->GetController();

			// カメラのアニメーションの取得
			pSceneController->ChangeScene(std::make_shared<CTitleState>(), CCameraAnimationSystem::TYPE_REULT_TO_TITLE, 120);
		}
			//pManager->BeginFade(std::make_unique<CTitle>());
			break;
		default:
			break;
		}
	}
}