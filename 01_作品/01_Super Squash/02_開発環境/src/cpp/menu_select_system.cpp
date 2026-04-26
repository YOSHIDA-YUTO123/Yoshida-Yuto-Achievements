//===================================================
//
// メニュー選択の更新処理 [menu_select_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "menu_select_system.h"
#include "input.h"
#include "manager.h"
#include "menu_select_component.hpp"
#include "utility_math.h"
#include "sound.h"

//===================================================
// 更新処理
//===================================================
void MenuSelectSystem::Update(entt::registry& registry)
{
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr) return;

	// メニュー選択の取得
	auto view = registry.view<MenuSelectComponent>();

	// 音の取得
	CSound* pSound = pManager->GetSound();

	// 取得した分回す
	for (auto entity : view)
	{
		bool bSelect = false;

		// コンポーネントの取得
		auto& menuSelectComp = registry.get<MenuSelectComponent>(entity);

		// 使用状態じゃないなら
		if (!menuSelectComp.bActive) continue;

		// 次のメニューの選択のキーを押したら
		if (pKeyboard->GetTrigger(menuSelectComp.nNextKey) || pJoyPad->GetTrigger(menuSelectComp.nNextJoyKey))
		{
			bSelect = true;
			menuSelectComp.nCurrent++;
		}
		// 前のメニューの選択のキーを押したら
		else if (pKeyboard->GetTrigger(menuSelectComp.nPrevKey) || pJoyPad->GetTrigger(menuSelectComp.nPrevJoyKey))
		{
			bSelect = true;
			menuSelectComp.nCurrent--;
		}

		// メニューを選択したら
		if (bSelect && pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_MENU_SELECT000);
		}

		// ラップするなら
		if (menuSelectComp.bWrap)
		{
			// 範囲内をループ
			menuSelectComp.nCurrent = math::Wrap(menuSelectComp.nCurrent, 0, menuSelectComp.nMax);
		}
		else
		{
			// 範囲制限
			menuSelectComp.nCurrent = math::Clamp(menuSelectComp.nCurrent, 0, menuSelectComp.nMax);
		}
	}
}