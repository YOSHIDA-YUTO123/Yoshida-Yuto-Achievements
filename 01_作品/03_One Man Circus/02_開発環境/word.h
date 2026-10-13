//**************************************
// 
// ワード
// AouthorTetsuji Yamamoto
// 
//**************************************
#ifndef _WORD_H_
#define _WORD_H_

#include "main.h"
#include "object2D.h"
#include "fontManager.h"

// ワードクラス
class CWord
{
public:

	CWord();
	~CWord();

	void Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* pStr, DWORD format = DT_CENTER, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CWord* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* pStr, DWORD format = DT_CENTER, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B);

	// セッター
	void SetColor(D3DXCOLOR col);
	void SetStringWord(const char* pStr) { if (pStr != NULL) { m_sStrWord = pStr; } }

	// ゲッター
	D3DXCOLOR GetColor(void) { return m_col; }

private:
	LPDIRECT3DTEXTURE9 m_pTextureFont;		// フォント用のテクスチャ
	LPDIRECT3DSURFACE9 m_pSurfaceTexFont;	// フォント用テクスチャのレンダーターゲット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXCOLOR m_col;						// カラー
	std::string m_sStrFont;					// フォントの名前
	std::string m_sStrWord;					// 書き出す文字列
	DWORD m_format;							// 文字の書き出し形式
	int m_nStrWidth;						// 文字列の幅
};

#endif // !_WORD_H_
