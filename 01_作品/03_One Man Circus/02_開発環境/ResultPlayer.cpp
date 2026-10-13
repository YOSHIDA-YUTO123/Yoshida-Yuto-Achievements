//================================================
//
// リザルトのプレイヤー [ResultPlayer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "ResultPlayer.h"
#include "character3D.h"
#include "player.h"
#include "motion.h"

//*************************************************
// 定数宣言
//*************************************************
namespace ResultPlayer_Constants
{
	constexpr int BLEND_FRAME = 15; // モーションブレンドのフレーム(<<--モーションの移行を調節したい場合はここを変更)
}

//=================================================
// コンストラクタ
//=================================================
CResultPlayer::CResultPlayer()
{
}

//=================================================
// デストラクタ
//=================================================
CResultPlayer::~CResultPlayer()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT CResultPlayer::Init(void)
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
void CResultPlayer::Uninit(void)
{
	// 終了処理
	CScenePlayer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CResultPlayer::Update(void)
{
	// 定数の名前空間の使用
	using namespace ResultPlayer_Constants;

	// キャラクターの取得
	CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	// 取得できなかったら
	if (pMotion == nullptr) return;

	// モーションが終了したら
	if (pMotion->FinishMotion())
	{
		// モーションの種類の取得
		int nCurrentMotionType = pMotion->GetType();

		// お辞儀モーションかどうかを判定する
		const bool bBowMotion = (nCurrentMotionType == CPlayer::MOTION_BOW);

		// (Bow -> Hand) : (Hand -> Bow)
		int nMotionType = bBowMotion ? CPlayer::MOTION_HAND : CPlayer::MOTION_BOW;

		pMotion->SetMotion(nMotionType, true, BLEND_FRAME);
	}
	// 更新処理
	CScenePlayer::Update();
}

//=================================================
// 描画処理
//=================================================
void CResultPlayer::Draw(void)
{
	// 描画処理
	CScenePlayer::Draw();
}
