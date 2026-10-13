//===============================================
// 
// ルールフラフープクラス[rulehula.h]
// Author：Rio Ohno
// 
//===============================================
#ifndef _RULEHULA_H_
#define _RULEHULA_H_

// インクルード
#include "rulebase.h"
#include "NormalTimer.h"

// 前方宣言
class CObject2D;
class CObject2DAnim;

// フラフープクラス
class CRuleHula:public CRuleBase
{
public:

	typedef enum
	{
		UI_BASE = 0,
		UI_MAX
	}UI;

	CRuleHula();
	~CRuleHula();

	HRESULT Init(void);
	void Uninit(void);
	int Update(void);

	// constexpr
	static constexpr int ANIM_SPANFREAM = 5;

private:
	static CNormalTimer* m_pTime;
	static CObject2D* m_apObject2D[UI_MAX];
	static CObject2DAnim* m_pObject2DAnim;
	int m_nCntAnimFream;
};

#endif // !_RULEHULA_H_
