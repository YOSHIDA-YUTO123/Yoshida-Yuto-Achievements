//================================================
//
// 当たり判定 [collision.h]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// 多重インクルード防止
//************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************
// インクルードファイル
//************************************************
#include"main.h"
#include"object.h"
#include<memory>

//************************************************
// 前方宣言
//************************************************
class CCollider;

//************************************************
// 当たり判定のクラスの定義
//************************************************
class CCollision
{
public:

	// 判定の種類
	typedef enum
	{
		TYPE_BASE = 0,	// 基底クラス
		TYPE_AABB,		// 矩形
		TYPE_SPHERE,	// 球
		TYPE_FOV,		// 視界
		TYPE_CAPSULE,	// カプセル
		TYPE_OBB,		// 回転矩形
		TYPE_MAX
	}TYPE;

	CCollision(TYPE type);
	virtual ~CCollision();

	static CCollision* Create(const TYPE type);

	TYPE GetType(void) const { return m_type; }

private:
	TYPE m_type;		// 当たり判定の種類
};

//************************************************
// 当たり判定AABBクラス
//************************************************
class CCollisionAABB : public CCollision
{
public:

	// 面の種類
	typedef enum
	{
		FACE_ALL = 0,
		FACE_RIGHT,
		FACE_LEFT,
		FACE_FRONT,
		FACE_BACK,
		FACE_TOP,
		FACE_BUTTOM,
		FACE_MAX
	}FACE;

	~CCollisionAABB();
	static bool Collision(CCollider* pMyBox, CCollider* pTargetBox, D3DXVECTOR3* pushPos = nullptr, int* pFace = nullptr);
	static bool CollisionUnderPos(CCollider* pMyBox, CCollider* pTargetBox, D3DXVECTOR3* pushPos = nullptr, int* pFace = nullptr);
private:
	CCollisionAABB();
};

//************************************************
// 当たり判定(円)クラス
//************************************************
class CCollisionSphere : public CCollision
{
public:
	~CCollisionSphere();
	static bool Collision(CCollider* myCollider, CCollider* otherCollider);
private:
	CCollisionSphere();
};

//************************************************
// 当たり判定(視界)クラス
//************************************************
class CCollisionFOV : public CCollision
{
public:
	~CCollisionFOV();

	static bool Collision(const D3DXVECTOR3 otherpos, CCollider* pFOV);
private:
	CCollisionFOV();
};

//************************************************
// 当たり判定(カプセル)クラス
//************************************************
class CCollisionCapsule : public CCollision
{
public:
	~CCollisionCapsule();

	/**
	* @brief カプセルvsカプセル
	* @param 自分のカプセル
	* @param 相手のカプセル
	* @param 自分の最近点
	* @param 相手の最近点
	* @return 判定の結果
	*/
	static bool Collision(CCollider* myCapsule, CCollider* otherCapsule, D3DXVECTOR3* NearPos1 = nullptr, D3DXVECTOR3* NearPos2 = nullptr);

	/**
	* @brief カプセルvs円
	* @param 自分のカプセル
	* @param 相手の円
	* @return 判定の結果
	*/
	static bool CollisionSphere(CCollider* pCapsule, CCollider* pSphere);
private:
	CCollisionCapsule();
	static float ClosestPtSegmentSegment(D3DXVECTOR3 Start1, D3DXVECTOR3 End1, D3DXVECTOR3 Start2, D3DXVECTOR3 End2, float* pParam1, float* pParam2, D3DXVECTOR3* c1osestPos1, D3DXVECTOR3* c1osestPos2);
};

//************************************************
// 当たり判定(円)クラス
//************************************************
class CCollisionOBB : public CCollision
{
public:
	// 面の種類
	typedef enum
	{
		FACE_ALL = 0,	// すべての面
		FACE_RIGHT,		// 右
		FACE_LEFT,		// 左
		FACE_FRONT,		// 前面
		FACE_BACK,		// 奥
		FACE_TOP,		// 上の面
		FACE_BUTTOM,	// 下の面
		FACE_MAX
	}FACE;

	~CCollisionOBB();

	static bool Collision(CCollider* pOBB, CCollider* pAABB, D3DXVECTOR3* pPush = nullptr, int* pFace = nullptr);

private:
	CCollisionOBB();
};

#endif
