//===============================================
// 
// ルールジャグリングクラス[ruleJuggling.h]
// Author：Rio Ohno
// 
//===============================================

#ifndef _RULEJUGGLING_H_
#define _RULEJUGGLING_H_

// インクルード
#include "rulebase.h"
#include "object2D.h"
#include "NormalTimer.h"

// ジャグリングクラス
class CRuleJug : public CRuleBase
{
public:

	typedef enum
	{
		BOTTON_Y = 0,
		BOTTON_X,
		BOTTON_A,
		BOTTON_B,
		BOTTON_MAX
	}BOTTON;

	typedef enum
	{
		UI_BASE = 0,
		UI_Y,
		UI_X,
		UI_A,
		UI_B,
		UI_MAX
	}UI;

	CRuleJug();
	~CRuleJug();

	HRESULT Init(void);
	void Uninit(void);
	int Update(void);

private:

	// constexpr
	static constexpr int NUM_BOTTON = 3;
	static constexpr float ENLARGED_SIZE = 18.75f;

	bool CommandInput(void);			// コマンド入力

	static CNormalTimer* m_pTime;
	static CObject2D* m_apObject2D[UI_MAX];	// オブジェクト2Dへのポインタ
	int m_anBottonList[NUM_BOTTON];		// コマンドリスト
	int m_nCommandIndx;					// コマンドインデックス
};
#endif