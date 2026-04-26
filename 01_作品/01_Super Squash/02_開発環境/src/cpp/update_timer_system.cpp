//===================================================
//
// タイマーの更新処理 [update_timer_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_timer_system.h"
#include "timer_component.hpp"
#include "sheet_component.hpp"
#include "utility_math.h"
#include "color_component.hpp"
#include "color_constants.hpp"

//===================================================
// 更新処理
//===================================================
void UpdateTimerSystem::Update(entt::registry& registry)
{
	// タイマーの取得
	auto view = registry.view<TimerComponent>();

	// タイマー分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& timerComp = registry.get<TimerComponent>(entity);

		// 分と秒の計算
		timerComp.nMinuteValue = timerComp.nTime / Const::FRAME;
		timerComp.nSecondValue = timerComp.nTime % Const::FRAME;

		// 桁数分回す
		for (int nCnt = 0; nCnt < timerComp.MAX_DIGIT; nCnt++)
		{
			// 分と秒のスプライトシートの取得
			auto& minuteSpriteComp = registry.get<SheetComponent>(timerComp.aMinuteID[nCnt]);
			auto& secondSpriteComp = registry.get<SheetComponent>(timerComp.aSecondID[nCnt]);

			int nData = 10;

			// 桁を求める準備をする
			int nDivi = (int)pow((double)nData, (double)nCnt);

			// 分の桁の数字を求める
			int nNumber = timerComp.nMinuteValue / nDivi % nData;

			float fPosTexU = 0.1f * nNumber;

			// テクスチャ座標の設定(分)
			minuteSpriteComp.aPosUV[0] = D3DXVECTOR2(fPosTexU, 0.0f);
			minuteSpriteComp.aPosUV[1] = D3DXVECTOR2(0.1f + fPosTexU, 0.0f);
			minuteSpriteComp.aPosUV[2] = D3DXVECTOR2(fPosTexU, 1.0f);
			minuteSpriteComp.aPosUV[3] = D3DXVECTOR2(0.1f + fPosTexU, 1.0f);

			// 秒桁の数字を求める
			nNumber = timerComp.nSecondValue / nDivi % nData;

			fPosTexU = 0.1f * nNumber;

			// テクスチャ座標の設定(秒)
			secondSpriteComp.aPosUV[0] = D3DXVECTOR2(fPosTexU, 0.0f);
			secondSpriteComp.aPosUV[1] = D3DXVECTOR2(0.1f + fPosTexU, 0.0f);
			secondSpriteComp.aPosUV[2] = D3DXVECTOR2(fPosTexU, 1.0f);
			secondSpriteComp.aPosUV[3] = D3DXVECTOR2(0.1f + fPosTexU, 1.0f);

			// 残り10秒になったら
			if (timerComp.nMinuteValue <= 0 && timerComp.nSecondValue <= 10)
			{
				SetLastTimer(registry, timerComp.aMinuteID[nCnt]);
				SetLastTimer(registry, timerComp.aSecondID[nCnt]);
			}
		}

		timerComp.nCounter++;

		// i秒経過したら
		if (timerComp.nCounter >= Const::FRAME)
		{
			// タイムを減らす
			timerComp.nTime--;

			timerComp.nTime = math::Clamp(timerComp.nTime, 0, 999999);
			timerComp.nCounter = 0;
		}
	}
}

//===================================================
// 終了間際の処理
//===================================================
void UpdateTimerSystem::SetLastTimer(entt::registry& registry, const entt::entity sheetID)
{
	// コンポーネントの取得
	auto& colorComp = registry.get<ColorComponent>(sheetID);

	colorComp.col = Color::RED;
}