//===================================================
//
// ランキングのスコアのクラス [RankingScore.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RANKING_SCORE_H_
#define _RANKING_SCORE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include <array>
#include <memory>

//***************************************************
// 前方宣言
//***************************************************
class CNumber; // ナンバークラス

//***************************************************
// ランキングのスコアのクラスの定義
//***************************************************
class CRankingScore : public CObject
{
public:
	CRankingScore();
	~CRankingScore();

	static CRankingScore* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nScore, const bool bFlash);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	static constexpr int MAX_DIGIT = 8;							// 最大の桁数

	std::array<std::unique_ptr<CNumber>, MAX_DIGIT> m_apNumber; // ナンバークラスへのポインタ
	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR2 m_size;											// 大きさ
	float m_fFlashCounter;										// 点滅カウンター
	int m_nScore;												// スコア
	bool m_bFlash;												// 強調表示するかどうか
};
#endif