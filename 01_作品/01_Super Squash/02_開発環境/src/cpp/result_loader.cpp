//===================================================
//
// リザルトのロード処理 [result_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "result_loader.h"
#include "fstream"
#include "loader_constants.hpp"

//===================================================
// スコアのロード処理
//===================================================
int Loader::Result::GetScore(void)
{
	// 名前空間の使用
	using namespace LoaderConst;

	// バイナリ
	std::fstream file(SCORE_PATH, std::ios::binary | std::ios::in);

	// ファイルが開けなかったら
	if (!file.is_open())
	{
		MessageBox(NULL, "ファイルが開けません", SCORE_PATH, MB_OK);
		return -1;
	}

	int nScore = -1;

	// ファイルの最後まで続ける
	while (!file.eof())
	{
		// 読み込み
		file.read(reinterpret_cast<char*>(&nScore), sizeof(nScore));
	}

	file.clear();
	file.close();

	return nScore;
}

//===================================================
// ミニゲームのランキングの取得
//===================================================
HRESULT Loader::Result::MinigameRanking(std::array<int, RankingComponent::MAX_RANK>& ranking)
{
	// 名前空間の使用
	using namespace LoaderConst;

	// ファイルを開く
	std::fstream file(RANKING_PATH, std::ios::binary | std::ios::in);

	// ファイルが開けないなら
	if (!file.is_open())
	{
		MessageBox(NULL, "ファイルが開けません", RANKING_PATH, MB_OK);
		return E_FAIL;
	}

	// 値を読み込む
	file.read(reinterpret_cast<char*>(&ranking), sizeof(ranking));
	file.close();
	file.clear();

	return S_OK;
}
