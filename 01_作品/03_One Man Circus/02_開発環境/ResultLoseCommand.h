//===================================================
//
// リザルト失敗のコマンドクラス [ResultLoseCommand.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RESULT_LOSE_COMMAND_H_
#define _RESULT_LOSE_COMMAND_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "MenuCommandBase.h"

//***************************************************
// リザルト失敗時のメニューのコマンド(リトライ)
//***************************************************
class CResultLoseRetryCommand : public CMenuCommandBase
{
public:
	CResultLoseRetryCommand() = default;
	~CResultLoseRetryCommand() = default;

	void Execute(void) override;
private:
};

//***************************************************
// リザルト失敗時のメニューのコマンド(やめる)
//***************************************************
class CResultLoseQuitCommand : public CMenuCommandBase
{
public:
	CResultLoseQuitCommand() = default;
	~CResultLoseQuitCommand() = default;

	void Execute(void) override;
private:
};

#endif