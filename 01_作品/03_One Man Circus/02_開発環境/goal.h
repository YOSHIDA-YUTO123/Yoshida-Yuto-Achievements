//**************************************************
// 
// ゴール
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _GOAL_H_
#define _GOAL_H_

#include "object.h"
#include "objectX.h"

// 前方宣言
class CMeshCylinder;	// シリンダー

// ゴールクラス
class CGoal : CObjectX
{
public:
	CGoal();
	~CGoal();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGoal* Create(void);

	bool Collision(D3DXVECTOR3* pOutPos, D3DXVECTOR3* pOutMove, D3DXVECTOR3 pos, D3DXVECTOR3 posOld);
	void LoadModel(const char* pModelFilePath);

	// ゲッター
	float GetRadius(void) { return m_fRadius; }
	D3DXVECTOR3 GetPosition(void) { return CObjectX::GetPosition(); }

	// セッター
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetPosition(D3DXVECTOR3 pos) { CObjectX::SetPosition(pos); }

private:
	float m_fRadius;	// 半径
	float m_fHeight;	// 高さ
};

#endif