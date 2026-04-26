//===================================================
//
// モーションのデータ [motion_component.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_COMPONENT_HPP_
#define _MOTION_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"

//***************************************************
// モーションのデータの定義
//***************************************************
struct MotionComponent
{
	// モーションの種類
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,			// ニュートラル
		MOTIONTYPE_MOVE,				// 移動
		MOTIONTYPE_SHOT,				// ショット
		MOTIONTYPE_QUICK_SHOT,			// 素早いショット
		MOTIONTYPE_SERVE_STANCE,		// サーブ構え
		MOTIONTYPE_SERVE_SHOT,			// サーブショット
		MOTIONTYPE_QUICK_SHOT_LEFT,		// 素早いショット(左にボールが来た時)
		MOTIONTYPE_JUMP,				// ジャンプ
		MOTIONTYPE_LANDING,				// 着地
		MOTIONTYPE_JUMP_SHOT,			// ジャンプショット
		MOTIONTYPE_BLOW_OFF,			// 吹き飛び
		MOTIONTYPE_CHARGE,				// 溜めモーション
		MOTIONTYPE_JET_PACK,			// ジェットパック
		MOTIONTYPE_NEUTRAL_LEFT_HAND,	// ニュートラル(ラケット左手持ち)
		MOTIONTYPE_JETPACK_MOVE_LEFT,	// ジェットパックの素早い左移動
		MOTIONTYPE_JETPACK_MOVE_RIGHT,	// ジェットパックの素早い右移動
		MOTIONTYPE_RIGHT_MOVE,			// 右移動
		MOTIONTYPE_LEFT_MOVE,			// 左移動
		MOTIONTYPE_MAX,
	};

	MotionComponent(const int _nNumMotion) : 
		nNumMotion(_nNumMotion), nNumModel(0),
		current({}), blend({}), aInfo({}),
		nAllFrame(0), nAllCounter(0)
	{
		// 配列の確保
		aInfo.resize(nNumMotion);
	}

	~MotionComponent()
	{
		for (auto& info : aInfo)
		{
			for (auto& keyinfo : info.aKeyInfo)
			{
				keyinfo.aKey.clear();
			}
		}

		for (auto& info : aInfo)
		{
			info.aKeyInfo.clear();
			info.aStartFrame.clear();
			info.aEndFrame.clear();
		}

		// 要素のクリア
		aInfo.clear();
	}

	// キーの構造体
	struct Key
	{
		float fPosX; // 位置X
		float fPosY; // 位置Y
		float fPosZ; // 位置Z

		float fRotX; // 向きX
		float fRotY; // 向きY
		float fRotZ; // 向きZ
	};

	// キー情報の構造体
	struct KeyInfo
	{
		std::vector <Key> aKey;		// 各キーパーツのキー要素
		int nFrame;					// 再生フレーム
	};

	// モーション情報の構造体
	struct Info
	{
		std::vector<KeyInfo> aKeyInfo;	// キー情報
		std::vector<int> aStartFrame;	// スタートフレーム
		std::vector<int> aEndFrame;		// 終了フレーム
		int nNumkey;					// キーの総数
		int nNumEventFrame;				// イベントフレームの総数
		bool bLoop;						// ループするかどうか
	};

	// 現在のモーションのデータ
	struct CurrentInfo
	{
		int nType;						// モーションの種類
		int nNumKey;					// キーの総数
		int nKey;						// 現在のキーNo.
		int nCount;						// モーションのカウンター
		int nNextKey;					// 次のキー
		bool bLoopMotion;				// ループするかどうか
	};

	// ブレンド
	struct BlendInfo
	{
		bool bBlend;					// ブレンドがあるかどうか
		bool bLoopBlend;				// ループするかどうか
		bool bFinish;					// モーションが終わったかどうか
		bool bFirst;					// モーションが始まったフラグ

		int nEndBlendMotion;			// モーションが終わった時につなげるモーションの種類
		int nTypeBlend;					// ブレンドの種類
		int nFrameBlend;				// ブレンドのフレーム数
		int nCounterBlend;				// ブレンドカウンター
		int nNumKeyBlend;				// ブレンドモーションの最大のキー
		int nKeyBlend;					// ブレンドモーションの現在のキー
		int nNextKeyBlend;				// ブレンドモーションの次のキー
		int nCounterMotionBlend;		// ブレンドのカウンター
	};

	std::vector<Info> aInfo;	// モーション情報へのポインタ
	CurrentInfo current;		// 現在のデータ
	BlendInfo blend;			// ブレンドのデータ
	int nNumMotion;				// モーションの総数
	int nNumModel;				// モデルの総数
	int nAllFrame;				// 全体のフレーム数
	int nAllCounter;			// 全体フレームのカウンター
};
#endif