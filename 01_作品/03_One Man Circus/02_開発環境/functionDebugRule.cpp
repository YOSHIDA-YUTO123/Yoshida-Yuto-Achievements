//===============================================
// 
// デバッグ時のルールの処理クラス [functionDebugRule.cpp]
// Author:Rio Ohno
// 
//===============================================

// インクルード
#include "functionDebugRule.h"
#include "game.h"
#include "manager.h"
#include "input.h"
#include "ruleJuggling.h"
#include "ruleHula.h"
#include "ruleBalance.h"

//===============================================
// デバッグ時のルールの処理クラス更新
//===============================================
void CFuncDebRule::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		if (pKeyboard->GetTrigger(DIK_8))
		{
			// ルール：ジャグリング起動
			CGame::ChangeRule(new CRuleJug);
		}
		else if (pKeyboard->GetTrigger(DIK_9))
		{
			// ルール：フラフープ起動
			CGame::ChangeRule(new CRuleHula);
		}
		else if (pKeyboard->GetTrigger(DIK_0))
		{
			// ルール：縄跳び起動
			CGame::ChangeRule(new CRuleBalance);
		}
	}
}