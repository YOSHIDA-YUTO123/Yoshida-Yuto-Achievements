//================================================
//
// コライダー [collider.h]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// 多重インクルード防止
//************************************************
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//************************************************
// インクルードファイル
//************************************************
#include"main.h"
#include <memory>
#include <string>

//************************************************
// 前方宣言
//************************************************
class CTransform;

//************************************************
// コライダークラスの定義
//************************************************
class CCollider
{
public:

	// コライダーの種類
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

	CCollider();
	CCollider(const TYPE type);
	virtual ~CCollider();

	// データ取得
	virtual const void* GetData(void) const { return nullptr; }

	virtual void Update(void);
	virtual void UpdateData(void) {};

	CTransform* GetTransform(void) { return m_pTransform; }
	void SetTransform(CTransform* pTransform) { m_pTransform = pTransform; }
	void SetTag(const char* pTag) { m_tag = pTag; }

	void DeleteTransform(void);
	TYPE GetType(void) const { return m_type; }
	const char* GetTag(void) const { return m_tag.c_str(); }

	void SetResult(const bool bResult) { m_bHit = bResult; }
protected:
	D3DXCOLOR GetLineColor(void) const { return m_col; }
private:
	virtual void DrawLine(void) {};

	CTransform* m_pTransform;	// トランスフォームクラスへのポインタ
	std::string m_tag;			// コライダーのタグ
	TYPE m_type;				// コライダーの種類
	D3DXCOLOR m_col;			// 色
	bool m_bHit;				// 当たったかどうか
};

//************************************************
// AABBのコライダークラスの定義
//************************************************
class CColliderAABB : public CCollider
{
public:
	struct Data
	{
		D3DXVECTOR3 WorldCenterPos; // 世界から見た中心座標
		D3DXVECTOR3 LocalCenterPos;	// ローカルの中心座標
	};

	CColliderAABB();
	~CColliderAABB();
	const void* GetData(void) const override { return &m_Data; };
	void Update(void) override;
	void UpdateData(void) override;

	static std::unique_ptr<CColliderAABB> Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	static CColliderAABB CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	static CColliderAABB* Create(const D3DXVECTOR3 CenterPos);
private:
	void DrawLine(void) override;
	Data m_Data; // 情報
};

//************************************************
// Sphereのコライダークラスの定義
//************************************************
class CColliderSphere : public CCollider
{
public:
	CColliderSphere();
	~CColliderSphere();

	const void* GetData(void) const override { return nullptr; };
	void Update(void) override;
	void UpdateData(void) override {};

	static std::unique_ptr<CColliderSphere> Create(const D3DXVECTOR3 pos,const float fRadius, CTransform* pTransform);
	static CColliderSphere* CreateRawPtr(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform);
	static CColliderSphere CreateCollider(const D3DXVECTOR3 pos, const float fRadius);
private:
	void DrawLine(void) override;
};

//************************************************
// FOVのコライダークラスの定義
//************************************************
class CColliderFOV : public CCollider
{
public:

	struct Data
	{
		float fLength;		// 視界の長さ
		float fNowAngle;	// 今の角度
		float fAngleLeft;	// 視野左
		float fAngleRight;	// 視野右
	};

	CColliderFOV();
	~CColliderFOV();
	static std::unique_ptr<CColliderFOV> Create(const float fAngle, const float fAngleLeft, const float fAngleRight,const float fLength);
	static CColliderFOV* CreateRawPtr(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength);

	static CColliderFOV CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength);
	const void* GetData(void) const override { return &m_Data; }
	void Update(void) override;
	void UpdateData(void) override {};

private:
	void DrawLine(void) override;
	Data m_Data; // データ
};

//************************************************
// カプセルコライダークラスの定義
//************************************************
class CColliderCapsule : public CCollider
{
public:

	// 当たり判定のデータ
	struct Data
	{
		D3DXVECTOR3 StartPos; // 始点
		D3DXVECTOR3 EndPos;	// 終点
		float fRadius;		// 半径
	};

	CColliderCapsule();
	~CColliderCapsule();

	static std::unique_ptr<CColliderCapsule> Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius);
	static CColliderCapsule* CreateRawPtr(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos);
	static CColliderCapsule CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius);
	const void* GetData(void) const override { return &m_Data; }
	void Update(void) override;
	void UpdateData(void) override {};

private:
	void DrawLine(void) override;
	Data m_Data;			  // データ
	D3DXVECTOR3 m_LocalStart; // 始点の位置
	D3DXVECTOR3 m_LocalEnd;	  // 終点の位置
};

//************************************************
// OBBクラスの定義
//************************************************
class CColliderOBB : public CCollider
{
public:

	struct Data
	{
		D3DXVECTOR3 WorldCenterPos; // 世界から見た中心座標
		D3DXVECTOR3 LocalCenterPos;	// ローカルの中心座標
	};

	CColliderOBB();
	~CColliderOBB();

	static CColliderOBB* Create(void);
	const void* GetData(void) const override { return &m_Data; }
	void Update(void) override;
	void UpdateData(void) override;
private:
	void DrawLine(void) override;
	Data m_Data; // 情報構造体
};
#endif