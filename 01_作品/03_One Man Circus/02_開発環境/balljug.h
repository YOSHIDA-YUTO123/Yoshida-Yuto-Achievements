//***************************************************
//
// ジャグリングボール
// Author:
//
//***************************************************
#ifndef _BALLJUG_H_
#define _BALLJUG_H_

#include "objectX.h"

class CPlayer;

// ジャグリングオブジェ
class CBallJug : public CObjectX
{
public:
	static constexpr float ADDROTSTION = D3DX_PI * 0.1f;	// 増やす角度
	static constexpr float RADIUS = 15.0f;					// 半径
	static constexpr float DIPS = -10.0f;					// 深さ
	static constexpr int APPTIME = 30;					// 深さ

	CBallJug();
	~CBallJug();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Jugling(void);
	void fall(void);
	void fire(void);
	void app(void);

	void Setplayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetIsFall(bool bFall) { m_bFall = bFall; m_fRadius = 0.0f; }
	bool GetIsLoss(void) { return m_bLoss; }
	static CBallJug* Create(void);
private:
	CPlayer* m_pPlayer;					// プレイヤーポインタ
	std::vector<CObjectX*> m_vpJugObj;	// ジャグリング用オブジェクト
	float m_fAngle;						// 角度
	float m_fAngleOld;					// 角度
	float m_fAngleDest = 0.0f;			// 角度
	float m_fRadius;
	bool m_bApp;
	bool m_bFall;
	bool m_bFire;
	bool m_bLoss;// 失敗
	int m_nCnt;
};
#endif
