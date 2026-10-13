//================================================
//
// 制限時間目前で表示するタイマー[LastTimer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "LastTimer.h"
#include "NormalTimer.h"
#include "number.h"

//=================================================
// コンストラクタ
//=================================================
CLastTimer::CLastTimer() : 
	m_pTimer(nullptr),
	m_col(Const::WHITE)
{
}

//=================================================
// デストラクタ
//=================================================
CLastTimer::~CLastTimer()
{
}

//=================================================
// 生成処理
//=================================================
CLastTimer* CLastTimer::Create(const D3DXVECTOR2& Size, const int nTime)
{
	CLastTimer* pInstance = new CLastTimer;

	// タイマーの生成
	pInstance->m_pTimer = CNormalTimer::Create({ 800.0f,360.0f,0.0f }, Size, nTime);

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

//=================================================
// 初期化処理
//=================================================
HRESULT CLastTimer::Init(void)
{
	if (m_pTimer == nullptr)
	{
		return E_FAIL;
	}

	m_col.a = 0.1f;

	// 桁数分回す
	for (int nCnt = 0; nCnt < CTimer::MAX_DIGIT; nCnt++)
	{
		// ナンバーの取得
		CNumber *pNumber = m_pTimer->GetNumber(nCnt);

		if (pNumber != nullptr)
		{
			// 色の設定
			pNumber->SetColor(m_col);
		}
	}

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CLastTimer::Uninit(void)
{
	m_pTimer = nullptr;

	// 自分自身の破棄
	CObject::Release();
}

//=================================================
// 更新処理
//=================================================
void CLastTimer::Update(void)
{
	//// 桁数分回す
	//for (int nCnt = 0; nCnt < CTimer::MAX_DIGIT; nCnt++)
	//{
	//	// ナンバーの取得
	//	m_pTimer->GetNumber(nCnt);
	//}
}

//=================================================
// 描画処理
//=================================================
void CLastTimer::Draw(void)
{
}
