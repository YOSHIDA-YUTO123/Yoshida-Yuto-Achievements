//**************************************************
// 
// ステートプレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "statePlayer.h"
#include "player.h"
#include "myMath.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "transform.h"
#include "objectX.h"
#include "Audience.h"
#include "sound.h"
#include "CollisionMapObject.h"
#include "transform.h"
#include "easing.h"
#include "ruleBalance.h"
#include "ruleHula.h"
#include "ruleJuggling.h"
#include "ruleSkip.h"
#include "game.h"
#include "funcZoomCamera.h"
#include "functionForrowCamera.h"
#include "particle.h"
#include "balljug.h"
#include "hulaobj.h"
#include "score.h"

using namespace std;

//--------------------------------------------------
// プレイヤーステートなしクラス

//**************************************************
// ステートプレイヤー待機状態更新
//**************************************************
void CStatePlayerNone::Update()
{

}

//--------------------------------------------------
// ステートプレイヤー待機状態

//**************************************************
// ステートプレイヤー待機状態初期化
//**************************************************
void CStatePlayerNewtral::Init(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 取得できないなら処理しない
	if (pPlayer == nullptr) return;

	pPlayer->SetMotion(CPlayer::MOTION_NEUTRAL, 10, true);

	// 待機初期化
	pPlayer->InitNewtral();
}

//**************************************************
// ステートプレイヤー待機状態更新
//**************************************************
void CStatePlayerNewtral::Update(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 取得できないなら処理しない
	if (pPlayer == nullptr) return;

	//pPlayer->SetMotion(CPlayer::MOTION_NEUTRAL, 10, true);

	// ニュートラル更新
	pPlayer->UpdateNewtral();
}

//--------------------------------------------------
// ステートプレイヤー移動状態

//**************************************************
// ステートプレイヤー移動状態更新
//**************************************************
void CStatePlayerMove::Update(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	pPlayer->SetMotion(CPlayer::MOTION_MOVE, 10, true);

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 移動量取得
	D3DXVECTOR3 move = pPlayer->GetMove();

	// カメラの向き取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotaition();

	//// 移動量取得
	//float fMovement = pPlayer->GetMovement();

	// 当たり判定の取得
	CCollisionObject3D* pCollision = pPlayer->GetCollision();

	// 空間情報の取得
	CTransform* pTransform = pCollision->GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	// 移動処理
	if (pPlayer->MoveOperation() == true)
	{

	}
	else
	{
		// 待機状態に変更
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		return;
	}

	// ジャンプ仕様としたら
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		pPlayer->ChangeState(std::make_shared<CStatePlayerJump>());
		return;
	}
}

//--------------------------------------------------
// ステートプレイヤージャンプ状態

//**************************************************
// ステートプレイヤージャンプ状態初期化
//**************************************************
void CStatePlayerJump::Init(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	pPlayer->SetMotion(CPlayer::MOTION_JUMP, 8, true);

	pPlayer->InitJump();
}

//**************************************************
// ステートプレイヤージャンプ状態更新
//**************************************************
void CStatePlayerJump::Update(void)
{
	//// キーボード取得
	//CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	//// ジョイパッド取得
	//CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();


	// 当たり判定の取得
	CCollisionObject3D* pCollision = pPlayer->GetCollision();

	// 空間情報の取得
	CTransform* pTransform = pCollision->GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	// 向き取得
	D3DXVECTOR3 rot = transform.rot;
	
	// 移動量取得
	D3DXVECTOR3 move = pPlayer->GetMove();
	
	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotaition();

	//// 移動量取得
	//float fMovement = pPlayer->GetMovement();

	// 移動処理
	if (pPlayer->MoveOperation() == true)
	{

	}
	
	// 着地している状態であれば
	if (pPlayer->GetIsRanding() == true)
	{
		// 待機状態に変更
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
	}
}

//--------------------------------------------------
// プレイヤーステートドラム缶クラス

