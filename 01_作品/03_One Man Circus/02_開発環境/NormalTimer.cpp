//================================================
//
// 普通のタイマーを表示する処理 [NormalTimer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "NormalTimer.h"

//*************************************************
// 名前空間
//*************************************************
using namespace Const; // 名前空間Constの使用

//=================================================
// コンストラクタ
//=================================================
CNormalTimer::CNormalTimer()
{
	m_nCounter = NULL;
}

//=================================================
// デストラクタ
//=================================================
CNormalTimer::~CNormalTimer()
{
}

//=================================================
// 生成処理
//=================================================
CNormalTimer* CNormalTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nTime)
{
	CNormalTimer* pTimer = new CNormalTimer;

	pTimer->SetPosition(pos);
	pTimer->SetSize(Size);
	pTimer->SetTime(nTime);

	// 初期化処理
	if (FAILED(pTimer->Init()))
	{
		pTimer->Uninit();
		pTimer = nullptr;
		return nullptr;
	}

	return pTimer;
}

//=================================================
// 初期化処理
//=================================================
HRESULT CNormalTimer::Init(void)
{
	// 初期化処理
	if (FAILED(CTimer::Init()))
	{
		return E_FAIL;
	}

	// テクスチャの設定
	CTimer::SetTexture("Number001.png");

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CNormalTimer::Uninit(void)
{
	// 終了処理
	CTimer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CNormalTimer::Update(void)
{
	m_nCounter++;

	// 一秒経過したら
	if (m_nCounter >= FRAME)
	{
		// 現在の時間の取得
		int nTime = CTimer::GetTime();

		nTime--;

		// 範囲制限
		if (nTime <= 0)
		{
			nTime = 0;
		}

		// 現在の時間の設定
		CTimer::SetTime(nTime);

		m_nCounter = 0;
	}

	// 更新処理
	CTimer::Update();
}

//=================================================
// 描画処理
//=================================================
void CNormalTimer::Draw(void)
{
	// 描画処理
	CTimer::Draw();
}
