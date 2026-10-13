//**************************************************
// 
// ボタン
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "button.h"
#include "word.h"
#include "porygon2D.h"
#include "manager.h"
#include "fontManager.h"
#include "input.h"
#include "myMath.h"

//**************************************************
// ボタンデストラクタ
//**************************************************
CButton::CButton()
{
	m_info.pos = Const::VEC3_NULL;			// 位置
	m_info.rot = Const::VEC3_NULL;			// 向き
	m_info.fWidth = 0.0f;					// 幅
	m_info.fHeight = 0.0f;					// 高さ
	m_info.colWord = Const::WHITE;			// 文字通常色
	m_info.colWordEmp = Const::WHITE;		// 文字通常色
	m_info.colPolygon = Const::WHITE;		// ポリゴン通常色
	m_info.colPolygonEmp = Const::WHITE;	// ポリゴン通常色
	pWord = nullptr;						// 文字列クラス
	pPolygon2D = nullptr;					// 2Dポリゴン
	m_bTouch = false;						// 触れているか否か
	m_bPress = false;						// 押しているか否か
}

//**************************************************
// ボタンコンストラクタ
//**************************************************
CButton::~CButton()
{

}

//**************************************************
// ボタン初期化
//**************************************************
void CButton::Init(INFO info, const char* pStrWord, const char* pTexturePassPolygon, int nFontID, const char* pFontName, DWORD format)
{
	// 情報設定
	m_info = info;

	// 文字列初期化
	if (pWord == nullptr)
	{
		pWord = CWord::Create(info.pos, info.colWord, info.fWidth, info.fHeight, pStrWord, format,nFontID, pFontName);
	}

	// ポリゴン初期化
	if (pPolygon2D == nullptr)
	{
		pPolygon2D = CPolygon2D::Create(info.pos,info.rot,info.colPolygon,info.fWidth,info.fHeight,pTexturePassPolygon);
	}
}

//**************************************************
// ボタン終了
//**************************************************
void CButton::Uninit(void)
{
	// 文字列削除
	if (pWord != nullptr)
	{
		pWord->Uninit();
		pWord = nullptr;
	}

	// ポリゴン削除
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Uninit();
		pPolygon2D = nullptr;
	}

	// 自身の破棄
	Release();
}

//**************************************************
// ボタン更新
//**************************************************
void CButton::Update(void)
{
	// マウス情報取得
	CInputMouse*pInputMouse = CManager::GetInputMouse();

	// マウスの位置取得
	D3DXVECTOR2 posMouse = pInputMouse->GetVelocity();

	// ポリゴンの位置と高さと幅を取得
	D3DXVECTOR3 posPoly = pPolygon2D->GetPosition();
	float fWidthPoly = pPolygon2D->GetWidth();
	float fHeightPoly = pPolygon2D->GetHeight();

	// ポリゴンとマウスのが当たっていれば
	if (CMyMath::IsColiisionRectTopoint2D(posMouse, D3DXVECTOR2(posPoly.x, posPoly.y),fWidthPoly,fHeightPoly) == true)
	{
		// 触れている状態にする
		m_bTouch = true;

		// 左クリックで押したことにする
		if (pInputMouse->OnMousePress(CInputMouse::LEFT) == true)
		{
			m_bPress = true;
		}
		// クリックしていなければ押していないことにする
		else
		{
			m_bPress = false;
		}
	}
	// 当たっていない場合
	else
	{
		// 触れていなくて押していない状態にする
		m_bTouch = false;
		m_bPress = false;
	}

	// 触れている状態であれば色をかえる
	if (m_bTouch == true)
	{
		pWord->SetColor(m_info.colWordEmp);
	}
	// 触れていない状態であれば色をかえる
	else if (m_bTouch == false)
	{
		pWord->SetColor(m_info.colWord);
	}

	// 押している状態であれば色をかえる
	if (m_bPress == true)
	{
		pPolygon2D->SetColor(m_info.colPolygonEmp);
	}
	// 押していない状態であれば色をかえる
	else if (m_bPress == false)
	{
		pPolygon2D->SetColor(m_info.colPolygon);
	}

	// 文字列更新
	if (pWord != nullptr)
	{
		pWord->Update();
	}

	// ポリゴン更新
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Update();
	}
}

//**************************************************
// ボタン描画
//**************************************************
void CButton::Draw(void)
{
	// ポリゴン更新
	if (pPolygon2D != nullptr)
	{
		pPolygon2D->Draw();
	}

	// 文字列更新
	if (pWord != nullptr)
	{
		pWord->Draw();
	}
}

//***************************************************************
// ボタン生成
//***************************************************************
CButton* CButton::Create(INFO info, const char* pStrWord, const char* pTexturePassPolygon, int nFontID, const char* pFontName, DWORD format)
{
	// ボタンインスタンス生成
	CButton* pTemplate = new CButton;

	if (pTemplate != nullptr)
	{
		pTemplate->Init(info,pStrWord,pTexturePassPolygon,nFontID,pFontName,format);
	}

	return pTemplate;
}