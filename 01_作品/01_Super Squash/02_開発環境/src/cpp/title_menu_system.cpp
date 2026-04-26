//===================================================
//
// タイトルのメニューの更新処理 [title_menu_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "title_menu_system.h"
#include "tag_component.hpp"
#include "menu_component.hpp"
#include "menu_select_component.hpp"
#include "tItle_menu.hpp"
#include "manager.h"
#include "input.h"
#include "ui_scaling_component.hpp"
#include "camera_animation_system.h"
#include "scene_controller.h"
#include "minigame_state.h"
#include "ui_flash_component.hpp"
#include "sound.h"
#include "titlemenu_animation_component.hpp"
#include "scene.h"
#include "scene_change_constants.hpp"

//===================================================
// 更新処理
//===================================================
void TitleMenuSystem::Update(entt::registry& registry)
{
	// マネージャーの取得
	CManager *pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// 音の取得
	CSound* pSound = pManager->GetSound();

	// タイトルのメニューの取得
	auto view = registry.view<Tag::TitleMenuTag>();

	// 取得した分回す
	for (auto entity : view)
	{
		auto& menuComp = registry.get<MenuComponent>(entity);

		// 選択処理がないなら
		if (!registry.valid(menuComp.selectMenuID))
		{
			continue;
		}

		// メニュー選択のコンポーネントの取得
		auto& menuSelectComp = registry.get<MenuSelectComponent>(menuComp.selectMenuID);
		auto& titleMenuAnimationComp = registry.get<TitleMenuAnimationComponent>(entity);

		menuSelectComp.bActive = true;
		
		// 選択中か判定
		const bool bSelect = menuSelectComp.nCurrent == menuComp.nID;

		// 選択されていたらtrue : されていないならfalse
		titleMenuAnimationComp.bPlay = bSelect ? true : false;
		
		// triggerを押してないなら
		if (!pKeyboard->GetTrigger(DIK_RETURN) && !pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)) continue;
		
		// 現在選択中のメニューと一致したら
		if (menuSelectComp.nCurrent == TItleMenu::TYPE_MINIGAME && bSelect)
		{
			if (pSound != nullptr)
			{
				pSound->Play(CSound::LABEL_SE_TITLE_DECISION);
			}

			// シーンの取得
			CScene* pScene = pManager->GetScene();

			// シーンのコントローラーの取得
			CSceneController* pSceneController = pScene->GetController();

			// カメラのアニメーションの取得
			pSceneController->ChangeScene(std::make_shared<CMinigameState>(),CCameraAnimationSystem::TYPE_TITLE_TO_GAME, SceneChangeConst::TITLE_TO_RESULT);

			menuSelectComp.bActive = false;
		}
		// 現在選択中のメニューと一致したら
		else if (menuSelectComp.nCurrent == TItleMenu::TYPE_QUIT)
		{
			PostQuitMessage(0);
		}	
	}
}