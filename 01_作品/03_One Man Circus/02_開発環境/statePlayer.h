//**************************************************
// 
// ステートプレイヤー
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _STATEPlAYER_H_
#define _STATEPlAYER_H_

#include "statebase.h"
#include "player.h"
#include "game.h"
#include "rulebase.h"
#include "ruleBalance.h"
#include "ruleHula.h"
#include "ruleJuggling.h"

// 前方宣言
class CPlayer;
class CObjectX;
class CRuleBalance;
class CRuleCRuleHula;
class CRuleJuga;
class CRuleSkip;
class CBallJug;

// ステートプレイヤークラス
class CStateBasePlayer : public CStateBase
{
public:
	// ID列挙
	enum ID
	{
		ID_BASE = 0,
		ID_NONE,				// 無し
		ID_NEWTRAL,				// 待機
		ID_MOVE,				// 移動
		ID_JUMP,				// ジャンプ
		ID_BALL,				// ボール
		ID_BALANCEBEAM,			// 平均台
		ID_ROPE,				// 綱
		ID_JUMPBALANCE,			// ジャンプバランス
		ID_MOVINGFLOOR,			// 移動床
		ID_DRAMCAN,				// ドラム缶
		ID_JUGGLING,			// ジャグリング
		ID_SKIPPINGROPE,		// 縄跳び
		ID_HULAHOOP,			// フラフープ
		ID_REBUILD,				// 立て直し
		ID_BALANCE_NONE,		// バランス待機
		ID_BALANCE_BALANCE,		// バランス
		ID_BALANCE_REBUILD,		// バランス立て直し
		ID_MAX
	};

	CStateBasePlayer() { m_pPlayer = nullptr; }
	CStateBasePlayer(ID id) { m_id = id; m_pPlayer = nullptr;}
	virtual ~CStateBasePlayer() {}

	virtual void Init(void) {};
	virtual void Uninit(void) {};
	virtual void Update(void) {};
	virtual void Draw(void) {};
	int GetID(void) const { return m_id; }
	void SetOwner(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

protected: 
		CPlayer* GetPlayer(void) { return m_pPlayer; }

private:
	CPlayer* m_pPlayer;
	ID m_id;
};

// プレイヤーステートなしクラス
class CStatePlayerNone : public  CStateBasePlayer
{
public:
	CStatePlayerNone() : CStateBasePlayer(ID_NONE) {}
	~CStatePlayerNone() {}

	void Init(void) override {}
	void Update(void) override;
	void Uninit(void) override {}

private:
};

//-----------------------------------------------------------------------
// プレイヤー

// プレイヤーステート待機クラス
class CStatePlayerNewtral : public  CStateBasePlayer
{
public:
	CStatePlayerNewtral(): CStateBasePlayer(ID_NEWTRAL){}
	~CStatePlayerNewtral() {}
	
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステート移動クラス
class CStatePlayerMove : public  CStateBasePlayer
{
public:
	CStatePlayerMove() : CStateBasePlayer(ID_MOVE) {}
	~CStatePlayerMove() {}

	void Init(void) override {}
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステートジャンプクラス
class CStatePlayerJump : public  CStateBasePlayer
{
public:
	CStatePlayerJump() : CStateBasePlayer(ID_JUMP) {}
	~CStatePlayerJump() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステートボールクラス
class CStatePlayerBall : public  CStateBasePlayer
{
public:
	static constexpr float DISTANCEMOVE = 80.0f;
	static constexpr float TIMEMOVE = 120.0f;
	static constexpr float MOVEMENT = DISTANCEMOVE / TIMEMOVE;

	CStatePlayerBall() : CStateBasePlayer(ID_BALL) { m_pObjectRanding = nullptr; }
	~CStatePlayerBall() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
	CCollisionMapObject* m_pObjectRanding;	// 乗っているオブジェクトポインタ
	D3DXVECTOR3 m_posOrigin;				// 元の位置
	D3DXVECTOR3 m_posDestA;					// 目的地
	D3DXVECTOR3 m_posDestB;					// 目的地
	D3DXVECTOR3 m_vecMove;					// 移動距離
	int m_nCntTime;							// 時間
	bool m_bRight;							// 右移動か否か
};

// プレイヤーステート平均台クラス
class CStatePlayerBalanceBeam : public  CStateBasePlayer
{
public:
	static constexpr float ADDROTSTION = D3DX_PI * 0.1f;	// 増やす角度
	static constexpr float RADIUS = 15.0f;					// 半径
	static constexpr float DIPS = -10.0f;					// 深さ

	CStatePlayerBalanceBeam() : CStateBasePlayer(ID_BALANCEBEAM) { m_rotFixed = Const::VEC3_NULL; }
	~CStatePlayerBalanceBeam() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
	D3DXVECTOR3 m_rotFixed;				// 固定用向き
	CBallJug* m_pJugObj;// ジャグリング用オブジェクト
};

// プレイヤーステート移動床クラス
class CStatePlayerMovingFloor : public  CStateBasePlayer
{
public:
	CStatePlayerMovingFloor() : CStateBasePlayer(ID_MOVINGFLOOR) {}
	~CStatePlayerMovingFloor() {}

