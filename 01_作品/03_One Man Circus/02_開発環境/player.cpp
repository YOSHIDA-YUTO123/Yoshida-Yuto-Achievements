//**************************************************
// 
// プレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "player.h"
#include "character3D.h"
#include "objectX.h"
#include "camera.h"
#include "statePlayer.h"
#include "motion.h"
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "input.h"
#include "RotationFloor.h"
#include "game.h"
#include "transform.h"
#include "myMath.h"
#include "Collider.h"
#include "CollisionMapObjManager.h"
#include "Collision.h"
#include "CollisionMapObject.h"
#include "modelManager.h"
#include "light.h"
#include "particle.h"
#include "score.h"
#include "Audience.h"

//**************************************************
// 定数宣言
//**************************************************
namespace Player_Constants
{
	constexpr float LIGHT_HEIGHT = 100.0f; // ライトの高さ
}

//**************************************************
// プレイヤーデストラクタ
//**************************************************
CPlayer::CPlayer(const int nPriority) : CObject(nPriority)
{
	m_bRotationFloorLanding = false;				// 回転床に着地しているかどうか
	m_pTransformRef = nullptr;						// プレイヤー情報
	m_pCharacter3D = nullptr;						// キャラクター
	m_pStateMachine = nullptr;						// ステートマシーン
	m_pStateMachineBarance = nullptr;				// ステートマシーンバランス
	m_axisBarance = D3DXVECTOR3(0.0f,1.0f,0.0f);	// バランス軸
	m_rotVaranse = Const::VEC3_NULL;				// バランス角度
	m_rotVaranseOld = Const::VEC3_NULL;				// 古いバランス角度
	m_rotAddVaranse = Const::VEC3_NULL;				// 足すバランス角度
	m_move = Const::VEC3_NULL;						// 移動量
	D3DXMatrixIdentity(&m_mtxLandingRotFloor);		// 回る床に乗ったときのマトリックス
	m_pMtxRanding = nullptr;						// 乗っているもののマトリックスポインター
	m_fMovement = 0.0f;								// 移動量取得
	m_fFriction = 0.0f;								// 摩擦係数取得
	m_fGravity = 0.0f;								// 重力取得
	m_fLowLimit = 0.0f;								// 限界低度取得
	m_fXLimit = 0.0f;								// 限界X値
	m_fZLimit = 0.0f;								// 限界Y値
	m_fInertia = 0.0f;								// 慣性
	m_bRanding = true;								// 乗っているか否か
	m_nModelIdx = -1;								// モデルのID
	m_nCntScore = 0;								// スコアカウンター
	m_nCntMax = 0;									// スコアの最大カウンター
	m_nScore = 0;									// スコア
	m_nAddScore = 10000;							// 足すスコア
	m_nRebuildTime = -1;							// 立て直し時間
	m_nCntRebuild = 0;								// 立て直しカウンター
	m_pCollision = nullptr;							// 当たり判定用クラス
	m_bMove = false;	
	m_nRuluInfo = -1;								// ルールの戻り値
	m_bGravity = true;								// 重力をつけるかどうか
	m_pCollObjectRanding = nullptr;					// 乗っているオブジェクトポインタ
	m_fMovementBarance = 0.0f;						// バランス時移動量
	m_aLandingMapObjName.clear();					// 着地しているオブジェクトのパス
	m_nLightID = -1;
}

//**************************************************
// プレイヤーコンストラクタ
//**************************************************
CPlayer::~CPlayer()
{

}

//**************************************************
// プレイヤー初期化
//**************************************************
HRESULT CPlayer::Init(void)
{
	// ライトの取得
	CLight* pLight = CManager::GetLight();

	// ライトのIDの取得
	m_nLightID = pLight->SetPoint(Const::VEC3_NULL, 130.0f, D3DCOLOR_RGBA(240, 128, 128, 255), Const::WHITE);

	// キャラクターの生成
	if (m_pCharacter3D == nullptr)
	{
		m_pCharacter3D = CCharacter3D::Create("motion.txt", Const::VEC3_NULL, Const::VEC3_NULL, MOTION_MAX);
		m_pCharacter3D->SetCharacter(1,m_fMovement,D3DXVECTOR3(1.0f, 0.2f, 1.0f),Const::VEC3_NULL);
	}

	if (m_pCollision == nullptr)
	{
		m_pCollision = CCollisionObject3D::Create();
	}

	// 空間情報の参照
	m_pTransformRef = m_pCollision->GetTransform();

	// ステートマシン生成
	if (m_pStateMachine == nullptr)
	{
		m_pStateMachine = std::make_unique< CStateMachine>();
	}

	// ステートマシン生成
	if (m_pStateMachineBarance == nullptr)
	{
		m_pStateMachineBarance = std::make_unique< CStateMachine>();
	}

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// モデルのIDの取得
	m_nModelIdx = pModelManager->Register("data\\MODEL\\player\\testPlayer_000.x");

	CTransform::Info transform = m_pTransformRef->GetInfo();

	// 大きさの設定
	float fSize = 15.0f;
	transform.Size = { fSize,50.0f,fSize };

	// 情報の設定
	m_pTransformRef->SetInfo(transform);

	m_pCollision->AddCollider(CColliderAABB::Create({ 0.0f,transform.Size.y * 0.5f,0.0f }));

	// 状態を待機状態に変更
	ChangeState(std::make_shared<CStatePlayerNewtral>());
	ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());

	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//**************************************************
// プレイヤー終了
//**************************************************
void CPlayer::Uninit(void)
{
	m_pTransformRef = nullptr;

	// キャラクターの破棄
	if (m_pCharacter3D != nullptr)
	{
		m_pCharacter3D->Uninit();
		m_pCharacter3D.reset();
		m_pCharacter3D = nullptr;
	}

	if (m_pCollision != nullptr)
	{
		m_pCollision->Uninit();
		m_pCollision.reset();
	}

	// 終了処理
	CObject::Release();
}

