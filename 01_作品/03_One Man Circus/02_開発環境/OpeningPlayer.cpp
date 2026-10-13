//===================================================
//
// オープニングのプレイヤークラス [OpeningPlayer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "OpeningPlayer.h"
#include "light.h"
#include "manager.h"
#include "character3D.h"
#include "math.h"
#include "motion.h"
#include "player.h"
#include "game.h"
#include "MeshCircle.h"
#include "particle.h"
#include "sound.h"
#include "OpeningCamera.h"

//***************************************************
// 定数宣言
//***************************************************
namespace OpeningPlayer_Constants
{
	const D3DXVECTOR3 MOVE_DIR			= { 1.0f,0.0f,0.0f };	// 移動方向
	constexpr float JUMP_HEIGHT			= 7.0f;					// ジャンプの高さ
	constexpr float MOVE_SPEED			= 1.3f;					// 移動速度
	constexpr int LAND_AFTER_HAND_TIME	= 120;					// 着地した後の手をふるモーションの時間
	constexpr int MOVE_TIME				= 120;					// 移動時間
	constexpr int JUGGLING_TIME			= 60;					// ジャグリング時間
	constexpr int BALL_TO_MOVE_TIME		= 20;					// ボールまでの移動時間
	constexpr int JUMP_TIME				= 45;					// ジャンプ時間
	constexpr int WAVE_HAND_TIME		= 180;					// 手を振る時間
	constexpr int BOW_TIME				= 120;					// お辞儀時間
	constexpr int BALANCE_TIME			= 240;					// お辞儀時間
}

//===================================================
// コンストラクタ
//===================================================
COpeningPlayer::COpeningPlayer() : 
	m_move(Const::VEC3_NULL),
	m_nMoveCount(NULL),
	m_nFlag(MOVIE_FLAG_START),
	m_nWaveHandCount(NULL),
	m_bJump(false),
	m_nJumpCount(NULL),
	m_fGravity(NULL),
	m_nJugTime(NULL),
	m_nLightID(-1)
{
}

//===================================================
// デストラクタ
//===================================================
COpeningPlayer::~COpeningPlayer()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT COpeningPlayer::Init(void)
{
	// 初期化処理
	if (FAILED(CScenePlayer::Init()))
	{
		return E_FAIL;
	}

	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// キャラクターの取得
	CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

	// 取得できないなら処理しない
	if (pCharacter == nullptr) return E_FAIL;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	// 取得できないなら処理しない
	if (pMotion == nullptr) return E_FAIL;

	D3DXVECTOR3 dir = { 1.0f,0.0f,0.0f };

	m_move = (dir * 1.3f);

	pMotion->SetMotion(CPlayer::MOTION_MOVE, true, 10);

	m_fGravity = -Const::MAX_GRABITY * 0.25f;

	// ライトの取得
	CLight* pLight = CManager::GetLight();

	m_nLightID = pLight->SetPoint(Const::VEC3_NULL, 350.0f, D3DCOLOR_RGBA(240, 128, 128, 255), Const::WHITE);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void COpeningPlayer::Uninit(void)
{
	// 終了処理
	CScenePlayer::Uninit();
}

//===================================================
// 更新処理
//===================================================
void COpeningPlayer::Update(void)
{
	// キャラクターの取得
	CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

	// 取得できないなら処理しない
	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	// 取得できないなら処理しない
	if (pMotion == nullptr) return;

	// ライトの更新処理
	UpdateLight(pCharacter);

	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// 位置の取得
	D3DXVECTOR3 pos = pCharacter->GetPosition();

	// 重力を加算
	m_move.y += m_fGravity;

	// 移動量の更新
	pos += m_move;

	// 移動のスモーク
	MoveSmoke(pMotion, pCharacter);

	// 着地したときの処理
	Landing(&pos);

	// フラグがスタート時の処理
	FlagStart(pMotion);

	// 最初の移動が終わったあとの処理
	FlagMoveEnd(pMotion);

	// 手を振るモーション終わりったときの処理
	FlagWaveHandEnd(pMotion);

	// ボールへ向かって歩く処理が終わったときの処理
	FlagBallToMoveEnd(pMotion);

	// 手を振る終わり時の処理
	FlagBowEnd(pMotion);

	// 着地のときの処理
	FlagLanding();

	// 傾く処理
	FlagBalance();


	// 位置の設定
	pCharacter->SetPosition(pos);

	// 更新処理
	CScenePlayer::Update();
}

//===================================================
// 描画処理
//===================================================
void COpeningPlayer::Draw(void)
{
	// 描画処理
	CScenePlayer::Draw();
}

//===================================================
// ライトの更新処理
//===================================================
void COpeningPlayer::UpdateLight(CCharacter3D *pCharacter)
{
	// ライトの取得
	auto& light = CManager::GetLight()->GetInfo(m_nLightID);

	D3DXVECTOR3 pos = pCharacter->GetPosition();

	// ポイントライトだったら
	if (light.bUse && light.aLight.Type == D3DLIGHT_POINT)
	{
		light.aLight.Position = { pos.x,pos.y + 200.0f,pos.z };
	}
	
}

//===================================================
// 着地した
//===================================================
void COpeningPlayer::Landing(D3DXVECTOR3* pPos)
{
	// 地面より下に行ったら
	if (pPos->y <= 0.0f)
	{
		m_bJump = true;

		pPos->y = 0.0f;
	}
	else
	{
		m_bJump = false;
	}
}

//===================================================
// フラグがスタートのときの処理
//===================================================
void COpeningPlayer::FlagStart(CMotion *pMotion)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// 開始時の移動が終わったら
	if (m_nMoveCount >= MOVE_TIME && m_nFlag & MOVIE_FLAG_START)
	{
		// ムービー終了フラグをオン
		m_nFlag = MOVIE_FLAG_MOVE_END;

		pMotion->SetMotion(CPlayer::MOTION_HAND, true, 10);

		m_move = Const::VEC3_NULL;
	}
	m_nMoveCount++;
}

