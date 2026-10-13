//===============================================
// 
// デバッグ時のルールの処理クラス [functionDebugRule.h]
// Author:Rio Ohno
// 
//===============================================
#ifndef _FUNCTIONDEBUGRULE_H_
#define _FUNCTIONDEBUGRULE_H_

// インクルード
#include "functionBase.h"

// デバッグ時のルールの処理クラス
class CFuncDebRule :public CFunctionBase
{
public:
	CFuncDebRule() {}
	~CFuncDebRule() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif // !_FUNCTIONDEBUGRULE_H_