//**************************************************
// プレイヤー更新
//**************************************************
void CPlayer::Update(void)
{
	// ステートマシン更新
	if (m_pStateMachine != nullptr)
	{
		m_pStateMachine->Update();
	}

	// ステートマシン更新
	if (m_pStateMachineBarance != nullptr)
	{
		m_pStateMachineBarance->Update();
	}

	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// ｘ慣性
	m_fInertia += transform.pos.x - transform.posOld.x;
	m_fInertia -= m_fInertia * 0.3f; // 乗った時の勢いの処理

	CDebugProc::Print("m_fInertia : %f\n" , m_fInertia);

	// 古い位置更新
	transform.posOld = transform.pos;


	// 移動量に慣性をつける
	m_move.x += (-m_move.x * m_fFriction);
	m_move.z += (-m_move.z * m_fFriction);
	
	// 移動量を追加
	transform.pos += m_move;

	// ライトの更新処理
	UpdateLight(&transform.pos);

	SetMotionEffect();

	// 重力をつけるなら
	if (m_bGravity)
	{
		// 重力を加える
		m_move.y += m_fGravity;
	}

	// 下限値まで落ちたらそこで止める
	if (transform.pos.y <= m_fLowLimit)
	{
		if (m_bRanding == false && m_pCharacter3D->GetMotion()->GetType() == MOTION_JUMP)
		{
			m_pCharacter3D->GetMotion()->SetMotion(MOTION_LANDING, true, 8);
		}

		// 高さを限界地に設定
		transform.pos.y = m_fLowLimit;

		// 縦方向の移動量をリセット
		m_move.y = 0.0f;

		// 着地状態にする
		m_bRanding = true;
	}

	// Xの限界地に行ったら
	if (transform.pos.x >= m_fXLimit)
	{
		transform.pos.x = m_fXLimit;
	}
	else if (transform.pos.x <= -m_fXLimit / 3.0f)
	{
		transform.pos.x = -m_fXLimit / 3.0f;
	}

	// Zの限界地に行ったら
	if (transform.pos.z >= m_fZLimit)
	{
		transform.pos.z = m_fZLimit;
	}
	else if (transform.pos.z <= -m_fZLimit)
	{
		transform.pos.z = -m_fZLimit;
	}

	// キャラクターの生成
	if (m_pCharacter3D != nullptr)
	{
		m_pCharacter3D->SetPosition(transform.pos);
	}

	// 設定
	m_pTransformRef->SetInfo(transform);

#if 1
	// Xファイルパス
	std::string XFilePath;
	std::string XFilePathConfi;

	// 当たっているオブジェクト無し
	m_pCollObjectRanding = nullptr;

	// 当たっているか否か
	bool bCollision = false;

	// 当たっている面情報
	int face = -1;

	// マップオブジェクトとの判定をする前にリセットしておく
	m_aLandingMapObjName.clear();

	// 当たり判定
	for (auto& colliders : m_pCollision->GetCollider())
	{
		bool bGravity = true;
		D3DXVECTOR3 push = transform.pos;
		CCollisionMapObject* pDummy = nullptr;

		if (CGame::GetMapObjectManager()->Collision(&pDummy,XFilePathConfi,colliders, &push, &face, &bGravity))
		{
			// ファイルパス移し替え
			XFilePath = XFilePathConfi;

			transform.pos.x = push.x;

			// 当たっている判定
			bCollision = true;
		}
		else
		{
			m_bGravity = true;
		}

		push = transform.pos;

		// 設定
		m_pTransformRef->SetInfo(transform);

		if (CGame::GetMapObjectManager()->Collision(&pDummy, XFilePathConfi, colliders, &push, &face, &bGravity))
		{
			m_pCollObjectRanding = pDummy;

			// ファイルパス移し替え
			XFilePath = XFilePathConfi;

			transform.pos.y = push.y;

			if (face == CCollisionAABB::FACE_TOP || face == CCollisionAABB::FACE_BUTTOM)
			{
				m_move.y = Const::VEC3_NULL.y;
			}

			// 当たっている判定
			bCollision = true;

			if (!bGravity)
			{
				m_bGravity = false;
				m_bRanding = true;
			}
		}
		else
		{
			m_bGravity = true;
		}
		push = transform.pos;

		// 設定
		m_pTransformRef->SetInfo(transform);

		if (CGame::GetMapObjectManager()->Collision(&pDummy, XFilePathConfi, colliders, &push, &face, &bGravity))
		{
			// ファイルパス移し替え
			XFilePath = XFilePathConfi;

			transform.pos.z = push.z;

			// 当たっている判定
			bCollision = true;
		}
		else
		{
			m_bGravity = true;
		}

		// 設定
		m_pTransformRef->SetInfo(transform);

	}

	// 当たっていたら
	if (bCollision == true)
	{
		if (m_pCollObjectRanding != nullptr)
		{
			if (m_pCharacter3D->GetMotion()->GetType() == MOTION_JUMP )
			{
				m_pCharacter3D->GetMotion()->SetMotion(MOTION_LANDING, true, 8);
			}

			// 天面に当たっていたら
			if (CCollisionAABB::FACE_TOP == face)
			{
				// 状態を変える
				ChangeStateCollision(XFilePath);

				// 乗った状態にする
				m_pCollObjectRanding->SetIsRanding(true);
			}
		}
	}

	if (m_pCharacter3D != nullptr)
	{
		// キャラクターの向きを取得(キャラクターと自分の使ってる情報が違うため取得しないと回転しなかったから)
		transform.rot = m_pCharacter3D->GetRotation();
	}

	// 設定
	m_pTransformRef->SetInfo(transform);

	// キャラクターの生成
	if (m_pCharacter3D != nullptr)
	{
		m_pCharacter3D->Update();


		m_pCharacter3D->UpdateMotion();
	}

	if (m_pCollision != nullptr)
	{
		// マトリックスの計算
		m_pTransformRef->CalcuMatrix();

		// 更新処理
		m_pCollision->Update();
	}

#endif // 0

#ifdef _DEBUG
	CDebugProc::Print("transform.pos.x : %.3f : .y%.3f : .z%.3f\n", transform.pos.x, transform.pos.y, transform.pos.z);
	CDebugProc::Print("移動量 = %.3f : %.3f : %.3f\n", m_move.x, m_move.y, m_move.z);
#endif
}

