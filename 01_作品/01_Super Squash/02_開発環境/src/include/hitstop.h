//===================================================
//
// ヒットストップの処理 [hitstop.h]
// Author:YUTO YOSHIDA
// 
// << 更新だけ担当 >>
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _HITSTOP_H_
#define _HITSTOP_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "world_system.h"

//***************************************************
// 前方宣言
//***************************************************
class CHitStopController; // ヒットストップを操作するクラス

//***************************************************
// ヒットストップのクラスの定義
//***************************************************
class CHitStop : public CWorldSystem
{
public:
	CHitStop();
	~CHitStop();

	CHitStopController* GetController(void) { return m_pController.get(); }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	std::unique_ptr<CHitStopController> m_pController; // 操作するクラスを参照用
};
#endif