//================================================
//
// スコアを表示する処理 [score.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "object.h"
#include <memory>

//*************************************************
// 前方宣言
//*************************************************
class CNumber;		// ナンバー
class CGameManager; // ゲームマネージャー

//*************************************************
// スコアクラスの定義
//*************************************************
class CScore : public CObject
{
public:
	CScore();
	~CScore();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="大きさ(全体の大きさ)"></param>
	/// <param name="スコア"></param>
	/// <param name="0を表示するかどうか"></param>
	/// <returns>自分のインスタンス</returns>
	static CScore* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nScore, const bool bShowZero = false, CGameManager* pGameManager = nullptr);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	int GetScore(void) const { return m_nScore; }
	void AddScore(const int nAdd) { m_nScore += nAdd; }
	void SetScore(int nScore) { m_nScore = nScore; }
	static int Load(const char* pFilename);
	void Save(const char* pFilename);

private:
	static constexpr int MAX_DIGIT = 8; // スコアの桁数

	std::unique_ptr<CNumber> m_apNumber[MAX_DIGIT];		// ナンバークラスへのポインタ
	CGameManager* m_pGameManagerRef;					// 参照用ゲームマネージャー
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR2 m_Size;									// 大きさ
	int m_nScore;										// スコア
	int m_nDigit;										// 桁数
	bool m_bShowZero;									// いらない0が見えるかどうか
};

#endif