//**************************************************
// プレイヤー描画
//**************************************************
void CPlayer::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;

	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&transform.mtxWorld);

	if (m_bRotationFloorLanding)
	{		
		// 向きを反映
		D3DXMATRIX mtxRotFloor = Const::MTX_NULL; 
		mtxRotFloor = m_mtxLandingRotFloor;
		mtxRotFloor._41 = 0.0f;
		mtxRotFloor._42 = 0.0f;
		mtxRotFloor._43 = 0.0f;
		D3DXVECTOR3 pos = CMyMath::MulWorldMatrix(m_OnFloorPos, m_mtxLandingRotFloor);

		// フロアの回転を追加
		D3DXMatrixMultiply(&transform.mtxWorld, &transform.mtxWorld, &mtxRotFloor);
		
		// バランスの向き追加
		D3DXMatrixMultiply(&transform.mtxWorld, &transform.mtxWorld, &m_mtxRot);

		// 位置の反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&transform.mtxWorld, &transform.mtxWorld, &mtxTrans);

		// 位置を更新する
		D3DXVec3TransformCoord(&transform.pos, &m_OnFloorPos, &m_mtxLandingRotFloor);
	}
	else
	{
		// 向きを反映
		D3DXMATRIX mtxRotTest;
		D3DXMatrixRotationYawPitchRoll(&mtxRotTest, transform.rot.y + D3DX_PI, transform.rot.x, transform.rot.z);
		D3DXMatrixMultiply(&mtxRot, &mtxRotTest, &m_mtxRot);
		D3DXMatrixMultiply(&transform.mtxWorld, &transform.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, transform.pos.x, transform.pos.y, transform.pos.z);
		D3DXMatrixMultiply(&transform.mtxWorld, &transform.mtxWorld, &mtxTrans);
	}

	// 情報の設定
	m_pTransformRef->SetInfo(transform);

	if (m_pCharacter3D != nullptr)
	{
		// 描画処理
		m_pCharacter3D->Draw(transform.mtxWorld);
	}
}

//***************************************************************
// プレイヤー生成
//***************************************************************
CPlayer* CPlayer::Create(void)
{
	// プレイヤーインスタンス生成
	CPlayer* pPlayer = new CPlayer;

	// 初期化
	if (pPlayer != nullptr)
	{
		pPlayer->Init();
	}

	return pPlayer;
}

//***************************************************************
// 傾きリセット
//***************************************************************
void CPlayer::ResetBarance(void)
{
	m_rotVaranse = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxRot);
}

//***************************************************************
// ローカル位置作成処理
//***************************************************************
void CPlayer::CreatePositionLocal(D3DXMATRIX* pMtx, D3DXVECTOR3 pos)
{
	// 逆行列を作る
	D3DXMATRIX mtxInverse;
	D3DXMatrixInverse(&mtxInverse, nullptr, pMtx);

	// ローカル位置を求め値を設定
	m_posLocal = CMyMath::MulWorldMatrix(pos, mtxInverse);

	// 値を設定
	m_pMtxRanding = pMtx;
}

//***************************************************************
// ローカル位置削除処理
//***************************************************************
void CPlayer::DeletePositionLocal(void)
{
	// 値のリセット
	m_posLocal = Const::VEC3_NULL;
	m_pMtxRanding = nullptr;
}

//***************************************************************
// プレイヤーステート変更処理
//***************************************************************
void CPlayer::ChangeState(std::shared_ptr<CStateBasePlayer> stateBasePlayer)
{
	// 自身のぽいんたーを設定
	stateBasePlayer->SetOwner(this);

	// 状態を変更
	m_pStateMachine->Change(stateBasePlayer);
}

//***************************************************************
// プレイヤーステートバランス変更処理
//***************************************************************
void CPlayer::ChangeStateBalance(std::shared_ptr<CStateBasePlayer> stateBasePlayerBalance)
{
	// 自身のぽいんたーを設定
	stateBasePlayerBalance->SetOwner(this);

	// 状態を変更
	m_pStateMachineBarance->Change(stateBasePlayerBalance);
}

//***************************************************************
// プレイヤーステートファイルパスによって変更
//***************************************************************
void CPlayer::ChangeStateCollision(std::string& XFilepath)
{
	// 立て直し状態であれば処理しない
	if (m_pStateMachineBarance->GetID() == CStateBasePlayer::ID_BALANCE_REBUILD)
	{
		m_aLandingMapObjName.clear();

		return;
	}

	// すでに乗っている場合は処理しない
	if (m_pCollObjectRanding->GetIsRanding() == true)
	{
		return;
	}
	//// ジャグリング
	//if (pInputKeyBoard->GetPress(DIK_1) == true)
	//{
	//	ChangeState(std::make_shared<CStatePlayerJuggling>());
	//	ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	//}
	// 平均台
	if (XFilepath == FILEPATH_BALANCEBEAM)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVE_JAG, true, 8);

		ChangeState(std::make_shared<CStatePlayerBalanceBeam>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// 移動床
	else if (XFilepath == FILEPATH_MOVEFLOOR)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVEBARANSE, true, 8);

		ChangeState(std::make_shared<CStatePlayerMovingFloor>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// ドラム缶
	else if (XFilepath == FILEPATH_DRUMMCAN)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVEBARANSE, true, 15);

		ChangeState(std::make_shared<CStatePlayerDramcan>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// 綱
	else if (XFilepath == FILEPATH_ROPE)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVEBARANSE, true, 8);

		ChangeState(std::make_shared<CStatePlayerRope>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// ぼっこ
	else if (XFilepath == FILEPATH_ROD)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_HURA, true, 8);

		ChangeState(std::make_shared<CStatePlayerHulaHoop>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// フラフープ
	/*else if (pInputKeyBoard->GetPress(DIK_6) == true)
	{
		ChangeState(std::make_shared<CStatePlayerHulaHoop>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}*/
	// 移動床
	else if (XFilepath == FILEPATH_MOVEFLOOR)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_HURA, true, 8);

		ChangeState(std::make_shared<CStatePlayerMovingFloor>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	// ボール
	else if (XFilepath == FILEPATH_BALL)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVEBARANSE, true, 8);

		ChangeState(std::make_shared<CStatePlayerBall>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
	}
	
	// 着地しているオブジェクトのパスの取得
	m_aLandingMapObjName = XFilepath;
}

//***************************************************************
// プレイヤー待機初期化
//***************************************************************
void CPlayer::InitNewtral(void)
{
	// 傾きリセット
	m_rotVaranse.x = 0.0f;
	m_rotAddVaranse = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxRot);
}

//***************************************************************
// プレイヤーバランス初期化処理
//***************************************************************
void CPlayer::InitJump(float fPalamJumpment)
{
	// 乗っていない状態であれば処理しない
	if (m_bRanding == false)
	{
		return;
	}

	// ジャンプ量追加
	m_move.y += cosf(m_rotVaranse.x) * m_fJumpment * fPalamJumpment;
	m_move.x += sinf(m_rotVaranse.x) * m_fJumpment * fPalamJumpment;

	// 傾きリセット
	m_rotVaranse.x = 0.0f;
	m_rotAddVaranse = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxRot);

	// 乗っていないじょうたいにする
	m_bRanding = false;
}

