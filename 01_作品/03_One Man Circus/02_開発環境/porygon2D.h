//****************************************************
// 
// 2Dのポリゴン
// Author Tetsuji Yamamoto
// 
//****************************************************
#ifndef _PORYGON2D_H_
#define _PORYGON2D_H_

#include "main.h"

// ゲーム2Dクラス
class CPolygon2D
{
public:
	CPolygon2D();
	~CPolygon2D();

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight,const char*pTexturePass);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetColor(D3DXCOLOR col) { m_col = col; }

	// ゲッター
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	static CPolygon2D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, const char* pTexturePass);


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXCOLOR m_col;					// 色
	int m_nIdxTexture;					// テクスチャへパス
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
};

#endif // !_GAME2D_H_