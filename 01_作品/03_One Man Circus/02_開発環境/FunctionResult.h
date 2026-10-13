//===================================================
//
// リザルトの処理 [FunctionResult.h]
// Author:YUTO YOSHIDA
//
//===================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _FUNCTION_RESULT_H_
#define _FUNCTION_RESULT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "functionBase.h"

//***************************************************
// リザルトの処理のクラスの定義
//***************************************************
class CFunctionResult : public CFunctionBase
{
public:
	CFunctionResult();
	~CFunctionResult() = default;

	void Init(void) override {};
	void Uninit(void) override {};
	void Update(void) override;
private:
	int m_nEffectCounter; // エフェクトのカウンター
};
#endif