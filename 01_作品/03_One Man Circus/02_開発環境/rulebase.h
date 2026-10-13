//===============================================
// 
// ルールクラス(基底クラス)[rulebase.h]
// Author：Rio Ohno
// 
//===============================================
#ifndef _RULEBASE_H_
#define _RULEBASE_H_

// ルール基底クラス
class CRuleBase
{
public:

	// 結果の列挙
	typedef enum
	{
		RESULT_NONE = -1,	// 何もなし
		RESULT_LIMIT,		// 時間切れ
		RESULT_PLAYER_MISS,	// プレイヤーのミス
		RESULT_COMMAND_MISS,// コマンドのミス
		RESULT_SUCCESS,		// 成功
		RESULT_MAX
	}RESULT;

	// コマンド（ルール）の種類の列挙
	typedef enum
	{
		RULETYPE_NONE = 0,
		RULETYPE_JUG,		// ジャグリング
		RULETYPE_HULA,		// フラフープ
		RULETYPE_SKIP,		// 縄跳び
		RULETYPE_BALANCE,	// バランスのみ
		RULETYPE_MAX
	}RULETYPE;

	CRuleBase(RULETYPE Ruletype);
	virtual ~CRuleBase();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual int Update(void);

	// セッター
	void SetResult(RESULT result) { m_nResult = (int)result; }
	void SetResult(int result) { m_nResult = result; }

	// ゲッター
	int GetResult(void) { return m_nResult; }
	int GetFream(void) { return m_nCntFream; }
	RULETYPE GetRuleType(void) { return m_Ruletype; }

	void Release(void) { m_bDeth = true; }

	// constexper
	static const int GIMMICK_FREAM = 600;

private:
	int m_nResult;			// 結果
	int m_nCntFream;		// フレームカウンタ
	RULETYPE m_Ruletype;	// 種類
	bool m_bDeth;			// 消すフラグ
};

#endif // !_RULEBASE_H_