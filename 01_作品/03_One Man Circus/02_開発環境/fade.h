//===================================================
//
// フェード処理 [fade.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FADE_H_
#define _FADE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include<memory>

//***************************************************
// 前方宣言
//***************************************************
class CScene;

//***************************************************
// フェードクラスの定義
//***************************************************
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,			// フェードなしの状態
		FADE_IN,				// フェードインの状態
		FADE_OUT,				// フェードアウトの状態
		FAFE_MAX
	}FADE;

	~CFade();

	static CFade* Create(void);

	void SetFade(std::unique_ptr<CScene> pNewScene, const int nTime = 60, const D3DXCOLOR& Incol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), const D3DXCOLOR& OutColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	FADE GetState(void) const { return m_Fade; }

private:
	CFade();
	std::unique_ptr<CScene> m_pScene;		// シーンのポインタ
	FADE m_Fade;							// フェード
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   // 頂点バッファへのポインタ
	D3DXCOLOR m_InColor;					// フェードインの色
	D3DXCOLOR m_OutColor;					// フェードアウトの色
	float m_fAlphaChangeSpeed;				// アルファ値の変化スピード
};

#endif
