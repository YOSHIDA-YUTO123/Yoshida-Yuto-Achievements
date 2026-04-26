//===================================================
//
// ミニゲームの補助 [minigame_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MINIGAME_MANAGER_H_
#define _MINIGAME_MANAGER_H_

//***************************************************
// 前方宣言
//***************************************************
struct ScoreAreaComponent; // エリアのコンポーネントの取得

//***************************************************
// ミニゲームのマネージャークラスの定義
//***************************************************
class CMiniGameManager
{
public:

	static constexpr int TIMER_NO_FIND = 99999999; // タイマーが見つからない

	// ミニゲームの状態
	enum class State : uint8_t
	{
		Normal = 0, // 通常の状態
		Tutorial,	// チュートリアル状態
		Max
	};

	explicit CMiniGameManager(entt::registry& registry);
	~CMiniGameManager();

	static std::unique_ptr<CMiniGameManager> Create(entt::registry& registry);

	int GetTimer(void);
	int GetScore(void) const { return m_nScore; }
	int GetPlayerComp(void) const { return m_nComb; }
	void SetScore(const int nScore) { m_nScore = nScore; }

	void Update(void);
private:
	void SaveInfo(void);
	void SaveRanking(void);
private:
	entt::registry& m_registry;		// 参照用レジストリ
	State m_state;					// 状態
	int m_nScore;					// スコア
	int m_nComb;					// コンボ数
	bool m_bChangeScene;			// シーン変更したかどうか
};
#endif