//**************************************************
// プレイヤーステートドラム缶更新
//**************************************************
void CStatePlayerDramcan::Init(void)
{
	// 時間
	m_nCntTime = 0;
	m_posDestA = Const::VEC3_NULL;
	m_posDestB = Const::VEC3_NULL;
	m_bRight = false;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 乗っているオブジェクトポーインター取得
	m_pObjectRanding = pPlayer->GetCollObjectRanding();

	if (m_pObjectRanding != nullptr)
	{
		// 位置情報取得
		CTransform::Info info = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo();
		float fRotDest = pPlayer->GetRotationDest().y;

		float fAngleA = info.rot.y + D3DX_PI*0.5f;
		float fAngleB = info.rot.y + D3DX_PI * 0.5f + D3DX_PI;
		fAngleA = CMyMath::NormalizingAngles(fAngleA);
		fAngleB = CMyMath::NormalizingAngles(fAngleB);
		float fDiffSave = D3DX_PI * 0.5f;
		float fDiff;

		// オブジェクト向き参照
		fDiff = fabsf(fAngleA - fRotDest);
		if (fDiff <= fDiffSave)
		{
			fDiffSave = fDiff;
			m_rotMove.y = fAngleA;
			pPlayer->SetRotationDest(D3DXVECTOR3(0.0f, fAngleA,0.0f));
		}

		// オブジェクト向き参照
		fDiff = fabsf(fAngleB - fRotDest);
		if (fDiff <= fDiffSave)
		{
			fDiffSave = fDiff;
			m_rotMove.y = fAngleB;
			pPlayer->SetRotationDest(D3DXVECTOR3(0.0f, fAngleB,0.0f));
		}

		pPlayer->GetRotationDest();

		// 元の位置
		m_posOrigin = info.pos;

		// 目的地設定
		m_posDestA.x = info.pos.x + sinf(m_rotMove.y) * DISTANCEMOVE;
		m_posDestA.y = info.pos.y;
		m_posDestA.z = info.pos.z + cosf(m_rotMove.y) * DISTANCEMOVE;

		// 目的地設定
		m_posDestB.x = info.pos.x;
		m_posDestB.y = info.pos.y;
		m_posDestB.z = info.pos.z;

		// 移動距離
		m_vecMove = m_posDestA - m_posDestB;
	}

	// ルール生成
	CGame::ChangeRule(new CRuleBalance);
}

//**************************************************
// プレイヤーステートドラム缶更新
//**************************************************
void CStatePlayerDramcan::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	if (m_pObjectRanding != nullptr)
	{
		CTransform::Info info = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo();

		// カウンターを進める
		if (m_bRight == true)
		{
			m_nCntTime--;
		}
		else
		{
			m_nCntTime++;
		}

		// 左右切替
		if (m_nCntTime >= (int)TIMEMOVE || m_nCntTime <= 0)
		{
			m_bRight = !m_bRight;
		}

		// せき止める繰り返す
		m_nCntTime = Clamp(m_nCntTime, 0, (int)TIMEMOVE);

		// タイム係数
		float fPlamTime = m_nCntTime / TIMEMOVE;

		// イージングを掛ける
		fPlamTime = SelectEasing(EASING_TYPE_EaseInOutSine, fPlamTime);

		info.pos.x = m_posOrigin.x + sinf(m_rotMove.y) * DISTANCEMOVE * fPlamTime;
		info.pos.z = m_posOrigin.z + cosf(m_rotMove.y) * DISTANCEMOVE * fPlamTime;

		m_pObjectRanding->GetCollision()->GetTransform()->SetInfo(info);

		// 回す
		// 円周を求める
		D3DXVECTOR3 vecPos = info.pos - m_posOrigin;
		float fMovement = D3DXVec3Length(&info.pos);
		float fRadius = info.Size.y * 0.5f;
		float fCircum = 2.0f * D3DX_PI * fRadius;

		// 回転量を求める
		float fRotValue = (fMovement / fCircum) * 2.0f * D3DX_PI;

		// 回転を足す
		D3DXVECTOR3 rot = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo().rot;
		rot.z = fRotValue;
		m_pObjectRanding->GetCollision()->GetTransform()->SetRotation(rot);

		// プレイヤーの位置を上に変える
		D3DXVECTOR3 pos = info.pos;
		pos.y = info.pos.y + info.Size.y * 0.5f;
		pPlayer->SetPosition(pos);
	}

	// ボール状態の更新
	pPlayer->UpdateBall();

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// 位置の取得
		D3DXVECTOR3 pos = pPlayer->GetCollision()->GetTransform()->GetInfo().pos;

		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}

	//pPlayer->UpdateInverse();

	//// プレイヤー取得
	//CPlayer* pPlayer = GetPlayer();

	//pPlayer->UpdateDramcan();

	//pPlayer->UpdateInverse();
}

