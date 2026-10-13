//**********************************************************
// 
// ボタンクラス
// Author Tetsuji Yamamoto
// 
//**********************************************************
#ifndef  _BUTTON_H_
#define  _BUTTON_H_

#include "object.h"
#include "fontManager.h"

// 前方宣言
class CWord;		// 文字列クラス
class CPolygon2D;	// 2Dポリゴン

// ボタンクラス
class CButton : public CObject
{
public:

	// 情報
	typedef struct
	{
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 rot;			// 向き
		float fWidth;				// 幅
		float fHeight;				// 高さ
		D3DXCOLOR colWord;			// 文字通常色
		D3DXCOLOR colWordEmp;		// 文字協調常色
		D3DXCOLOR colPolygon;		// ポリゴン通常色
		D3DXCOLOR colPolygonEmp;	// ポリゴン協調常色
	}INFO;

	CButton();
	~CButton();

	HRESULT Init(void) { return S_OK; }
	void Init(INFO info, const char*pStrWord,const char* pTexturePassPolygon = NULL, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B, DWORD format = DT_CENTER);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButton* Create(INFO info, const char* pStrWord, const char* pTexturePassPolygon = NULL, int nFontID = IDR_HG明朝B, const char* pFontName = FONT_NAME_HG明朝B, DWORD format = DT_CENTER);

	// ゲッター
	bool GetIsTouch(void) { return m_bTouch; }
	bool GetIsPress(void) { return m_bPress; }

private:
	CWord* pWord;				// 文字列クラス
	CPolygon2D* pPolygon2D;		// 2Dポリゴン
	INFO m_info;				// 情報
	bool m_bTouch;				// 触れているか否か
	bool m_bPress;				// 押しているか否か
};

#endif // ! _BUTTON_H_
