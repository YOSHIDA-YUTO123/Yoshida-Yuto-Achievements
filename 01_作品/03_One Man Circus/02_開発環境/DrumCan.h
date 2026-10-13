//==================================================
// 
// ドラム缶 [DramCan.h]
// Author：Rio Ohno
// 
//==================================================

#ifndef _DRAMCAN_H_
#define _DRAMCAN_H_

// インクルード
#include "objectX.h"

// マクロ定義
#define DRAMCAN_MODEL_PASS ((const char*)"data/MODEL/Model()/drumcan.x")

// ドラム缶クラス
class CDramcan : public CObjectX
{
public:
	CDramcan();
	~CDramcan();

	static CDramcan* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void isMove(D3DXVECTOR3 move);
	bool Collison(D3DXVECTOR3* pOutPos, D3DXVECTOR3 pos);

	// セッター
	void SetMove(D3DXVECTOR3 rot, float fMoveMent);
	void SetIsMove(bool bMove) { m_bMove = bMove; }
	void SetPositionLocal(D3DXVECTOR3 pos) { m_posLocal = pos; }

	// ゲッター
	D3DXVECTOR3 GetPositionLocalToWorld(void) { return m_posLocalToWorld; }
private:
	D3DXVECTOR3 m_posLocal;			// ローカル位置
	D3DXVECTOR3 m_posLocalToWorld;	// ローカル位置をワールド座標に変換した値
	D3DXVECTOR3 m_move;				// 移動量
	float m_fRadius;				// 半径
	bool m_bMove;					// 移動の有無
};
#endif