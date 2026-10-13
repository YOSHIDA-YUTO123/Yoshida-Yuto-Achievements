//===================================================
//
// リザルトシーン [result.h]
// Author:YUTO YOSHIDA
//
//===================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "scene.h"
#include <memory>
#include <vector>
#include "DualUnitTimer.h"

//***************************************************
// 前方宣言
//***************************************************
class CFunctionBase; // システムクラス

//***************************************************
// リザルト(勝利)クラスの定義
//***************************************************
class CResultWin : public CScene
{
public:

	CResultWin();
	~CResultWin();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int LoadScore(void);
	static CDualUnitTimer* GetTimer(void) { return m_pTimer; }
private:
	void AddFunction(std::unique_ptr<CFunctionBase> pFunction);
	static CDualUnitTimer* m_pTimer;	// タイマーへのポインタ
private:
	std::vector<std::unique_ptr<CFunctionBase>> m_vpFunction;		// 機能ポインタ
};

//***************************************************
// リザルト(敗北)クラスの定義
//***************************************************
class CResultLose : public CScene
{
public:

	CResultLose();
	~CResultLose();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};
#endif