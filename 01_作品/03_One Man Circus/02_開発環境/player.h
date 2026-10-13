//**************************************************
// 
// プレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "objectX.h"
#include "character3D.h"
#include "CollisionObject3D.h"
#include "transform.h"
#include "statebase.h"

class CStateBasePlayer;		// ステートベース
class CCollisionObject3D;	// 当たり判定
class CCollisionMapObject;	// マップオブジェクト

// プレイヤークラス
class CPlayer : public CObject
{
public:

	static constexpr const char* FILEPATH_MOVEFLOOR =	"data/MODEL/MapObjectFile/move_floor.x";
	static constexpr const char* FILEPATH_BALL =		"data/MODEL/MapObjectFile/ball.x";
	static constexpr const char* FILEPATH_DRUMMCAN =	"data/MODEL/MapObjectFile/drumcan.x";
	static constexpr const char* FILEPATH_ROPE =		"data/MODEL/MapObjectFile/rope.x";
	static constexpr const char* FILEPATH_BALANCEBEAM = "data/MODEL/MapObjectFile/balancebeam.x";
	static constexpr const char* FILEPATH_ROD =			"data/MODEL/MapObjectFile/rod.x";
	static constexpr const char* FILEPATH_GOAL =		"data/MODEL/MapObjectFile/goal.x";

	static constexpr const float GIMICK_RANGE_X = 90.0f;
	static constexpr const float GIMICK_RANGE_Y = 1000.0f;

	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_MOVE,		// 移動
		MOTION_MOVEBARANSE,	// 移動バランス
		MOTION_JUMP,		// ジャンプ
		MOTION_LANDING,		// 着地
		MOTION_HURA,		// フラフープ
		MOTION_MOVE_JAG,	// 移動ジャグリング
		MOTION_BLOW,		// 吹っ飛び
		MOTION_DOWN,		// ダウン
		MOTION_DOWN_NEWTRAL,// ダウンニュートラル
		MOTION_GETUP,		// 起き上がり
		MOTION_BOW,			// お辞儀
		MOTION_HAND,		// 手を振る

