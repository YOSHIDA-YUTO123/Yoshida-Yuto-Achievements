//===================================================
//
// リザルトの処理 [FunctionResult.h]
// Author:YUTO YOSHIDA
//
//===================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _FUNCTIONGAMEEND_H_
#define _FUNCTIONGAMEEND_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "functionBase.h"

//***************************************************
// リザルトの処理のクラスの定義
//***************************************************
class CFunctionGameEnd : public CFunctionBase
{
public:
	CFunctionGameEnd();
	~CFunctionGameEnd() = default;

	void Init(void) override {};
	void Uninit(void) override {};
	void Update(void) override;
private:
	int m_nEffectCounter; // エフェクトのカウンター
};
#endif