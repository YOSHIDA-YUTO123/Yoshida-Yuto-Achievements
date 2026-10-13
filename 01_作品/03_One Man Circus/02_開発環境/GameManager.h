//================================================
//
// ゲームのマネージャークラス [GameManager.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//*************************************************
// インクルードファイル
//*************************************************

//*************************************************
// ゲームのマネージャークラスの定義
//*************************************************
class CGameManager
{
public:
	static constexpr const char* SAVE_TIMER_FILE = "data/TEXT/NowTime.txt";		// タイマーをセーブするファイルのパス
	static constexpr const char* SAVE_SCORE_FILE = "data/TEXT/NowScore.txt";	// ランキングをセーブするファイルのパス

	CGameManager();
	~CGameManager();

	void SaveScore(const char* pFilePath, const int nScore);
	void SaveTime(const char* pFilePath, const int nTime);

	void Update(void);

	// 現在のタイマーの取得
	void SetCurrentTime(const int nTime) { m_nTime = nTime; }
private:
	int m_nScore;			// スコア
	int m_nTime;			// タイマー
	bool m_bSetLastTimer;	// 制限時間目前のタイマーを表示したかどうか
};
#endif