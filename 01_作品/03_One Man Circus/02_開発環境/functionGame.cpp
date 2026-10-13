//**************************************************
// 
// 機能ルール管理処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionGame.h"
#include "game.h"
#include "player.h"
#include "rulebase.h"
#include "pause.h"

//-------------------------------------------------
// 機能ルール管理処理クラス

//**************************************************
// 機能ルール管理処理クラス更新
//**************************************************
void CFuncRulu::Update(void)
{
	// ルールの取得
	CRuleBase* pRule = CGame::GetRule();

	int result = -1;

	// 更新処理
	if (pRule != nullptr)
	{
		result = pRule->Update();
	}

	/*if (result == CRuleBase::RESULT_LIMIT || 
		result == CRuleBase::RESULT_SUCCESS)
	{
		CGame::DestoroyRule();
	}*/
}

//-------------------------------------------------
// 機能プレイヤーステート管理処理クラス

//**************************************************
// 機能プレイヤーステート管理処理クラス更新
//**************************************************
void CFuncPlayerState::Update(void)
{

}