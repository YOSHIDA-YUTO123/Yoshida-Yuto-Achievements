//===================================================
//
// タイトルロゴ [TitleLogo.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object2D.h"

//***************************************************
// タイトルロゴクラスの定義
//***************************************************
class CTitleLogo : public CObject2D
{
public:
	CTitleLogo();
	~CTitleLogo();

	static CTitleLogo* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	D3DXCOLOR m_col;	// 色
	float m_fAddAlv;	// アルファ値の増加速度
};

#endif