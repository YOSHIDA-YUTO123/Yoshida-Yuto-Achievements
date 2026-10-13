//===============================================
// 
// ルールバランスクラス[ruleBalance.cpp]
// Author：Rio Ohno
// 
//===============================================

// インクルード
#include "ruleBalance.h"
#include "NormalTimer.h"
#include "object2DAnim.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"

// 静的メンバ変数
CNormalTimer* CRuleBalance::m_pTime = nullptr;
CObject2DAnim* CRuleBalance::m_pObject2DAnim = nullptr;
CObject2D* CRuleBalance::m_apObject2D[UI_MAX] = { nullptr };

//===============================================
// コンストラクタ
//===============================================
CRuleBalance::CRuleBalance() :CRuleBase(CRuleBase::RULETYPE_BALANCE)
{
	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;	// オブジェクト2Dのポインタ
	}

	m_pTime = nullptr;					// タイマーのポインタ
	m_pObject2DAnim = nullptr;			// アニメーション付きのポインタ
}

//===============================================
// デストラクタ
//===============================================
CRuleBalance::~CRuleBalance()
{
	// なし
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRuleBalance::Init(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// タイマーの設定
	m_pTime = CNormalTimer::Create(D3DXVECTOR3(780.0f, 180.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), 10);

	// ポリゴンの設定
	m_apObject2D[UI_BASE] = CObject2D::Create(60.0f, 60.0f, D3DXVECTOR3(760.0f, 270.0f, 0.0f));

	// テクスチャの設定
	m_apObject2D[UI_BASE]->SetTextureID("ButtonUI_Juggling_Base.png");

	if (pJoypad->GetLastInput() <= pKeyboard->GetLastPress())
	{
		// ポリゴンの設定
		m_apObject2D[UI_RING] = CObject2D::Create(35.0f, 35.0f, D3DXVECTOR3(760.0f, 270.0f, 0.0f));
		m_pObject2DAnim = CObject2DAnim::Create(D3DXVECTOR3(760.0f, 270.0f, 0.0f), D3DXVECTOR2(90.0f, 30.0f), 4, 1);

		// テクスチャの設定
		m_apObject2D[UI_RING]->SetTextureID("ButtonUI_Timing_Circle.png");
		m_pObject2DAnim->SetTextureID("Command_UI_Stick01Anim.png");

		// アニメーションの設定
		m_pObject2DAnim->SetAnim(true, ANIM_SPANFREAM);

	}
	else if (pJoypad->GetLastInput() > pKeyboard->GetLastPress())
	{
		// ポリゴンの設定
		m_pObject2DAnim = CObject2DAnim::Create(D3DXVECTOR3(760.0f, 260.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), 2, 1);

		// テクスチャの設定
		m_pObject2DAnim->SetTextureID("WASD.png");

		// アニメーションの設定
		m_pObject2DAnim->SetAnim(true, ANIM_SPANFREAM);
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRuleBalance::Uninit(void)
{
	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			// 終了処理
			m_apObject2D[nCnt]->Uninit();

			m_apObject2D[nCnt] = nullptr;
		}
	}

	if (m_pTime != nullptr)
	{
		// 終了処理
		m_pTime->Uninit();

		m_pTime = nullptr;
	}

	if (m_pObject2DAnim != nullptr)
	{
		// 終了処理
		m_pObject2DAnim->Uninit();

		m_pObject2DAnim = nullptr;
	}

	// 自身の破棄フラグ
	CRuleBase::Release();
}

//===============================================
// 更新処理
//===============================================
int CRuleBalance::Update(void)
{
	// 基底クラスの更新
	CRuleBase::Update();

	return CRuleBase::GetResult();
}