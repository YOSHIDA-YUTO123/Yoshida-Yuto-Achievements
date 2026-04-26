//===================================================
//
// ヒットストップを操作するクラス [hitstop_controller.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _HITSTOP_CONTROLLER_H_
#define _HITSTOP_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// ヒットストップを操作するクラス
//***************************************************
class CHitStopController
{
public:
	CHitStopController();
	~CHitStopController();

	void Begin(const int nTime);
	void AddFinishEvent(std::function<void(void)> FinishEvent) { m_aFinishEvent.push_back(FinishEvent); }

	const bool IsStop(void) const { return m_bStop; }

	void Update(void);
private:
	std::vector<std::function<void(void)>> m_aFinishEvent;	// ヒットストップが終了したときのEvent

	int m_nTime;	 // ヒットストップの時間
	bool m_bStop;	 // 停止するかどうか
};
#endif