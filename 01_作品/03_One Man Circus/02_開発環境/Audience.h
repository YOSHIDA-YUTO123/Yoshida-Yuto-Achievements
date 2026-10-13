//==================================================
// 
// 観客 [Audience.h]
// Author：Rio Ohno
// 
//==================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_

// インクルード
#include "main.h"
#include "billboard.h"

// マクロ定義
#define AUDIENCE_TEX_PASS ((const char*)"Audience_Normal.png")	// テクスチャパス
#define AUDIENCE_GRAVITY ((float)0.3f)							// 観客の重力
#define AUDIENCE_SUCANDMIS (120)								// 観客のミスした時と成功した時の反応フレーム
#define AUDIENCE_NONE (180)										// 観客の通常状態

// 観客のクラス
class CAudience :public CObjectBillboard
{
public:
 
	typedef enum
	{
		ANIMETYPE_NONE,			// 通常
		ANIMETYPE_SUCCESS,		// 成功
		ANIMETYPE_MISTAKE,		// 失敗
		ANIMETYPE_MAX
	}ANIMETYPE;// アニメーションの種類

	CAudience();
	~CAudience();

	static CAudience* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,float fJump,float fSpeed);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAnimation(ANIMETYPE animetype);

	static void SetMiss(bool bMiss) { m_bMiss = bMiss; };
	static void SetSuccess(bool bSuccess) { m_bSuccess = bSuccess; };

private:
	void Animation(void);						// アニメーション
	void isJump(void);							// ジャンプアニメーション
	void Miss(void);
	void SearchSkill(void);

	ANIMETYPE m_Animetype;						// アニメーションの種類
	D3DXVECTOR3 m_move;							// 移動値
	int m_nCntFream;							// フレームカウンタ
	int m_nCntReaction;							// フレームカウンタ
	float m_fJump;								// ジャンプ量
	float m_fSpeed;								// 移動スピード
	float m_fBasePosY;							// 基準の高さ
	bool m_bJump;								// ジャンプするかどうか(してるかどうかじゃないよ)

	static bool m_bMiss;
	static bool m_bSuccess;
};

#endif