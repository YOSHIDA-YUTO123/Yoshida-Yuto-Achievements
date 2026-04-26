//===================================================
//
// スコアの更新処理 [score_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "score_system.h"
#include "score_component.hpp"
#include "sheet_component.hpp"

//===================================================
// 更新処理
//===================================================
void ScoreSystem::Update(entt::registry& registry)
{
	// スコアの取得
	auto view = registry.view<ScoreComponent>();

	// 取得した分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& scoreComp = registry.get<ScoreComponent>(entity);

		int nCnt = 0;

		// スコアの取得
		for (const auto& number : scoreComp.aNumberID)
		{
			// 使用できないなら
			if (!registry.valid(number))
			{
				continue;
			}

			// シートの取得
			auto& sheetComp = registry.get<SheetComponent>(number);

			int nData = 10;

			// 桁を求める準備をする
			int nDivi = (int)pow((double)nData, (double)nCnt);

			// 分の桁の数字を求める
			int nNumber = scoreComp.nScore / nDivi % nData;

			float fPosTexU = 0.1f * nNumber;

			// テクスチャ座標の設定(分)
			sheetComp.aPosUV[0] = D3DXVECTOR2(fPosTexU, 0.0f);
			sheetComp.aPosUV[1] = D3DXVECTOR2(0.1f + fPosTexU, 0.0f);
			sheetComp.aPosUV[2] = D3DXVECTOR2(fPosTexU, 1.0f);
			sheetComp.aPosUV[3] = D3DXVECTOR2(0.1f + fPosTexU, 1.0f);

			nCnt++;
		}
	}
}