//*************************************
// 
// フォントマネージャー
// Author Tetsuji Yamamoto
// 
//*************************************
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "main.h"
#include "resource.h"

// マクロ定義
#define FONT_NAME_たぬき油性マジック "たぬき油性マジック"
#define FONT_NAME_HG明朝B "HG明朝B"
#define FONT_NAME_JF_DOT_M_10 "JFドットM+10"

// フォントマネージャー
class CFontManager
{
public:

	// フォント情報
	typedef struct
	{
		HRSRC hFontResource;	// リソースの場所
		LPD3DXFONT pFont;		// フォントへのポインタ
	}FONTINFO;

	CFontManager();
	~CFontManager();

	void Init(void);
	void Uninit(void);
	static CFontManager* Create(void);
	static const char* Regiter(int nFontID, const char* pFontName);
	static LPD3DXFONT UnLord(const char* pFontName);
	static LPD3DXFONT GetFont(const char* pFontName);

private:
	static std::map<std::string, FONTINFO> m_mFontInfo;	// フォント情報
};

#endif // !_FONTMANAGER_H_
