//===================================================
//
// カメラのアニメーションの処理 [CameraAnimationSystem.h]
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
#include <vector>

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
	};

	// キー情報
	struct Key_Info
	{
		Key key;		// キー
		int nFrame;		// フレーム
	};

	// アニメーション情報
	struct Info
	{
		std::vector<Key_Info> aKey_Info;	// キーの情報
		int nNumKey;						// キーの総数
		bool bLoop;							// ループするかどうか
	};

	CCameraAnimationSystem() = default;
	CCameraAnimationSystem(CCamera* pCamera);
	~CCameraAnimationSystem();

	void Play(const int nType);

	HRESULT Init(const char* pFilePath);
	void Uninit(void);
	void Update(void);

private:
	HRESULT Load(const char* pFilePath);
private:
	CCamera* m_pCameraRef;		// カメラクラスへのポインタ(参照用)
	std::vector<Info> m_aInfo;	// 情報
	State m_state;				// 状態

	int m_nType;				// 種類
	int m_nKey;					// キー
	int m_nNextKey;				// 次のキー
	int m_nCounter;				// カウンター
};
#endif