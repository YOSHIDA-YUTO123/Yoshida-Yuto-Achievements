//===================================================
//
// オープニングのプレイヤークラス [OpeningPlayer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OPENING_PLAYER_H_
#define _OPENING_PLAYER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "ScenePlayer.h"

//***************************************************
// 前方宣言
//***************************************************
class CMotion; // モーション

//***************************************************
// オープニングのシーンのプレイヤークラスの定義
//***************************************************
class COpeningPlayer : public CScenePlayer
{
public:

	// ムービーのフラグ
	enum MOVIE_FALG
	{
		MOVIE_FLAG_START			= 1 << 0,	// 開始
		MOVIE_FLAG_MOVE_END			= 1 << 1,	// 移動終わり
		MOVIE_FLAG_WAVE_HAND_END	= 1 << 2,	// 手を振る終わり
		MOVIE_FLAG_BOW_END			= 1 << 3,	// お辞儀終わり
		MOVIE_FLAG_BALL_TO_MOVE_END	= 1 << 4,	// ボールまで移動終わり
		MOVIE_FLAG_LANDING			= 1 << 5,	// 着地
		MOVIE_FLAG_BALANCE			= 1 << 6,	// 傾かせる

		MOVIE_FALG_MAX
	};

	COpeningPlayer();
	~COpeningPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void UpdateLight(CCharacter3D* pCharacter);
	void Landing(D3DXVECTOR3* pPos);
	void FlagStart(CMotion* pMotion);
	void FlagMoveEnd(CMotion* pMotion);
	void FlagWaveHandEnd(CMotion* pMotion);
	void FlagBowEnd(CMotion* pMotion);
	void FlagBallToMoveEnd(CMotion* pMotion);
	void FlagLanding(void);
	void FlagBalance(void);
	void MoveSmoke(CMotion* pMotion, CCharacter3D* pCharacter);
private:
	D3DXVECTOR3 m_move;		// 移動量
	unsigned int m_nFlag;	// フラグ
	float m_fGravity;		// 重力量
	int m_nJugTime;			// ジャグリングのカウンター
	int m_nJumpCount;		// ジャンプのカウンター
	int m_nMoveCount;		// 移動カウンター
	int m_nWaveHandCount;	// 手をふるモーションのカウンター
	int m_nBowCount;		// お辞儀モーションのカウンター
	int m_nLightID;			// ライトのID
	int m_nCnt;
	bool m_bJump;			// ジャンプできるかどうか
};
#endif