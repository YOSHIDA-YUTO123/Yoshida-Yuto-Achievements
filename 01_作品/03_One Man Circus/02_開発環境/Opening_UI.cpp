//===================================================
//
// オープニングのUIの処理 [Opening_UI.h]
// Author:YUTO YOSHIDA
//
//===================================================


//***************************************************
// インクルードファイル
//***************************************************
#include "Opening_UI.h"

//===================================================
// コンストラクタ
//===================================================
COpening_UI::COpening_UI() : 
	m_bShow(true),
	m_fFlashCounter(NULL),
	m_col(Const::WHITE)
{
}

//===================================================
// デストラクタ
//===================================================
COpening_UI::~COpening_UI()
{
}

//===================================================
// 生成処理
//===================================================
COpening_UI* COpening_UI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& Size, const char* pTexturePath)
{
	COpening_UI* pInstance = new COpening_UI;

	pInstance->SetPosition(pos);
	pInstance->SetSize(Size);
	pInstance->SetTextureID(pTexturePath);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT COpening_UI::Init(void)
{
	// 初期化処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void COpening_UI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void COpening_UI::Update(void)
{
	if (m_bShow)
	{
		m_fFlashCounter += 0.03f;

		float fOffset = 0.8f;

		m_col.a = 1.0f - fabsf(sinf(m_fFlashCounter) * fOffset);
	}

	// 色の設定
	CObject2D::SetColor(m_col);

	// 更新処理
	CObject2D::Update();
}

//===================================================
// 描画処理
//===================================================
void COpening_UI::Draw(void)
{
	if (m_bShow)
	{
		// 描画
		CObject2D::Draw();
	}
}