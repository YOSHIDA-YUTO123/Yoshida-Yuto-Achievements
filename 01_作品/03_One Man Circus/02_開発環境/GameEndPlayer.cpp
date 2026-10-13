//================================================
//
// リザルトのプレイヤー [CGameEndPlayer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "GameEndPlayer.h"
#include "character3D.h"
#include "player.h"
#include "motion.h"
#include "Audience.h"

//*************************************************
// 定数宣言
//*************************************************
namespace CGameEndPlayer_Constants
{
	constexpr int BLEND_FRAME = 15; // モーションブレンドのフレーム(<<--モーションの移行を調節したい場合はここを変更)
}

//=================================================
// コンストラクタ
//=================================================
CGameEndPlayer::CGameEndPlayer()
{
}

//=================================================
// デストラクタ
//=================================================
CGameEndPlayer::~CGameEndPlayer()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT CGameEndPlayer::Init(void)
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
	pMotion->SetMotion(CPlayer::MOTION_HAND, true, 10);
	
	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CGameEndPlayer::Uninit(void)
{
	// 終了処理
	CScenePlayer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CGameEndPlayer::Update(void)
{
	// 定数の名前空間の使用
	using namespace CGameEndPlayer_Constants;

	// キャラクターの取得
	CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	// 取得できなかったら
	if (pMotion == nullptr) return;

	// モーションが終了したら
	if (pMotion->FinishMotion())
	{
		pMotion->SetMotion(CPlayer::MOTION_HAND, true, BLEND_FRAME);
	}
	// 更新処理
	CScenePlayer::Update();
}

//=================================================
// 描画処理
//=================================================
void CGameEndPlayer::Draw(void)
{
	// 描画処理
	CScenePlayer::Draw();
}
