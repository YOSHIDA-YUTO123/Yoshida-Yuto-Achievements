//===============================================
// 
// ルールバランスクラス[ruleBalance.h]
// Author：Rio Ohno
// 
//===============================================
#ifndef _RULEBALANCE_H_
#define _RULEBALANCE_H_

// インクルード
#include "rulebase.h"

// 前方宣言
class CNormalTimer;
class CObject2D;
class CObject2DAnim;

// ルールバランスクラス
class CRuleBalance :public CRuleBase
{
public:

	typedef enum
	{
		UI_BASE = 0,
		UI_RING,
		UI_MAX
	}UI;

	CRuleBalance();
	~CRuleBalance();

	HRESULT Init(void);
	void Uninit(void);
	int Update(void);

private:

	// constexpr
	static constexpr int ANIM_SPANFREAM = 30;

	static CNormalTimer* m_pTime;
	static CObject2DAnim* m_pObject2DAnim;
	static CObject2D* m_apObject2D[UI_MAX];
};
#endif