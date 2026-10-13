//===============================================
// 
// ルール縄跳びクラス[ruleSkip.h]
// Author：Rio Ohno
// 
//===============================================

#ifndef _RULESKIP_H_
#define _RULESKIP_H_

// インクルード
#include "rulebase.h"

// 前方宣言
class CObject2D;

// なわとびクラス
class CRuleSkip :public CRuleBase
{
public:

	typedef enum
	{
		UI_BASE = 0,
		UI_BOTTON,
		UI_RING_AWARD,
		UI_RING,
		UI_MAX
	}UI;

	CRuleSkip();
	~CRuleSkip();

	HRESULT Init(void);
	void Uninit(void);
	int Update(void);

	void CreateRing(void);
	void DeleteRing(void);
	void UpdateRing(void);

private:
	void Decision(void);
	void Reduction(void);

	// constexpr
	static constexpr float DEFALT_SIZE = 55.0f;	// リング発生サイズ
	static constexpr float MIN_SIZE = 30.0f;	// 判定リングの再サイズ
	static constexpr float DEVIATION = 2.5f;	// 誤差
	static constexpr int NUM = 3;				// 回数

	static CObject2D* m_apObject2D[UI_MAX];		// 2Dポリゴン
	int m_nCntFream;							// フレームカウンタ
	int m_nCntNum;								// 回数カウンタ
};
#endif // !_RULESKIP_H_