//**************************************************
// プレイヤーステートドラム缶更新
//**************************************************
void CStatePlayerDramcan::Uninit(void)
{
	
}

//--------------------------------------------------
// プレイヤーステート平均台クラス

//**************************************************
// プレイヤーステートボールクラス初期化
//**************************************************
void CStatePlayerBall::Init(void)
{
	// 時間
	m_nCntTime = 0;
	m_posDestA = Const::VEC3_NULL;
	m_posDestB = Const::VEC3_NULL;
	m_bRight = false;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 乗っているオブジェクトポーインター取得
	m_pObjectRanding = pPlayer->GetCollObjectRanding();

	if (m_pObjectRanding != nullptr)
	{
		// 位置情報取得
		CTransform::Info info = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo();

		// 時間
		m_nCntTime = 0;

		// 元の位置
		m_posOrigin = info.pos;

		// 目的地設定
		m_posDestA.x = info.pos.x + DISTANCEMOVE;
		m_posDestA.y = info.pos.y;
		m_posDestA.z = info.pos.z;

		// 目的地設定
		m_posDestB.x = info.pos.x;
		m_posDestB.y = info.pos.y;
		m_posDestB.z = info.pos.z;

		// 移動距離
		m_vecMove = m_posDestA - m_posDestB;
	}

	// ルール生成
	CGame::ChangeRule(new CRuleBalance);
}

//**************************************************
// プレイヤーステートボールクラス更新
//**************************************************
void CStatePlayerBall::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	if (m_pObjectRanding != nullptr)
	{
		CTransform::Info info = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo();

		// カウンターを進める
		if (m_bRight == true)
		{
			m_nCntTime--;
		}
		else
		{
			m_nCntTime++;
		}

		// 左右切替
		if (m_nCntTime >= (int)TIMEMOVE || m_nCntTime <= 0)
		{
			m_bRight = !m_bRight;
		}

		// せき止める繰り返す
		m_nCntTime = Clamp(m_nCntTime,0,(int)TIMEMOVE);

		// タイム係数
		float fPlamTime = m_nCntTime / TIMEMOVE;

		// イージングを掛ける
		fPlamTime = SelectEasing(EASING_TYPE_EaseInOutSine,fPlamTime);
		info.pos.x = m_posOrigin.x + DISTANCEMOVE * fPlamTime;

		m_pObjectRanding->GetCollision()->GetTransform()->SetInfo(info);

		// 回す
		// 円周を求める
		float fMovement = fabsf(info.pos.x - m_posOrigin.x);
		float fRadius = info.Size.y * 0.5f;
		float fCircum = 2.0f * D3DX_PI * fRadius;

		// 回転量を求める
		float fRotValue = (fMovement / fCircum) * 2.0f * D3DX_PI;

		// 回転を足す
		D3DXVECTOR3 rot = m_pObjectRanding->GetCollision()->GetTransform()->GetInfo().rot;
		rot.z = -fRotValue;
		m_pObjectRanding->GetCollision()->GetTransform()->SetRotation(rot);

		// プレイヤーの位置を上に変える
		D3DXVECTOR3 pos = info.pos;
		pos.y = info.pos.y + info.Size.y * 0.5f;
		pPlayer->SetPosition(pos);
	}

	// ボール状態の更新
	pPlayer->UpdateBall();

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// スコア加算
		CGame::GetScore()->AddScore(50000);

		// 位置の取得
		D3DXVECTOR3 pos = pPlayer->GetCollision()->GetTransform()->GetInfo().pos;

		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}

	//pPlayer->UpdateInverse();
}

//--------------------------------------------------
// プレイヤーステート平均台クラス

