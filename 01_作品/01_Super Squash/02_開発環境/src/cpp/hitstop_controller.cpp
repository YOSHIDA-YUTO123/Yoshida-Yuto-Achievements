//===================================================
//
// ヒットストップを操作するクラス [hitstop_controller.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "hitstop_controller.h"

//***************************************************
// 定数宣言
//***************************************************
namespace HitStopConst
{
	constexpr int INVALID_TIME = -1;	// 無効なタイマーの値
}

//===================================================
// コンストラクタ
//===================================================
CHitStopController::CHitStopController() :
	m_aFinishEvent(),
	m_nTime(HitStopConst::INVALID_TIME),
	m_bStop(false)
{
}

//===================================================
// デストラクタ
//===================================================
CHitStopController::~CHitStopController()
{
}

//===================================================
// ヒットストップの開始
//===================================================
void CHitStopController::Begin(const int nTime)
{
	if (m_nTime <= 0)
	{
		m_nTime = nTime;
	}
}

//===================================================
// 更新処理
//===================================================
void CHitStopController::Update(void)
{
	// 開始していたら
	if (m_nTime >= 0)
	{
		m_nTime--;

		// 開始
		m_bStop = true;
	}
	else if(m_bStop)
	{
		// イベント分回す
		for (auto& event : m_aFinishEvent)
		{
			// イベントを発火
			event();
		}

		// 要素があるなら
		if (!m_aFinishEvent.empty())
		{
			// イベントを破棄
			m_aFinishEvent.clear();
		}

		// 停止
		m_bStop = false;
	}
}