//===================================================
// フラグが移動終わりのときの処理
//===================================================
void COpeningPlayer::FlagMoveEnd(CMotion* pMotion)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// 開始時の移動が終わっていたら
	if (m_nFlag & MOVIE_FLAG_MOVE_END)
	{
		// 手を振る時間が終わったら
		if (m_nWaveHandCount >= WAVE_HAND_TIME)
		{
			m_nFlag = MOVIE_FLAG_WAVE_HAND_END;

			pMotion->SetMotion(CPlayer::MOTION_BOW, true, 10);
		}

		m_nWaveHandCount++;
	}
}

//===================================================
// フラグが手を振る終わりのときの処理
//===================================================
void COpeningPlayer::FlagWaveHandEnd(CMotion* pMotion)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// 手を振る時間が終わっていたら
	if (m_nFlag & MOVIE_FLAG_WAVE_HAND_END)
	{
		m_nBowCount++;

		if (m_nBowCount >= BOW_TIME)
		{
			m_move = (MOVE_DIR * MOVE_SPEED);

			m_nFlag = MOVIE_FLAG_BOW_END;
			pMotion->SetMotion(CPlayer::MOTION_MOVE, true, 10);

			m_nMoveCount = 0;
		}
	}
}

//===================================================
// フラグがお辞儀終わりのときの処理
//===================================================
void COpeningPlayer::FlagBowEnd(CMotion* pMotion)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// お辞儀の時間が終わっていたら
	if (m_nFlag & MOVIE_FLAG_BOW_END && m_nMoveCount >= BALL_TO_MOVE_TIME)
	{
		if (m_bJump)
		{
			m_bJump = false;
			m_move.y = JUMP_HEIGHT;
		}

		m_nFlag = MOVIE_FLAG_BALL_TO_MOVE_END;
		pMotion->SetMotion(CPlayer::MOTION_JUMP, true, 10);
	}
}

//===================================================
// フラグがボールに向かって歩くのが終わったのときの処理
//===================================================
void COpeningPlayer::FlagBallToMoveEnd(CMotion* pMotion)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// ボールへ向かって歩く処理が終わっていたら
	if (m_nFlag & MOVIE_FLAG_BALL_TO_MOVE_END)
	{
		m_nJumpCount++;

		if (m_nJumpCount >= JUMP_TIME)
		{
			pMotion->SetMotion(CPlayer::MOTION_MOVE_JAG, true, 10);

			// イベントを発火
			BeginEvent();

			m_nFlag = MOVIE_FLAG_LANDING;
			m_move.y = 0.0f;
			m_fGravity = 0.0f;
		}
	}
}

//===================================================
// フラグが着地のときの処理
//===================================================
void COpeningPlayer::FlagLanding(void)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	// 着地していたら
	if (m_nFlag & MOVIE_FLAG_LANDING)
	{
		m_nJugTime++;

		if (m_nJugTime >= JUGGLING_TIME)
		{
			m_nFlag = MOVIE_FLAG_BALANCE;
			m_nCnt = 0;

			// 観客を常に沸かせる
			CAudience::SetSuccess(true);
		}
	}
}

//===================================================
// 傾くときの処理
//===================================================
void COpeningPlayer::FlagBalance(void)
{
	// 名前空間の使用
	using namespace OpeningPlayer_Constants;

	if (m_nFlag & MOVIE_FLAG_BALANCE)
	{
		// キャラクターの取得
		CCharacter3D* pCharacter = CScenePlayer::GetCharacter();

		float fDest = D3DX_PI * 0.3f;
		D3DXVECTOR3 rot = pCharacter->GetRotation();
		rot.x += (fDest - rot.x) * 0.035f;
		pCharacter->SetRotation(rot);

		m_nCnt++;

		// 観客を常に沸かせる
		CAudience::SetSuccess(true);

		if (m_nCnt >= BALANCE_TIME)
		{
			// シーンを切り替える
			CManager::SetFade(std::make_unique<CGame>());
		}
	}
}

//===================================================
// 移動のスモークの発生
//===================================================
void COpeningPlayer::MoveSmoke(CMotion* pMotion, CCharacter3D* pCharacter)
{
	D3DXVECTOR3 pos = pCharacter->GetPosition();

	// フレームないだったら
	if (pMotion->IsEventFrame(15, 15, CPlayer::MOTION_MOVE) || pMotion->IsEventFrame(25, 25, CPlayer::MOTION_MOVE))
	{
		CParticle* pParticle = CParticle::Create(pos, Const::WHITE, 13.0f, 2, CParticle::TYPE_SMOKE);
		pParticle->SetParam({ 1.0f,0.5f,1.0f }, 1, 60, 157, -157);
		pParticle->SetAnim(4, 3, 10, true);
	}
}