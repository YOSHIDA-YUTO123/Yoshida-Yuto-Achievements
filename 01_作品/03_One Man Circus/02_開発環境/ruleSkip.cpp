//===============================================
// 
// ルール縄跳びクラス[ruleSkip.cpp]
// Author：Rio Ohno
// 
//===============================================

// インクルード
#include "ruleSkip.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"

// 静的メンバ変数
CObject2D* CRuleSkip::m_apObject2D[UI_MAX] = { nullptr };

//===============================================
// コンストラクタ
//===============================================
CRuleSkip::CRuleSkip():CRuleBase(CRuleBase::RULETYPE_SKIP)
{
	// メンバ変数の初期化
	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;	// 2Dポリゴンへのポインタ
	}

	m_nCntNum = 0;		// 回数カウンタ
	m_nCntFream = 0;	// フレームカウンタ
}

//===============================================
// デストラクタ
//===============================================
CRuleSkip::~CRuleSkip()
{
	// なし
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRuleSkip::Init(void)
{
	//// キーボードの取得
	//CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	//// ジョイパッドの取得
	//CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// ポリゴンの設定
	m_apObject2D[UI_BASE] = CObject2D::Create(60.0f, 60.0f, D3DXVECTOR3(760.0f, 280.0f, 0.0f));
	m_apObject2D[UI_RING_AWARD] = CObject2D::Create(MIN_SIZE, MIN_SIZE, D3DXVECTOR3(760.0f, 280.0f, 0.0f));
	CreateRing();

	// テクスチャの設定
	m_apObject2D[UI_BASE]->SetTextureID("ButtonUI_Juggling_Base.png");
	m_apObject2D[UI_RING_AWARD]->SetTextureID("ButtonUI_Timing_Circle.png");

	// 色の設定
	m_apObject2D[UI_RING_AWARD]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	//if (pJoypad->GetLastInput() <= pKeyboard->GetLastPress())
	//{
		m_apObject2D[UI_BOTTON] = CObject2D::Create(MIN_SIZE - 5.0f, MIN_SIZE - 5.0f, D3DXVECTOR3(760.0f, 280.0f, 0.0f));
		m_apObject2D[UI_BOTTON]->SetTextureID("ButtonUI_Juggling_JoyPad_A.png");
	//}
	//else if (pJoypad->GetLastInput() > pKeyboard->GetLastPress())
	//{
	//}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRuleSkip::Uninit(void)
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

	// 自身の破棄
	CRuleBase::Release();
}

//===============================================
// 更新処理
//===============================================
int CRuleSkip::Update(void)
{
	// ベースの更新
	CRuleBase::Update();

	// リングの更新処理
	UpdateRing();

	// 結果を返す
	return CRuleBase::GetResult();
}

//===============================================
// リングの生成処理
//===============================================
void CRuleSkip::CreateRing(void)
{
	// ポリゴンの生成
	m_apObject2D[UI_RING] = CObject2D::Create(DEFALT_SIZE, DEFALT_SIZE, D3DXVECTOR3(760.0f, 280.0f, 0.0f));

	// テクスチャの設定
	m_apObject2D[UI_RING]->SetTextureID("ButtonUI_Timing_Circle.png");

	// 回数カウントアップ
	++m_nCntNum;
}

//===============================================
// リングの消去処理
//===============================================
void CRuleSkip::DeleteRing(void)
{
	// 終了処理
	m_apObject2D[UI_RING]->Uninit();

	m_apObject2D[UI_RING] = nullptr;
}

//===============================================
// リングの更新処理
//===============================================
void CRuleSkip::UpdateRing(void)
{
	if (m_apObject2D[UI_RING] != nullptr)// リングが存在するなら
	{
		// 判定処理
		Decision();
	}
	else
	{
		if (CRuleBase::GetResult() == CRuleBase::RESULT_NONE)
		{
			int nTiming = CRuleBase::GetFream() % (GIMMICK_FREAM / NUM);

			if (nTiming == 0)
			{
				// リングの生成
				CreateRing();
			}
		}
	}
}

//===============================================
// 判定処理
//===============================================
void CRuleSkip::Decision(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetTrigger(DIK_SPACE) ||
		pJoypad->GetTrigger(CInputJoypad::JOYKEY_A))
	{
		D3DXVECTOR2 size = m_apObject2D[UI_RING]->GetSize();

		if (size.x >= MIN_SIZE - DEVIATION && size.x <= MIN_SIZE + DEVIATION)// 判定範囲内なら
		{
			// リングを消す
			DeleteRing();

			if (m_nCntNum >= NUM)
			{
				// 成功判定
				CRuleBase::SetResult(CRuleBase::RESULT_SUCCESS);

				// 終了処理
				Uninit();
			}
			return;
		}
		else
		{
			// コマンドミス判定
			CRuleBase::SetResult(CRuleBase::RESULT_COMMAND_MISS);

			// リングを消す
			DeleteRing();

			// 終了処理
			Uninit();

			return;
		}
	}
	else
	{
		if ((CRuleBase::GetFream() % (GIMMICK_FREAM / NUM)) == 0)
		{
			// コマンドミス判定
			CRuleBase::SetResult(CRuleBase::RESULT_COMMAND_MISS);

			// リングを消す
			DeleteRing();

			// 終了処理
			Uninit();

			return;
		}
	}

	// リングを縮小する処理
	Reduction();
}

//===============================================
// 縮小処理
//===============================================
void CRuleSkip::Reduction(void)
{
	if (m_apObject2D[UI_RING] != nullptr)
	{
		float difSize = (DEFALT_SIZE + (DEVIATION * 1.5f) - MIN_SIZE) / (GIMMICK_FREAM / NUM);

		difSize *= CRuleBase::GetFream() % (GIMMICK_FREAM / NUM);

		m_apObject2D[UI_RING]->SetSize(D3DXVECTOR2(DEFALT_SIZE - difSize, DEFALT_SIZE - difSize));
	}
}