//**************************************************
// プレイヤーステート平均台クラス初期化
//**************************************************
void CStatePlayerBalanceBeam::Init(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 向きと移動量の設定
	pPlayer->SetToRandingFixd();

	// 固定用向き設定
	m_rotFixed = pPlayer->GetRotationDest();

	// ジャグリングオブジェ生成
	m_pJugObj = CBallJug::Create();
	m_pJugObj->Setplayer(GetPlayer());

	// ルール生成
	CGame::ChangeRule(new CRuleJug);
}

//**************************************************
// プレイヤーステート平均台クラス更新
//**************************************************
void CStatePlayerBalanceBeam::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	pPlayer->SetMotion(CPlayer::MOTION_MOVE_JAG, 8, true);

	// 向きを設定
	pPlayer->SetRotation(m_rotFixed);

	// 平均台処理更新
	pPlayer->UpdateBalanceBeam();

	// 遅くなったら失敗
	if (m_pJugObj->GetIsLoss() == true)
	{
		// 立て直し移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerBalanceRebuild>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// 位置の取得
		D3DXVECTOR3 pos = pPlayer->GetCollision()->GetTransform()->GetInfo().pos;

		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		m_pJugObj->SetIsFall(true);
		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//**************************************************
// プレイヤーステート平均台クラス更新
//**************************************************
void CStatePlayerBalanceBeam::Uninit()
{
	m_pJugObj->SetIsFall(true);
}

//--------------------------------------------------
// プレイヤーステート移動床クラス

//**************************************************
// プレイヤーステート移動床クラス更新
//**************************************************
void CStatePlayerMovingFloor::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	pPlayer->SetMotion(CPlayer::MOTION_HURA, 8, true);
	
	// 移動床処理更新
	pPlayer->UpdateMovingFloor();

	// 位置の取得
	D3DXVECTOR3 pos = pPlayer->GetCollision()->GetTransform()->GetInfo().pos;

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// スコア加算
		CGame::GetScore()->AddScore(100000);

		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//--------------------------------------------------
// プレイヤーステート綱クラス

//**************************************************
// プレイヤーステート綱クラス更新
//**************************************************
void CStatePlayerRope::Init()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 向きと移動量の設定
	pPlayer->SetToRandingFixd();

	// 固定用向き設定
	m_rotFixed = pPlayer->GetRotationDest();

	// 摩擦ゼロ
	pPlayer->SetFriction(0.0f);

	// ルール生成
	CGame::ChangeRule(new CRuleBalance);
}

//**************************************************
// プレイヤーステート綱クラス更新
//**************************************************
void CStatePlayerRope::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	if (pPlayer == nullptr) return;

	pPlayer->SetMotion(CPlayer::MOTION_MOVEBARANSE, 8, true);

	// 固定向き設定
	pPlayer->SetRotationDest(m_rotFixed);

	// 平均台処理更新
	pPlayer->UpdateBalanceRope();

	// 位置の取得
	D3DXVECTOR3 pos = pPlayer->GetCollision()->GetTransform()->GetInfo().pos;

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//**************************************************
// プレイヤーステート綱クラス破棄
//**************************************************
void CStatePlayerRope::Uninit()
{
}

//--------------------------------------------------
// プレイヤーステートジャグリングクラス

//**************************************************
// プレイヤーステートジャグリングクラスコンストラクタ
//**************************************************
CStatePlayerJuggling::CStatePlayerJuggling() : CStateBasePlayer(ID_JUGGLING)
{
	m_fAngle = 0.0f;
}

//**************************************************
// プレイヤーステートジャグリングクラス初期化
//**************************************************
void CStatePlayerJuggling::Init()
{
	// ジャグリングオブジェ生成
	m_pJugObj = CBallJug::Create();
	m_pJugObj->Setplayer(GetPlayer());
}

//**************************************************
// プレイヤーステートジャグリングクラス更新
//**************************************************
void CStatePlayerJuggling::Update()
{
	CPlayer* pPlayer = GetPlayer();
	pPlayer->SetMotion(CPlayer::MOTION_MOVE_JAG, 8, true);
}

//**************************************************
// プレイヤーステートジャグリングクラス終了
//**************************************************
void CStatePlayerJuggling::Uninit()
{
	m_pJugObj->SetIsFall(true);
}

//--------------------------------------------------
// プレイヤーステート縄跳びクラス