//***************************************************************
// プレイヤーバランス初期化処理
//***************************************************************
void CPlayer::InitJumpSkipingRope(float fPalamJumpment)
{
	// 乗っていない状態であれば処理しない
	if (m_bRanding == false)
	{
		return;
	}

	// ジャンプ量追加
	m_move.y += m_fJumpment * fPalamJumpment;

	// 乗っていないじょうたいにする
	m_bRanding = false;
}

//***************************************************************
// プレイヤーバランス初期化処理
//***************************************************************
void CPlayer::InitBalance(void)
{
	// ちょっと足す
	m_rotVaranse.x += D3DX_PI * 0.001f;
	m_rotVaranse.y = D3DX_PI * -0.5f;

	// 球座標でバランス軸計算
	m_axisBarance = CMyMath::SphericalCoordinates(Const::VEC3_NULL, m_rotVaranse, 1.0f);

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラズームの設定
	//pCamera->SetZoom(150.0f, 45);
}

//***************************************************************
// プレイヤー無し更新処理
//***************************************************************
bool CPlayer::UpdateNone(void)
{
	return true;
}

//***************************************************************
// プレイヤー待機更新
//***************************************************************
bool CPlayer::UpdateNewtral(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// プレイヤーのモーションが移動だったら
	if (m_pCharacter3D->GetMotion()->GetType() == MOTION_MOVE)
	{
		// ニュートラルに戻す
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_NEUTRAL, true, 10);
	}

	// 移動しようとしたら
	if (pInputKeyBoard->GetPress(DIK_W) == true ||
		pInputKeyBoard->GetPress(DIK_S) == true ||
		pInputKeyBoard->GetPress(DIK_A) == true ||
		pInputKeyBoard->GetPress(DIK_D) == true ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_LEFT) ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_RIGHT) ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_UP) ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_DOWN) ||
		pInputJoypad->GetJoyStickL() == true)
	{
		if (m_pCharacter3D->GetMotion()->GetType() != MOTION_JUMP)
		{
			m_pCharacter3D->GetMotion()->SetMotion(MOTION_MOVE, true, 10);
		}
		// 移動状態に変更
		ChangeState(std::make_shared<CStatePlayerMove>());
		return false;
	}
	// ジャンプ仕様としたら
	else if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		m_pCharacter3D->GetMotion()->SetMotion(MOTION_JUMP, true, 5);

		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		return false;
	}

