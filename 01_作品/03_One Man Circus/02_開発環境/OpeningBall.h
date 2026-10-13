//===================================================
//
// オープニングのボールの処理 [OpeningBall.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OPENING_BALL_H_
#define _OPENING_BALL_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "objectX.h"

//***************************************************
// オープニングのボールのクラスの定義
//***************************************************
class COpeningBall : public CObjectX
{
public:
	COpeningBall();
	~COpeningBall();

	// 生成処理
	static COpeningBall* Create(const D3DXVECTOR3& pos);

	void Active(void) { m_bActive = true; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	D3DXVECTOR3 m_move;		// 移動量
	bool m_bActive;			// 動く
};
#endif