//**************************************************
// プレイヤーステート縄跳びクラス更新
//**************************************************
void CStatePlayerSkippingRope::Update(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 縄跳び処理
	pPlayer->UpdateSkippingRope();
}

//--------------------------------------------------
// プレイヤーステートフラフープクラス

//**************************************************
// プレイヤーステートフラフープクラス初期化
//**************************************************
void CStatePlayerHulaHoop::Init(void)
{
	// フラフープ生成
	m_pHulaHoop = CObjectX::Create(Const::VEC3_NULL, "Model()\\hulahoop.x", Const::VEC3_NULL);
	m_fAngleMove += D3DX_PI * 0.1f;

	// ルール生成
	CGame::ChangeRule(new CRuleHula);
}

//**************************************************
// プレイヤーステートフラフープクラス更新
//**************************************************
void CStatePlayerHulaHoop::Update(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// スティック取得
	XINPUT_STATE* pInputState = pInputJoypad->GetJoyStickAngle();

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	pPlayer->SetMotion(CPlayer::MOTION_HURA, 8, true);

	float fAngleInput = 0.0f;

	//// 入力したか否か
	//bool bInput = false;

	float fPalmMove = 0.0f;

	if (pInputKeyBoard->GetPress(DIK_UP) == true)
	{
		fAngleInput = 0.0f;
		fPalmMove = PALAM_ANGLE_ARROW;
	}
	else if (pInputKeyBoard->GetPress(DIK_DOWN) == true)
	{
		fAngleInput = D3DX_PI;
		fPalmMove = PALAM_ANGLE_ARROW;
	}
	else if (pInputKeyBoard->GetPress(DIK_LEFT) == true)
	{
		fAngleInput = -D3DX_PI * 0.5f;
		fPalmMove = PALAM_ANGLE_ARROW;
	}
	else if (pInputKeyBoard->GetPress(DIK_RIGHT) == true)
	{
		fAngleInput = D3DX_PI * 0.5f;
		fPalmMove = PALAM_ANGLE_ARROW;
	}
	else if (pInputJoypad->GetJoyStickR() == true)
	{
		float fAngleX = (float)pInputState->Gamepad.sThumbRX;
		float fAngleY = (float)pInputState->Gamepad.sThumbRY;
		fAngleInput = atan2f(fAngleX, fAngleY);
		fPalmMove = PALAM_ANGLE_STICK;
	}

	// プレイヤーマトリックス取得
	D3DXMATRIX mtxPlayer = pPlayer->GetMatrix();

	// 位置
	D3DXVECTOR3 pos = Const::VEC3_NULL;

	// 入力情報の古い情報更新
	m_fAngleOld = m_fAngleAll;

	// 移動量設定
	float fDiffAngle = CMyMath::ShortestAngle(fAngleInput, m_fAngleInputOld);
	m_fAngleMove += fDiffAngle * fPalmMove;
	
	// 移動量追加
	m_fAngleAll += m_fAngleMove;

	// 同僚に完成
	m_fAngleMove -= m_fAngleMove * 0.01f;

	// 角度によって位置を決める
	pos.x = sinf(m_fAngleAll) * RADIUS;
	pos.y = 25.0f;
	pos.z = cosf(m_fAngleAll) * RADIUS;

	// プレイヤーのマトリックスを掛ける
	pos = CMyMath::MulWorldMatrix(pos, mtxPlayer);

	// フラフープの位置設定
	m_pHulaHoop->SetPosition(pos);

	// 角度を設定
	D3DXVECTOR3 rot = pPlayer->GetRotationBarance();
	rot.y += D3DX_PI;
	m_pHulaHoop->SetRotation(rot);

	// 古い位置更新
	m_fAngleInputOld = fAngleInput;

	// 止まりそう
	if (fabsf(m_fAngleOld - m_fAngleAll) < D3DX_PI * 0.015f)
	{
		// 立て直しに移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerBalanceNone>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceRebuild>());
		return;
	}

	// フラフープ処理
	pPlayer->UpdateHulahupe();

	// 成功していれば
	if (CGame::GetRuleResult() == CRuleBase::RESULT_LIMIT)
	{
		// パーティクルの生成
		CParticle* pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(255, 215, 0, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		pParticle = CParticle::Create(pos, D3DCOLOR_RGBA(0, 255, 255, 255), 3.0f, 50, CParticle::TYPE_NORMAL);
		pParticle->SetParam({ 3.0f,7.0f,3.0f }, 1, 100, 157, -157);
		pParticle->SetParam(0.0f, -0.2f);

		// ニュートラルへ移行
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//**************************************************
// プレイヤーステートフラフープクラス終了
//**************************************************
void CStatePlayerHulaHoop::Uninit(void)
{
	// ルール削除
	CGame::DestoroyRule();

	// フラフープ削除
	if (m_pHulaHoop != nullptr)
	{
		CHula::Create(m_pHulaHoop->GetPosition(), m_pHulaHoop->GetRotation());
		m_pHulaHoop->Uninit();
		m_pHulaHoop = nullptr;
	}
}


//--------------------------------------------------
// プレイヤーステートジャンプバランスクラス

//**************************************************
// プレイヤーステートジャンプバランス状態初期化
//**************************************************
void CStatePlayerJumpBarance::Init()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 移動量取得
	D3DXVECTOR3 move = pPlayer->GetMove();

	// ジャンプ量取得
	float fJumpment = pPlayer->GetJumpment();

	// ジャンプ量追加
	move.y += fJumpment;

	// 情報設定
	pPlayer->SetMove(move);
	pPlayer->SetIsRanding(false);
}

//**************************************************
// プレイヤーステートジャンプバランス状態初期化
//**************************************************
void CStatePlayerJumpBarance::Update()
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 着地している状態であれば
	if (pPlayer->GetIsRanding() == true)
	{
		// 待機状態に変更
		pPlayer->ChangeState(std::make_shared<CStatePlayerNone>());
	}
}

//--------------------------------------------------
// プレイヤーステートバランス待機クラス

//**************************************************
// ステートプレイヤーバランス待機状態初期化
//**************************************************
void CStatePlayerBalanceNone::Update()
{

}

//--------------------------------------------------
// ステートプレイヤーバランス状態

//**************************************************
// ステートプレイヤーバランス状態初期化
//**************************************************
void CStatePlayerBalance::Init(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// バランス初期化
	pPlayer->InitBalance();

	CGame::DeleteFuncTion<CFuncForrowCamera>();
	CGame::AddFuncTion(make_unique<CFuncZoomwCamera>());

}

//**************************************************
// ステートプレイヤーバランス状態更新
//**************************************************
void CStatePlayerBalance::Update(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	
	// バランス処理更新
	pPlayer->UpdateBalance();
}

//**************************************************
// ステートプレイヤーバランス状態終了
//**************************************************
void CStatePlayerBalance::Uninit(void)
{	
	GetPlayer()->UninitBalanse();

	// ルールの破棄
	CGame::DestoroyRule();

	// カメラの再設定
	//CManager::GetCamera()->SetZoom(350.0f, 45);

	CGame::DeleteFuncTion<CFuncZoomwCamera>();
	CGame::AddFuncTion(make_unique<CFuncForrowCamera>());
}

//--------------------------------------------------
// プレイヤーステートバランス立て直しクラス

//**************************************************
// プレイヤーステートバランス立て直し状態初期化
//**************************************************
void CStatePlayerBalanceRebuild::Init(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 立て直し初期化処理
	pPlayer->InitRebuild();


	// 音の取得
	CSound* pSound = CManager::GetSound();

	if (pSound != nullptr)
	{
		//音の再生
		pSound->Play(CSound::SOUND_LABEL_SE_MISS);
	}
}

//**************************************************
// プレイヤーステートバランス立て直し状態更新
//**************************************************
void CStatePlayerBalanceRebuild::Update(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 処理が終ったら待機状態へ移行
	if (pPlayer->UpdateRebuild() == false)
	{
		// バランス状態に変更
		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());

		CAudience::SetMiss(false);
	}
	else
	{
		CAudience::SetMiss(true);
	}
}

//**************************************************
// プレイヤーステートバランス立て直し状態終了
//**************************************************
void CStatePlayerBalanceRebuild::Uninit(void)
{

}