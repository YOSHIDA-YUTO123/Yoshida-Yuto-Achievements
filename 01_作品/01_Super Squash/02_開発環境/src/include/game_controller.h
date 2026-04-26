//===================================================
//
// ゲーム、ミニゲームの全体の操作をするクラス [game_controller.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CHitStop;				// ヒットストップ
class CHitStopController;	// ヒットストップを操作するクラス
class CCameraShake;			// カメラの揺れの処理

//***************************************************
// ゲーム、ミニゲームを操作するクラス
//***************************************************
class CGameController
{
public:
	~CGameController();

	static void Create(void);
	static CGameController* GetInstance(void);

	CHitStopController* GetHitStop(void);
	CCameraShake* GetCameraShake(void) { return m_pCameraShack; }

	void RegisterCameraShake(CCameraShake* pCameraShack) { m_pCameraShack = pCameraShack; }
	void RegisterHitStop(CHitStop* pHitStop) { m_pHitStop = pHitStop; }
	void Uninit(void);
	void Update(void);
	void SetPause(void);
	void SetPause(const bool bPause) { m_bPause = bPause; }
	bool GetPause(void) const { return m_bPause; }
private:
	CGameController();
private:
	static std::unique_ptr<CGameController> m_pInstance;	// 自分のインスタンス
	CHitStop* m_pHitStop;									// ヒットストップを操作するクラス
	CCameraShake* m_pCameraShack;							// カメラの揺れのクラス
	bool m_bPause;											// ポーズ中かどうか
};
#endif