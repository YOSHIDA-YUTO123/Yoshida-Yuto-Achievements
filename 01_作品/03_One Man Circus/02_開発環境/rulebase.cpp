//===============================================
// 
// ルールクラス(基底クラス)[rulebase.cpp]
// Author：Rio Ohno
// 
//===============================================

// インクルード
#include "rulebase.h"
#include "score.h"
#include "game.h"
#include "Audience.h"
#include "sound.h"
#include "manager.h"

//===============================================
// コンストラクタ
//===============================================
CRuleBase::CRuleBase(RULETYPE Ruletype)
{
	// 各メンバ変数の初期化
	m_Ruletype = Ruletype;
	m_nCntFream = 0;
	m_nResult = -1;
	m_bDeth = false;
}

//===============================================
// デストラクタ
//===============================================
CRuleBase::~CRuleBase()
{
	// なし
}

//===============================================
// 更新処理
//===============================================
int CRuleBase::Update(void)
{
	// フレームカウント
	++m_nCntFream;

	if (m_nCntFream >= GIMMICK_FREAM)// 時間切れになったら
	{
		// フレームカウントリセット
		m_nCntFream = 0;

		// 結果を時間切れにする
		m_nResult = RESULT_LIMIT;

		// スコア加算
		CGame::GetScore()->AddScore(100000);
		if (m_Ruletype == RULETYPE_HULA)
		{
			CGame::GetScore()->AddScore(50000);
		}
		else if (m_Ruletype == RULETYPE_JUG)
		{
			CGame::GetScore()->AddScore(30000);
		}
		CAudience::SetSuccess(true);

		CSound* pSound = CManager::GetSound();
		if (pSound != nullptr)
		{
			pSound->Play(CSound::SOUND_LABEL_SE_Success);
		}
	}

	return m_nResult;
}
