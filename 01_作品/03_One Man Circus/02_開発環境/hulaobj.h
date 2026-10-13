//***************************************************
//
// フラフープ
// Author:YUTO YOSHIDA
//
//***************************************************
#ifndef _HULAOBJ_H_
#define _HULAOBJ_H_

#include "objectX.h"

class CPlayer;

// ジャグリングオブジェ
class CHula : public CObjectX
{
public:
	CHula();
	~CHula();

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	CObjectX* m_pHulaObj;	// ジャグリング用オブジェクト
	float m_fmove;
};
#endif
