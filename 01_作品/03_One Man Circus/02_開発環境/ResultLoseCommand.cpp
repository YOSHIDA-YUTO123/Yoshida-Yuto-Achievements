//===================================================
//
// リザルト失敗のコマンドクラス [ResultLoseCommand.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ResultLoseCommand.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "title.h"

//===================================================
// 実行される処理(リトライ)
//===================================================
void CResultLoseRetryCommand::Execute(void)
{
	// ゲームシーンへ移行
	CManager::SetFade(std::make_unique<CGame>());
}

//===================================================
// 実行される処理(やめる)
//===================================================
void CResultLoseQuitCommand::Execute(void)
{
	// タイトルシーンへ移行
	CManager::GetFade()->SetFade(std::make_unique<CTitle>(), 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
}