		MOTION_MAX
	};

	CPlayer(const int nPriority = PRIORITY_CHARACTER);
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer* Create(void);

	void ResetBarance(void);
	void CreatePositionLocal(D3DXMATRIX* pMtx, D3DXVECTOR3 pos);
	void DeletePositionLocal(void);
	void ChangeState(std::shared_ptr<CStateBasePlayer> stateBasePlayer);
	void ChangeStateBalance(std::shared_ptr<CStateBasePlayer> stateBasePlayerBalance);
	void ChangeStateCollision(std::string& XFilepath);

	/// <summary>
	/// 着地しているオブジェクトのパスを判定する
	/// </summary>
	/// <param name="マップオブジェクトのパス"></param>
	/// <returns>判定結果(一致ならtrue)</returns>
	bool CheckLandingMapObj(const std::string& XFilePath);
	bool MoveOperation(float fPlamMovement = 1.0f);
	void MoveFixedRotation(float fMovement);
	void InitNewtral(void);
	void InitJump(float fPalamJumpment = 1.0f);
	void InitJumpSkipingRope(float fPalamJumpment);
	void InitBalance(void);
	void InitRebuild(void);
	void InitDramcan(void);
	bool UpdateNone(void);
	bool UpdateNewtral(void);
	bool UpdateBalance(void);
	bool UpdateRebuild(void);
	void UpdateInverse(void);
	void UpdateDramcan(void);
	void UpdateBall(void);
	void UpdateBalanceBeam(void);
	void UpdateBalanceRope(void);
	void UpdateMovingFloor(void);
	void UpdateSkippingRope(void);
	void UpdateHulahupe(void);
	void UninitBalanse(void);
	void SetToRandingFixd(void);
	float GetDistance(const D3DXVECTOR3& other);

	D3DXVECTOR3 MeasuringDistance(void);

	/// <summary>
	/// 回るゆかオブジェクトに着地したときの処理
	/// (Author : YUTO YOSHIDA)
	/// </summary>
	/// <param name="pOutMtx"></param>
	void SetRotationFloorLanding(const bool bLanding,const bool bGravity, D3DXMATRIX ObjectMtx = D3DXMATRIX());
	void SetLandingObject(const bool bLanding, const bool bGravity);

	// ゲッター
	CCollisionObject3D* GetCollision(void) { return m_pCollision.get(); }
	CCollisionMapObject* GetCollObjectRanding(void) { return m_pCollObjectRanding; }
	D3DXVECTOR3 GetPosition(void) { return m_pTransformRef->GetInfo().pos; }
	D3DXVECTOR3 GetAxisBarance(void) { return m_axisBarance; }
	D3DXVECTOR3 GetRotation(void) { return m_pTransformRef->GetInfo().rot; }
	D3DXVECTOR3 GetRotationDest(void) { return m_pCharacter3D->GetRotDest(); }
	D3DXVECTOR3 GetRotationBarance(void) { return m_rotVaranse; }
	D3DXVECTOR3 GetRotationAddBarance(void) { return m_rotAddVaranse; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetVecMove(void) { return m_pTransformRef->GetInfo().pos - m_pTransformRef->GetInfo().posOld; }
	D3DXMATRIX GetMatrixRotation(void) { return m_mtxRot; }
	D3DXMATRIX GetMatrix(void) { return m_pTransformRef->GetInfo().mtxWorld; }
	float GetMovement(void) { return m_fMovement; }
	float GetJumpment(void) { return m_fJumpment; }
	float GetFriction(void) { return m_fFriction; }
	float GetGravity(void) { return m_fGravity; }
	float GetLowLimit(void) { return m_fLowLimit; }
	bool GetIsRanding(void) { return m_bRanding; }
	int GetCountMax(void) { return m_nCntMax; }
	int GetCountScore(void) { return m_nCntScore; }
	int GetScore(void) { return m_nScore; }
	int GetStateID(void) { return m_pStateMachine->GetID(); }
	int GetStateBaranceID(void) { return m_pStateMachineBarance->GetID(); }
	int GetRuluInfo(void) { return m_nRuluInfo; }
	
	// セッター
	void SetMotion(const int nType, const int nFrameBlend, const bool bBlend);
	void SetAxisBarance(D3DXVECTOR3 axis) { m_axisBarance = axis; }
	void SetRotation(D3DXVECTOR3 rot);
	void SetRotationDest(D3DXVECTOR3 rot) { m_pCharacter3D->SetRotDest(rot); }
	void SetPosition(D3DXVECTOR3 pos) { m_pTransformRef->SetPosition(pos); }
	void SetRotationBarance(D3DXVECTOR3 rot) { m_rotVaranse = rot; }
	void SetRotationBaranceOld(D3DXVECTOR3 rot) { m_rotVaranseOld = rot; }
	void SetRotationAddBarance(D3DXVECTOR3 rot) { m_rotAddVaranse = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetMatrixRotation(D3DXMATRIX mtx) { m_mtxRot = mtx; }
	void SetMovement(float fMovement) { m_fMovement = fMovement; }
	void SetJumpment(float fJumpment) { m_fJumpment = fJumpment; }
	void SetFriction(float fFriction) { m_fFriction = fFriction;}
	void SetFrictionMemo(float fFriction) { m_fFrictionMemo = fFriction; }
	void SetGravity(float fGravity) { m_fGravity = fGravity; }
	void SetLowLimit(float fLowLimit) { m_fLowLimit = fLowLimit; }
	void SetXLimit(float fXLimit) { m_fXLimit = fXLimit; }
	void SetZLimit(float fZLimit) { m_fZLimit = fZLimit; }
	void SetIsRanding(bool bRanding) { m_bRanding = bRanding; }
	void SetCountMax(int nMax) { m_nCntMax = nMax; }
	void SetCountScore(int nCntScore) { m_nCntScore = nCntScore; }
	void SetScore(int nScore) { m_nScore = nScore; }
	void SetAddScore(int nAddScore) { m_nAddScore = nAddScore; }
	void SetRebuildTime(int nRebuildTime) { m_nRebuildTime = nRebuildTime; }
	void SetRuluInfo(int nRuluInfo) { m_nRuluInfo = nRuluInfo; }
private:
	void UpdateLight(D3DXVECTOR3* pPos);
	void SetMotionEffect(void);
private:
	std::unique_ptr<CCharacter3D> m_pCharacter3D;			// キャラクター
	std::unique_ptr<CCollisionObject3D> m_pCollision;		// 当たり判定用クラス
	std::unique_ptr<CStateMachine> m_pStateMachine;			// ステートマシーン
	std::unique_ptr<CStateMachine> m_pStateMachineBarance;	// ステートマシーンバランス
	CCollisionMapObject* m_pCollObjectRanding;				// 乗っているオブジェクトポインタ
	CTransform* m_pTransformRef;							// 参照用空間情報クラス
	std::string m_aLandingMapObjName;						// 着地しているオブジェクトのパス
	D3DXVECTOR3 m_posLocal;									// ローカル位置
	D3DXVECTOR3 m_axisBarance;								// バランス軸
	D3DXVECTOR3 m_rotVaranse;								// バランス角度
	D3DXVECTOR3 m_rotVaranseOld;							// 古いバランス角度
	D3DXVECTOR3 m_rotVaranseDiff;							// バランス角度差分
	D3DXVECTOR3 m_rotAddVaranse;							// 足すバランス角度
	D3DXVECTOR3 m_move;										// 移動量
	D3DXMATRIX* m_pMtxRanding;								// 乗っているもののマトリックスポインター
	D3DXMATRIX m_mtxRot;									// 回転マトリックス
	D3DXMATRIX m_mtxLandingRotFloor;						// 回る床に着地したときのマトリックス
	D3DXVECTOR3 m_OnFloorPos;								// 回る床に乗った時の位置
	float m_fMovement;										// 移動量
	float m_fMovementBarance;								// バランス時移動量
	float m_fJumpment;										// ジャンプ量量
	float m_fFriction;										// 摩擦係数
	float m_fFrictionMemo;									// 摩擦係数
	float m_fGravity;										// 重力
	float m_fLowLimit;										// 限界低度
	float m_fXLimit;										// 限界X値
	float m_fZLimit;										// 限界Y
	float m_fInertia;										// 慣性
	bool m_bRanding;										// 乗っているか否か
	bool m_bMove;											// 移動しているかどうか
	bool m_bRotationFloorLanding;							// 回転床に着地しているかどうか
	bool m_bGravity;										// 重力をつけるかどうか(オブジェクトの着地用変数)
	int m_nModelIdx;										// モデルのID
	int m_nCntScore;										// スコアカウンター
	int m_nCntMax;											// スコアの最大カウンター
	int m_nScore;											// スコア
	int m_nAddScore;										// 足すスコア
	int m_nRebuildTime;										// 立て直し時間
	int m_nCntRebuild;										// 立て直しカウンター
	int m_nRuluInfo;										// ルールの戻り値
	int m_nLightID;											// ライトのID
};

#endif