	void Init(void) override {CGame::ChangeRule(new CRuleBalance);}
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステート綱クラス
class CStatePlayerRope : public  CStateBasePlayer
{
public:
	CStatePlayerRope() : CStateBasePlayer(ID_ROPE) { m_rotFixed = Const::VEC3_NULL;  m_pRuluBalance = nullptr;}
	~CStatePlayerRope() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
	D3DXVECTOR3 m_rotFixed;			// 固定用向き
	CRuleBalance* m_pRuluBalance;	// バランスルール
};

// プレイヤーステートドラム缶クラス
class CStatePlayerDramcan : public  CStateBasePlayer
{
public:
	static constexpr float DISTANCEMOVE = 20.0f;
	static constexpr float TIMEMOVE = 30.0f;
	static constexpr float MOVEMENT = DISTANCEMOVE / TIMEMOVE;

	CStatePlayerDramcan() : CStateBasePlayer(ID_DRAMCAN) {}
	~CStatePlayerDramcan() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
	CCollisionMapObject* m_pObjectRanding;	// 乗っているオブジェクトポインタ
	D3DXVECTOR3 m_posOrigin;				// 元の位置
	D3DXVECTOR3 m_posDestA;					// 目的地
	D3DXVECTOR3 m_posDestB;					// 目的地
	D3DXVECTOR3 m_vecMove;					// 移動距離
	D3DXVECTOR3 m_rotMove;					// 移動向き
	int m_nCntTime;							// 時間
	bool m_bRight;							// 右移動か否か
};

// プレイヤーステートジャグリングクラス
class CStatePlayerJuggling : public  CStateBasePlayer
{
public:
	static constexpr float ADDROTSTION = D3DX_PI * 0.01f;	// 増やす角度
	static constexpr float RADIUS = 15.0f;					// 半径
	static constexpr float DIPS = 10.0f;					// 深さ

	CStatePlayerJuggling();
	~CStatePlayerJuggling() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
	CBallJug* m_pJugObj;// ジャグリング用オブジェクト
	float m_fAngle;						// 角度
};

// プレイヤーステート縄跳びクラス
class CStatePlayerSkippingRope : public  CStateBasePlayer
{
public:
	CStatePlayerSkippingRope() : CStateBasePlayer(ID_SKIPPINGROPE) {}
	~CStatePlayerSkippingRope() {}

	void Init(void) override {}
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステートフラフープクラス
class CStatePlayerHulaHoop : public  CStateBasePlayer
{
public:
	static constexpr float ANGLE_LIMIT_DEG = 20.0f;
	static constexpr int TIME_LAP = 60;
	static constexpr float ANGLEADD_DEG = 360.0f / (float)TIME_LAP;
	static constexpr float RADIUS = 20.0f;
	static constexpr float PALAM_ANGLE_STICK = 0.008f;
	static constexpr float PALAM_ANGLE_ARROW = 0.015f;

	CStatePlayerHulaHoop() : CStateBasePlayer(ID_HULAHOOP) { m_fAngleAll = 0.0f; m_fAngleMove = 0.0f; m_fAngleOld = 0.0f; m_fAngleInputOld = 0.0f; m_pHulaHoop = nullptr; }
	~CStatePlayerHulaHoop() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
	float m_fAngleAll;		// 基準角度
	float m_fAngleMove;		// 基準角度移動量
	float m_fAngleInputOld;		// 基準角度移動量
	float m_fAngleOld;		// 基準角度移動量
	CObjectX* m_pHulaHoop;	// フラフープ
};

//-----------------------------------------------------------------------
// プレイヤ-バランス

class CStatePlayerJumpBarance : public  CStateBasePlayer
{
public:
	CStatePlayerJumpBarance() : CStateBasePlayer(ID_JUMP) {}
	~CStatePlayerJumpBarance() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステートバランス待機クラス
class CStatePlayerBalanceNone : public  CStateBasePlayer
{
public:
	CStatePlayerBalanceNone() : CStateBasePlayer(ID_BALANCE_NONE) {}
	~CStatePlayerBalanceNone() {}

	void Init(void) override {}
	void Update(void) override;
	void Uninit(void) override {}
	void Draw(void) {}

private:
};

// プレイヤーステートバランスクラス
class CStatePlayerBalance : public  CStateBasePlayer
{
public:
	CStatePlayerBalance() : CStateBasePlayer(ID_BALANCE_BALANCE) {}
	~CStatePlayerBalance() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
};

// プレイヤーステートバランス立て直しクラス
class CStatePlayerBalanceRebuild : public  CStateBasePlayer
{
public:
	CStatePlayerBalanceRebuild() : CStateBasePlayer(ID_BALANCE_REBUILD) {}
		~CStatePlayerBalanceRebuild() {}

	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void Draw(void) {}

private:
};
#endif