//===============================================
// 
// オブジェクト2Dのアニメーションクラス　[object2DAnim.h]
// Author：Rio Ohno
// 
//===============================================

#ifndef _OBJECT2DANIM_H_
#define _OBJECT2DANIM_H_

// インクルード
#include "object.h"

// オブジェクト2Dのアニメーションクラス
class CObject2DAnim :public CObject
{
public:
	CObject2DAnim(int nPriority = PRIORITY::PRIORITY_UI_BACK);
	~CObject2DAnim();

	static CObject2DAnim* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, int UV, int HV);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	void SetAnim(bool bLoop, int Span);
	void SetAnim(int nU, int nV);
	void SetTextureID(const char* pTextureName);
	void SetColor(const D3DXCOLOR col);

	void UpdateVertex(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR2 m_Size;						// 大きさ
	float m_fAngle;							// 角度
	float m_Length;							// 長さ
	float m_fU;								// 1パターン当たりのu値
	float m_fV;								// 1パターン当たりのv値
	int m_nTextureIdx;						// テクスチャのID
	int m_nCntFream;						// フレームカウンタ
	int m_nCntPattern;						// パターンカウンタ
	int m_nUV;								// テクスチャの分割横
	int m_nHV;								// テクスチャの分割縦
	int m_nSpan;							// アニメーション間隔
	bool m_bLoop;							// ループするかどうか
};

#endif