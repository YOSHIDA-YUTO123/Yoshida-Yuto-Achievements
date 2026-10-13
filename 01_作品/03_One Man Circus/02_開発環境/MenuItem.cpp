//===================================================
//
// メニューの項目クラス [MenuItem.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MenuItem.h"
#include "MenuVisualBase.h"

//===================================================
// コンストラクタ
//===================================================
CMenuItem::CMenuItem() : 
	CObject2D(PRIORITY_UI_FRONT),
	m_bSelect(false),
	m_col(Const::WHITE),
	m_fScal(1.0f),
	m_pMenuVisual(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CMenuItem::~CMenuItem()
{
}

//===================================================
// 生成処理
//===================================================
CMenuItem* CMenuItem::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& Size, const D3DXCOLOR& col, const char* pTexturePath, std::unique_ptr<CMenuVisualBase> pMenuVisual, const float fScal)
{
	CMenuItem* pInstance = new CMenuItem;

	pInstance->SetPosition(pos);
	pInstance->SetSize(Size);
	pInstance->m_fScal = fScal;
	pInstance->m_col = col;
	pInstance->SetTextureID(pTexturePath);
	pInstance->m_pMenuVisual = std::move(pMenuVisual);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMenuItem::Init(void)
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
void CMenuItem::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CMenuItem::Update(void)
{
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	D3DXVECTOR2 Size = CObject2D::GetSize();

	if (m_pMenuVisual != nullptr)
	{
		// 更新処理
		m_pMenuVisual->Update(m_bSelect, pos, Size, m_col, m_fScal);
	}

	CObject2D::SetPosition(pos);
	CObject2D::SetSize(Size);
	CObject2D::SetColor(m_col);

	// 更新処理
	CObject2D::Update();
}

//===================================================
// 描画処理
//===================================================
void CMenuItem::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
