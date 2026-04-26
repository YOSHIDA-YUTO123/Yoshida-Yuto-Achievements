//===================================================
//
// カメラのアニメーションの処理 [camera_animation_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_ANIMATION_SYSTEM_H_
#define _CAMERA_ANIMATION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"

//***************************************************
// 前方宣言
//***************************************************
class CCamera; // カメラ

//***************************************************
// カメラのアニメーション処理の定義
//***************************************************
class CCameraAnimationSystem
{
public:

	// モーションの種類
	enum TYPE
	{
		TYPE_TITLE_TO_GAME = 0, // タイトルからゲームへ
		TYPE_GAME_TO_RESULT,	// ゲームからリザルトへ
		TYPE_REULT_TO_TITLE,	// リザルトからタイトルへ
		TYPE_GAME_TO_TITLE,		// ゲームからタイトルへ
		TYPE_RETRY,				// リトライ
		TYPE_CREATE,			// 生成処理
		TYPE_MAX
	};

	// 状態
	enum class State : uint8_t
	{
		Idle = 0,	// 何もしない
		Play,		// 再生
		Max
	};

	// キー
	struct Key
	{
		D3DXVECTOR3 posV;	// 視点
		D3DXVECTOR3 posR;	// 注視点
		D3DXVECTOR3 rot;	// 向き
	};

	// キー情報
	struct KeyInfo
	{
		Key key;		// キー
		int nFrame;		// フレーム
	};

	// アニメーション情報
	struct Info
	{
		std::vector<KeyInfo> aKeyInfo;	// キーの情報
		int nNumKey;						// キーの総数
		bool bLoop;							// ループするかどうか
	};

	CCameraAnimationSystem();
	~CCameraAnimationSystem();

	HRESULT Load(const char* pFilePath);

	void Play(const int nType);
	void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot);

	State GetState(void) const { return m_state; }
private:
	std::vector<Info> m_aInfo;	// 情報
	State m_state;				// 状態

	int m_nType;				// 種類
	int m_nKey;					// キー
	int m_nNextKey;				// 次のキー
	int m_nCounter;				// カウンター
};
#endif