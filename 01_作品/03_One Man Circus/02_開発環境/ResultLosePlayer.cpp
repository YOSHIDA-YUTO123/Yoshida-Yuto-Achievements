//================================================
//
// リザルト失敗のプレイヤー [ResultLosePlayer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "ResultLosePlayer.h"
#include "character3D.h"
#include "motion.h"
#include "player.h"

//=================================================
// コンストラクタ
//=================================================
CResultLosePlayer::CResultLosePlayer()
{
}

//=================================================
// デストラクタ
//=================================================
CResultLosePlayer::~CResultLosePlayer()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT CResultLosePlayer::Init(void)
{
	// 初期化処理
	if (FAILED(CScenePlayer::Init()))
	{
		return E_FAIL;
	}

	// キャラクターの取得
	CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	// 取得できなかったら
	if (pMotion == nullptr) return E_FAIL;

	// モーションの再生
	pMotion->SetMotion(CPlayer::MOTION_BOW, true, 10);

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CResultLosePlayer::Uninit(void)
{
	// 終了処理
	CScenePlayer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CResultLosePlayer::Update(void)
{
	// 更新処理
	CScenePlayer::Update();
}

//=================================================
// 描画処理
//=================================================
void CResultLosePlayer::Draw(void)
{
	// 描画処理
	CScenePlayer::Draw();
}