#ifdef _DEBUG
	// ジャグリング
	else if (pInputKeyBoard->GetPress(DIK_1) == true)
	{
		ChangeState(std::make_shared<CStatePlayerJuggling>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// 平均台
	else if (pInputKeyBoard->GetPress(DIK_2) == true)
	{
		ChangeState(std::make_shared<CStatePlayerBalanceBeam>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// 移動床
	else if (pInputKeyBoard->GetPress(DIK_3) == true)
	{
		ChangeState(std::make_shared<CStatePlayerMovingFloor>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// 綱
	else if (pInputKeyBoard->GetPress(DIK_4) == true)
	{
		ChangeState(std::make_shared<CStatePlayerRope>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// 縄跳び
	else if (pInputKeyBoard->GetPress(DIK_5) == true)
	{
		ChangeState(std::make_shared<CStatePlayerSkippingRope>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// フラフープ
	else if (pInputKeyBoard->GetPress(DIK_6) == true)
	{
		ChangeState(std::make_shared<CStatePlayerHulaHoop>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// 移動床
	else if (pInputKeyBoard->GetPress(DIK_7) == true)
	{
		ChangeState(std::make_shared<CStatePlayerMovingFloor>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}
	// ボール
	else if (pInputKeyBoard->GetPress(DIK_8) == true)
	{
		ChangeState(std::make_shared<CStatePlayerBall>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalance>());
		return false;
	}

	CDebugProc::Print("ジャグリング : DIK_1\n");
	CDebugProc::Print("平均台       : DIK_2\n");
	CDebugProc::Print("移動床       : DIK_3\n");
	CDebugProc::Print("綱           : DIK_4\n");
	CDebugProc::Print("縄跳び       : DIK_5\n");
	CDebugProc::Print("フラフープ   : DIK_6\n");
	CDebugProc::Print("移動床       : DIK_7\n");
	CDebugProc::Print("ボール       : DIK_8\n");
#endif // _DEBUG

	//// カメラの取得
	//CCamera* pCamera = CManager::GetCamera();
	//pCamera->Zoom();

	return true;
}

//***************************************************************
// プレイヤーバランス更新処理
//***************************************************************
bool CPlayer::UpdateBalance(void)
{
	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// スティック情報取得
	XINPUT_STATE* pStick = pInputJoypad->GetJoyStickAngle();

	// プレイヤー情報取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// 移動量を求める
	float fMoveXA = transform.posOld.x - transform.pos.x;
	float fMoveXB = m_fInertia;
	float fPalamMove = (fMoveXA / m_fMovement) + (fMoveXB / m_fMovement);
	float fLimit = 3.0f;
	if (fPalamMove >= fLimit)
	{
		fPalamMove = fLimit;
	}
	else if (fPalamMove <= -fLimit)
	{
		fPalamMove = -fLimit;
	}
	float fAddRotX = D3DX_PI * 0.001f * fPalamMove;

	// 右に傾く
	if (pInputKeyBoard->GetPress(DIK_D) == true ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_RIGHT) == true)
	{
		m_rotAddVaranse.x += D3DX_PI * 0.0002f;
		m_rotAddVaranse.x += fabsf(m_rotAddVaranse.x) * 0.05f;
	}
	// 左に傾く
	else if (pInputKeyBoard->GetPress(DIK_A) == true ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_LEFT) == true)
	{
		m_rotAddVaranse.x -= D3DX_PI * 0.0002f;
		m_rotAddVaranse.x -= fabsf(m_rotAddVaranse.x) * 0.05f;
	}
	// スティックで傾く
	else if (pInputJoypad->GetJoyStickLOutDeadZone() == true)
	{
		float fAngleStick = (float)pStick->Gamepad.sThumbLX;
		float fPlaMi = fAngleStick / fabsf(fAngleStick);
		float fPalamAngle = fabsf(fAngleStick) / CInputJoypad::ANGLEMAX;

		m_rotAddVaranse.x += D3DX_PI * 0.0002f * fPlaMi * fPalamAngle;
		m_rotAddVaranse.x += fabsf(m_rotAddVaranse.x * 0.05f) * fPlaMi;
	}

	// 古い角度更新
	m_rotVaranseOld = m_rotVaranse;

	// 角度を足す
	m_rotVaranse.x += m_rotVaranse.x * 0.02f;
	m_rotVaranse.x += fAddRotX;

	// バランスを足す
	m_rotVaranse.x += m_rotAddVaranse.x;

	m_rotAddVaranse.x += (-m_rotAddVaranse.x * 0.03f);

	float fPlam = 0.01f + fabsf(fPalamMove);
	if (m_rotAddVaranse.x > D3DX_PI * fPlam)
	{
		m_rotAddVaranse.x = D3DX_PI * fPlam;
	}
	else if (m_rotAddVaranse.x < -D3DX_PI * fPlam)
	{
		m_rotAddVaranse.x = -D3DX_PI * fPlam;
	}

	// 上限を設ける(90度)
	if (m_rotVaranse.x > D3DX_PI * 0.5f)
	{

		m_rotVaranse.x = D3DX_PI * 0.5f;

		m_rotAddVaranse = Const::VEC3_NULL;
		m_nScore = 0;

		// バランス状態に変更
		ChangeState(std::make_shared<CStatePlayerNone>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceRebuild>());
		return false;
	}
	else if (m_rotVaranse.x < -D3DX_PI * 0.5f)
	{
		m_rotVaranse.x = -D3DX_PI * 0.5f;

		m_rotAddVaranse = Const::VEC3_NULL;
		m_nScore = 0;

		// バランス状態に変更
		ChangeState(std::make_shared<CStatePlayerNone>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceRebuild>());

		return false;
	}

	// クォータニオンで軸の回転マトリックス作成
	D3DXVECTOR3 axisRot = { 0.0f,0.0f,-1.0f };
	D3DXQUATERNION quat;
	D3DXMATRIX mtxRot;
	D3DXQuaternionRotationAxis(&quat, &axisRot, m_rotVaranse.x);
	D3DXMatrixRotationQuaternion(&m_mtxRot, &quat);

	// スコアカウンター追加
	m_nCntScore++;

	// スコアはゼロ
	m_nScore = 0;

	// カウンターが最大値まで行ったら
	if (m_nCntScore >= m_nCntMax)
	{
		// カウンターゼロ
		m_nCntScore = 0;

		// 角度の差を取って
		float fDiffRot = fabsf(m_rotVaranse.x - m_rotVaranseOld.x);

		// 角度を保ち続けてたら
		if (fDiffRot >= D3DX_PI * 0.0f && fDiffRot <= D3DX_PI * 0.002f)
		{
			// スコア加算
			m_nScore = m_nAddScore;

			// ちょっと
			if (fabsf(m_rotVaranse.x) >= D3DX_PI * 0.05f)
			{
				CGame::SetAnimation(CAudience::ANIMETYPE_SUCCESS,1);
			}

			// まあまあ
			if (fabsf(m_rotVaranse.x) >= D3DX_PI * 0.15f)
			{
				CGame::SetAnimation(CAudience::ANIMETYPE_SUCCESS,2);
			}

			// ギリギリ
			if (fabsf(m_rotVaranse.x) >= D3DX_PI * 0.25f)
			{
				CGame::SetAnimation(CAudience::ANIMETYPE_SUCCESS,3);
				m_nScore = m_nAddScore * 2;
			}

			// 失敗
			if (fabsf(m_rotVaranse.x) >= D3DX_PI * 0.5f)
			{
				m_nScore = 0;
			}
		}
		else
		{
			m_nScore = 0;
		}
	}

	CGame::GetScore()->AddScore(m_nScore);

	CCamera* pCamera = CManager::GetCamera();
	if (m_bRanding)
	{
		pCamera->Zoom();
	}

	// 着地していない
	if (m_bRanding == false)
	{
		// バランス無し状態に変更
		ChangeState(std::make_shared<CStatePlayerNewtral>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
	}

	return true;
}

//***************************************************************
// プレイヤー立て直し初期化処理
//***************************************************************
void CPlayer::InitRebuild(void)
{
	m_rotVaranse.x = 0.0f;
	m_mtxRot = Const::MTX_NULL;
	m_pCharacter3D->GetMotion()->SetMotion(MOTION_DOWN_NEWTRAL, true, 5);
}

//***************************************************************
// プレイヤードラム缶初期化処理
//***************************************************************
void CPlayer::InitDramcan(void)
{

}

//***************************************************************
// プレイヤー立て直し更新処理
//***************************************************************
bool CPlayer::UpdateRebuild(void)
{
	m_pCharacter3D->GetMotion()->SetMotion(MOTION_GETUP, true, 90);

	//// タイムが無ければ処理しない
	//if (m_nRebuildTime <= 0)
	//{
	//	return false;
	//}

	//// カウンター加算
	//m_nCntRebuild++;

	//// 時間係数計算
	//float fPlamTime = (float)m_nCntRebuild / (float)m_nRebuildTime;
	//
	//// 角度設定
	////m_rotVaranse.x = m_rotVaranseDiff.x * (1.0f - fPlamTime);
	//m_rotVaranse.x = 0.0f;

	//// クォータニオンで軸の回転マトリックス作成
	//D3DXVECTOR3 axisRot = { 0.0f,0.0f,-1.0f };
	//D3DXQUATERNION quat;
	//D3DXMATRIX mtxRot;
	//D3DXQuaternionRotationAxis(&quat, &axisRot, m_rotVaranse.x);
	//D3DXMatrixRotationQuaternion(&m_mtxRot, &quat);

	//// 時間いっぱいになったら
	//if (m_nCntRebuild >= m_nRebuildTime)
	//{
	//	// カウンターリセット
	//	m_nCntRebuild = 0;
	//	return false;
	//}

	return !m_pCharacter3D->GetMotion()->IsEndMotion();
}

//***************************************************************
// プレイヤーのっかり処理
//***************************************************************
void CPlayer::UpdateInverse(void)
{
	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// 古い位置更新
	//transform.posOld = transform.pos;

	//// のっかるマトリックスがあるのであれば処理
	//if (m_pCollObjectRanding != nullptr)
	//{
	//	// 位置を設定
	//	transform.pos = CMyMath::MulWorldMatrix(m_posLocal, *m_pMtxRanding);
	//
	//	// 忘れる
	//	m_pMtxRanding = nullptr;
	//}

	// 設定
	m_pTransformRef->SetInfo(transform);
}

//***************************************************************
// オブジェクトに着地したときの処理
//***************************************************************
void CPlayer::SetRotationFloorLanding(const bool bLanding, const bool bGravity,D3DXMATRIX ObjectMtx)
{
	// 重力をつけないなら
	if (!bGravity)
	{
		m_move.y = 0.0f;
	}

	if (!bLanding)
	{
		m_bRotationFloorLanding = bLanding;
		//m_bRanding = bLanding;

		return;
	}

	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();
	D3DXVECTOR3 vecU(0.0f, 1.0f, 0.0f);
	D3DXMATRIX mtxRot, mtxTrans, mtxInverse;
	D3DXQUATERNION quat, FloorQuat,finalQuat;
	D3DXVECTOR3 worldPos;

	// マトリックスを取得
	m_mtxLandingRotFloor = ObjectMtx;

	if (!m_bRotationFloorLanding)
	{
		D3DXMatrixInverse(&mtxInverse, nullptr, &m_mtxLandingRotFloor);

		D3DXVec3TransformCoord(&m_OnFloorPos, &transform.pos, &mtxInverse);

		m_bRotationFloorLanding = bLanding;
	}

	// 床のマトリックスの座標の要素を抜く
	D3DXMATRIX FloorMtx = m_mtxLandingRotFloor;
	FloorMtx._41 = FloorMtx._42 = FloorMtx._43 = 0;

	// X,Y,Zを軸を代入
	D3DXVECTOR3 X(FloorMtx._11, FloorMtx._12, FloorMtx._13);
	D3DXVECTOR3 Y(FloorMtx._21, FloorMtx._22, FloorMtx._23);
	D3DXVECTOR3 Z(FloorMtx._31, FloorMtx._32, FloorMtx._33);

	// スケールを消す
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, &Y);
	D3DXVec3Normalize(&Z, &Z);

	// スケールを消した向きを設定
	FloorMtx._11 = X.x;
	FloorMtx._12 = X.y;
	FloorMtx._13 = X.z;

	FloorMtx._21 = Y.x;
	FloorMtx._22 = Y.y;
	FloorMtx._23 = Y.z;

	FloorMtx._31 = Z.x;
	FloorMtx._32 = Z.y;
	FloorMtx._33 = Z.z;

	// 床の逆マトリックスを取得
	D3DXMATRIX FloorRotInverse;
	D3DXMatrixInverse(&FloorRotInverse, nullptr, &FloorMtx);

	// ローカル移動方向
	D3DXVECTOR3 moveLocal;

	// 床の向きを抜いた移動方向を設定
	D3DXVec3TransformNormal(&moveLocal, &m_move, &FloorRotInverse);

	// 移動量を設定
	m_OnFloorPos += moveLocal;

	// 移動方向を求める
	D3DXVECTOR3 dir = moveLocal;

	// ローカル空間の移動方向を求める
	float fAngle = atan2f(dir.x, dir.z);
	transform.rot.y = fAngle;

	m_pCollision->GetTransform()->SetInfo(transform);

	if (!m_bRanding)
	{
		m_bRanding = true;

		// ジャンプ状態に変更
		//ChangeState(std::make_shared<CStatePlayerJump>());
	}
}

//***************************************************************
// 着地したときの処理
//***************************************************************
void CPlayer::SetLandingObject(const bool bLanding, const bool bGravity)
{
	if (bLanding)
	{
		m_move.y = 0.0f;
		m_bGravity = bGravity;
	}
	else
	{
		m_bGravity = true;
	}
	m_bRanding = bLanding;
}

//***************************************************************
// プレイヤードラム缶のっかり処理
//***************************************************************
void CPlayer::UpdateDramcan(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// 情報取得
	CTransform* tarnsform = m_pCollision->GetTransform();
	CTransform::Info info = tarnsform->GetInfo();

	// 情報設定
	tarnsform->SetInfo(info);

	// 乗っているドラム缶を動かす
	if (m_pCollObjectRanding != nullptr)
	{
		CTransform::Info infoColl = m_pCollObjectRanding->GetCollision()->GetTransform()->GetInfo();

		infoColl.pos.x += 2.0f;

		m_pCollObjectRanding->GetCollision()->GetTransform()->SetInfo(infoColl);
	}

	// ジャンプ仕様としたら
	if (pInputKeyBoard->GetPress(DIK_SPACE) == true ||
		pInputJoypad->GetPress(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//***************************************************************
// プレイヤーボール状態更新
//***************************************************************
void CPlayer::UpdateBall(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	//// 乗っているもののがない場合やめる
	//if (m_pCollObjectRanding == nullptr)
	//{
	//	// ジャンプ状態に変更
	//	ChangeState(std::make_shared<CStatePlayerNewtral>());
	//	ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
	//	return;
	//}

	// ジャンプ使用としたらやめる
	// ジャンプ仕様としたら
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//***************************************************************
// プレイヤー平均台状態更新
//***************************************************************
void CPlayer::UpdateBalanceBeam(void)
{
	//// キーボード取得
	//CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	//// ジョイパッド取得
	//CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// 移動処理
	MoveFixedRotation(m_fMovementBarance);

	//// ジャンプしたら解除
	//if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true || 
	//	pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	//{
	//	// ジャンプ状態に変更
	//	ChangeState(std::make_shared<CStatePlayerJump>());
	//	ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
	//	return;
	//}
}

//***************************************************************
// プレイヤー平均台状態更新
//***************************************************************
void CPlayer::UpdateBalanceRope(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// 移動処理
	MoveFixedRotation(m_fMovementBarance);

	// ジャンプしたら解除
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true || 
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//***************************************************************
// プレイヤーの移動操作処理
//***************************************************************
void  CPlayer::UpdateMovingFloor(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// ジャンプしたら解除
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//***************************************************************
// プレイヤーの縄跳び処理
//***************************************************************
void CPlayer::UpdateSkippingRope(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// ジャンプ処理
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ初期化
		InitJumpSkipingRope(0.5f);
	}
}

//***************************************************************
// プレイヤーのフラフープ処理
//***************************************************************
void CPlayer::UpdateHulahupe(void)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	if (m_bRanding == false)
	{
		// 待機状態に変更
		ChangeState(std::make_shared<CStatePlayerNewtral>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
	}

	// ジャンプしたら解除
	if (pInputKeyBoard->GetTrigger(DIK_SPACE) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A) == true)
	{
		// ジャンプ状態に変更
		ChangeState(std::make_shared<CStatePlayerJump>());
		ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
		return;
	}
}

//***************************************************************
// プレイヤーのバランス終了処理
//***************************************************************
void CPlayer::UninitBalanse(void)
{
	m_fFriction = m_fFrictionMemo;
}

//***************************************************************
// プレイヤーの乗ったものに向きを合わせる処理
//***************************************************************
void CPlayer::SetToRandingFixd(void)
{
	// 乗っているオブジェクトの向きにする
	if (m_pCollObjectRanding == nullptr)
	{
		return;
	}
	// プレイヤー向き取得
	CTransform::Info infoPlayer = m_pTransformRef->GetInfo();
	CTransform::Info infoModel = m_pCollObjectRanding->GetCollision()->GetTransform()->GetInfo();

	// モデルの大きさ取得
	CModelManager* pModelManager = CManager::GetModelManager();
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_pCollObjectRanding->GetModelID());

	// 大きさにモデルの回転行列を掛ける
	D3DXVECTOR3 sizeWorld = CMyMath::MulRotationMatrix(modelInfo.Size, infoModel.mtxWorld);
	D3DXVECTOR3 rot = Const::VEC3_NULL;

	float fTime = 600.0f;

	// 横に大きければ
	if (sizeWorld.x > sizeWorld.z)
	{
		// 基準向き設定
		float fAngleStand = D3DX_PI * 0.5f;
		float fAnglePlayer = atan2f((infoModel.pos.x - infoPlayer.pos.x), (infoModel.pos.z - infoPlayer.pos.z));
		float fAngleDiff = fAngleStand - fAnglePlayer;

		// 角度正規化
		fAngleDiff = fabsf(CMyMath::NormalizingAngles(fAngleDiff));

		// 基準向きに近かった
		if (fAngleDiff <= D3DX_PI * 0.5f)
		{
			rot.y = D3DX_PI * 0.5f;

			float fLength = (infoModel.pos.x + sizeWorld.x * 0.5f) - infoPlayer.pos.x;

			// 移動量設定
			m_fMovementBarance = fabsf(fLength / fTime);
		}
		// 基準向きに遠かった
		else
		{
			rot.y = -D3DX_PI * 0.5f;

			float fLength = (infoModel.pos.x - sizeWorld.x * 0.5f) - infoPlayer.pos.x;

			// 移動量設定
			m_fMovementBarance = fabsf(fLength / fTime);
		}
	}
	// 奥に大きければ
	else if (sizeWorld.x < sizeWorld.z)
	{
		// 基準向き設定
		float fAngleStand = 0.0f;
		float fAnglePlayer = atan2f((infoModel.pos.x - infoPlayer.pos.x), (infoModel.pos.z - infoPlayer.pos.z));
		float fAngleDiff = fAngleStand - fAnglePlayer;

		// 角度正規化
		fAngleDiff = fabsf(CMyMath::NormalizingAngles(fAngleDiff));

		// 基準向きに近かった
		if (fAngleDiff <= D3DX_PI * 0.5f)
		{
			rot.y = 0.0f;

			float fLength = (infoModel.pos.z + sizeWorld.z * 0.5f) - infoPlayer.pos.z;

			// 移動量設定
			m_fMovementBarance = fabsf(fLength / fTime);
		}
		// 基準向きに遠かった
		else
		{
			rot.y = D3DX_PI;

			float fLength = (infoModel.pos.z - sizeWorld.z * 0.5f) - infoPlayer.pos.z;

			// 移動量設定
			m_fMovementBarance = fabsf(fLength / fTime);
		}
	}

	// 設定
	m_pCharacter3D->SetRotDest(rot);
	m_pTransformRef->SetInfo(infoPlayer);
}

//***************************************************************
// 距離の取得
//***************************************************************
float CPlayer::GetDistance(const D3DXVECTOR3& other)
{
	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	D3DXVECTOR3 diff = transform.pos - other;

	// 距離を返す
	return D3DXVec3Length(&diff);
}

//***************************************************************
// モーションのエフェクトの設定処理
//***************************************************************
void CPlayer::SetMotionEffect(void)
{
	// 空間情報の取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// モーションの取得
	CMotion* pMotion = m_pCharacter3D->GetMotion();

	// フレームないだったら
	if (pMotion->IsEventFrame(15, 15, CPlayer::MOTION_MOVE) || pMotion->IsEventFrame(25, 25, CPlayer::MOTION_MOVE))
	{
		CParticle* pParticle = CParticle::Create(transform.pos, Const::WHITE, 13.0f, 1, CParticle::TYPE_SMOKE);
		pParticle->SetParam({ 1.0f,0.5f,1.0f }, 1, 60, 157, -157);
		pParticle->SetAnim(4, 3, 10, true);
	}

	if (pMotion->IsEventFrame(0, 0, CPlayer::MOTION_LANDING))
	{
		CParticle* pParticle = CParticle::Create(transform.pos, Const::WHITE, 13.0f, 2, CParticle::TYPE_SMOKE);
		pParticle->SetParam({ 2.0f,0.5f,2.0f }, 1, 60);
		pParticle->SetAnim(4, 3, 10, true);
	}
}

//***************************************************************
// 着地しているオブジェクトのパスの判定処理
//***************************************************************
bool CPlayer::CheckLandingMapObj(const std::string& XFilePath)
{
	// 一致していたら
	if (m_aLandingMapObjName == XFilePath)
	{
		return true;
	}
	return false;
}

//***************************************************************
// プレイヤーとオブジェクトの距離
//***************************************************************
D3DXVECTOR3 CPlayer::MeasuringDistance(void)
{
	float fMinDistance = 1000.0f;// 最短距離
	D3DXVECTOR3 delta = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 一番近いオブジェクトとの差分

	for (auto& obj : CGame::GetMapObjectManager()->GetList()) 
	{
		if (obj)
		{
			if (obj->GetXFilePath() == FILEPATH_BALANCEBEAM ||
				obj->GetXFilePath() == FILEPATH_MOVEFLOOR ||
				obj->GetXFilePath() == FILEPATH_DRUMMCAN ||
				obj->GetXFilePath() == FILEPATH_ROPE ||
				obj->GetXFilePath() == FILEPATH_ROD ||
				obj->GetXFilePath() == FILEPATH_BALL)		// ギミックオブジェクトなら
			{
				// 位置の取得
				D3DXVECTOR3 pos = obj->GetCollision()->GetTransform()->GetInfo().pos;

				// プレイヤーの情報取得
				CTransform::Info transform = m_pTransformRef->GetInfo();

				// 位置の差分
				pos -= transform.pos;

				if (pos.x > 0.0f)
				{
					// 距離の算出
					float fDistance = sqrtf(pos.x * pos.x + pos.z * pos.z);

					if (fDistance < fMinDistance)
					{
						fMinDistance = fDistance;

						delta = pos;
					}
				}
			}
		}
	};

	return delta;
}

//***************************************************************
// プレイヤーの移動操作処理
//***************************************************************
bool CPlayer::MoveOperation(float fPlamMovement)
{
	// キーボード取得
	CInputKeyboard* pInputKeyBoard = CManager::GetInputKeyboard();

	// ジョイパッド取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 rotCamera = pCamera->GetRotaition();

	// プレイヤー情報取得
	CTransform::Info transform = m_pTransformRef->GetInfo();
	
	// 移動に
	bool bMove = false;

	// 移動する方向
	D3DXVECTOR3 rotMove = rotCamera;

	// 奥移動
	if (pInputKeyBoard->GetPress(DIK_W) == true)
	{
		// 左移動
		if (pInputKeyBoard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.25f;
		}
		// 右移動
		else if (pInputKeyBoard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.25f;
		}
		else
		{

		}

		// 移動の有無
		bMove = true;
	}
	// 手前移動
	else if (pInputKeyBoard->GetPress(DIK_S) == true)
	{
		// 左移動
		if (pInputKeyBoard->GetPress(DIK_A) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * -0.75f;
		}
		// 右移動
		else if (pInputKeyBoard->GetPress(DIK_D) == true)
		{
			// 移動量追加
			rotMove.y += D3DX_PI * 0.75f;
		}
		else
		{
			// 移動量追加
			rotMove.y += D3DX_PI;
		}

		// 移動の有無
		bMove = true;
	}
	// 左移動
	else if (pInputKeyBoard->GetPress(DIK_A) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * -0.5f;

		// 移動の有無
		bMove = true;
	}
	// 右移動
	else if (pInputKeyBoard->GetPress(DIK_D) == true)
	{
		// 移動量追加
		rotMove.y += D3DX_PI * 0.5f;

		// 移動の有無
		bMove = true;
	}
	// Lスティックで移動
	else if (pInputJoypad->GetJoyStickL() == true)
	{
		XINPUT_STATE* pStick = pInputJoypad->GetJoyStickAngle();

		// 角度を図る
		float fAngle = atan2f((float)(pStick->Gamepad.sThumbLX), (float)(pStick->Gamepad.sThumbLY));
		//float fLength = CMyMath::LengthPositions2D(D3DXVECTOR2((float)(pStick->Gamepad.sThumbLX), 0.0f), D3DXVECTOR2(0.0f, (float)(pStick->Gamepad.sThumbLY)));
		//float fLengthMAX = CMyMath::LengthPositions2D(D3DXVECTOR2((float)(CInputJoypad::ANGLEMAX), 0.0f), D3DXVECTOR2(0.0f, (float)(CInputJoypad::ANGLEMAX)));
		//float fPalamLength = fLength / fLengthMAX;

		// 向き追加
		rotMove.y += fAngle;

		// 移動の有無
		bMove = true;
	}

	// モーションの取得
	CMotion* pMotion = m_pCharacter3D->GetMotion();

	// 動くのであれば移動量追加
	if (bMove == true)
	{
		// 移動量計算
		m_move.x += sinf(rotMove.y) * (m_fMovement * fPlamMovement);
		m_move.z += cosf(rotMove.y) * (m_fMovement * fPlamMovement);

		// 目的の移動方向の設定
		m_pCharacter3D->SetRotDest({ 0.0f,rotMove.y,0.0f });

		// 移動量があればモーションを移動に
		if ((m_move.x != 0.0f && m_move.z != 0.0f) && pMotion->GetType() != MOTION_JUMP)
		{
			if (pMotion->GetType() != MOTION_JUMP)
			{
				pMotion->SetMotion(1, true, 8);
			}
		}
	}
	else if (pMotion != nullptr && pMotion->GetType() == MOTION_MOVE)
	{
		m_pCharacter3D->GetMotion()->SetMotion(0, true, 20);
		ChangeState(std::make_shared<CStatePlayerNewtral>());	
	}
	// プレイヤー情報設定
	m_pTransformRef->SetInfo(transform);

	return bMove;
}

//***************************************************************
// プレイヤーの移動操作処理
//***************************************************************
void CPlayer::MoveFixedRotation(float fMovement)
{
	// プレイヤー情報取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// 移動量計算
	m_move.x = sinf(m_pCharacter3D->GetRotDest().y) * (fMovement);
	m_move.z = cosf(m_pCharacter3D->GetRotDest().y) * (fMovement);

	m_pTransformRef->SetInfo(transform);
	return;
}

//***************************************************************
// モーションの設定処理
//***************************************************************
void CPlayer::SetMotion(const int nType, const int nFrameBlend, const bool bBlend)
{
	// モーションの取得
	CMotion* pMotion = m_pCharacter3D->GetMotion();

	// 取得できなかったら処理しない
	if (pMotion == nullptr) return;

	// モーションの設定処理
	pMotion->SetMotion(nType, bBlend, nFrameBlend);
}

//***************************************************************
// プレイヤーの移動操作処理
//***************************************************************
void CPlayer::SetRotation(D3DXVECTOR3 rot)
{
	// プレイヤー情報取得
	CTransform::Info transform = m_pTransformRef->GetInfo();

	// 向き設定
	transform.rot = rot;

	// プレイヤー情報設定
	m_pTransformRef->SetInfo(transform);
}

//***************************************************************
// ライトの更新処理
//***************************************************************
void CPlayer::UpdateLight(D3DXVECTOR3* pPos)
{
	if (m_nLightID == -1) return;

	// ライトの取得
	auto &light = CManager::GetLight()->GetInfo(m_nLightID);

	// ポイントライトだったら
	if (light.bUse && light.aLight.Type == D3DLIGHT_POINT)
	{
		// ライトの位置の設定
		light.aLight.Position = { pPos->x,pPos->y + Player_Constants::LIGHT_HEIGHT,pPos->z };
	}
}