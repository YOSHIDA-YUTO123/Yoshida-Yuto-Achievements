//================================================
//
// プレイヤーのファクトリー [ScenePlayerFactory.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayerFactory.h"
#include "ResultPlayer.h"
#include "TitlePlayer.h"
#include "ResultLosePlayer.h"
#include "OpeningPlayer.h"

//=================================================
// プレイヤーの生成処理
//=================================================
CScenePlayer* ScenePlayerFactory::Create(const CScenePlayer::Type type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
    CScenePlayer* pInstance = nullptr;

	// 種類の遷移
	switch (type)
	{
	case CScenePlayer::Type::Title:
		pInstance = new CTitlePlayer;
		break;
	case CScenePlayer::Type::Opening:
		pInstance = new COpeningPlayer;
		break;
	//case CScenePlayer::Type::GameEnd:
	//	pInstance = new CTitlePlayer;
	//	break;
	case CScenePlayer::Type::ResultWin:
		pInstance = new CResultPlayer;
		break;
	case CScenePlayer::Type::ResultLose:
		pInstance = new CResultLosePlayer;
		break;
	default:
		MessageBox(NULL, "エラー", "プレイヤーの種類が判定できません", MB_OK);
		return nullptr;
	}

	// 生成処理
	pInstance->Create(pos, rot);

    return pInstance;
}
