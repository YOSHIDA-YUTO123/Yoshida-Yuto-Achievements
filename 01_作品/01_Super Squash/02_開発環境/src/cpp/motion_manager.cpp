//===================================================
//
// モーションのマネージャー [motion_manager.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion_manager.h"
#include "motion_component.hpp"
#include "utility_math.h"

//===================================================
// モーションの再生
//===================================================
bool MotionManager::Helper::Play(MotionComponent& MotionComp, const int nType, const int nFrameBlend, bool bBlend)
{
	// 現在のモーションのデータのアドレスの取得
	MotionComponent::CurrentInfo* pCurrent = &MotionComp.current;

	// ブレンドのモーションのデータのアドレスの取得
	MotionComponent::BlendInfo* pBlend = &MotionComp.blend;

	// 同じ種類のモーションだったら
	if (pBlend->nTypeBlend == nType || pCurrent->nType == nType) return false;

	// モーションの総数を超えていたら
	if (MotionComp.nNumMotion <= nType)
	{
		return false;
	}

	// ブレンドがあるなら
	if (bBlend == true)
	{
		pBlend->nKeyBlend = 0;					// ブレンドキーをリセット

		if (pBlend->bFirst == false)
		{
			pBlend->nCounterBlend = 0;			// ブレンドカウンターをリセット
			pBlend->nFrameBlend = nFrameBlend;	// ブレンドフレームを設定する
			pBlend->bFirst = true;				// 最初のブレンド開始フラグをtrueにする
		}

		pBlend->bFinish = false;				// モーションが終わっていない
		pBlend->nTypeBlend = nType;				// ブレンド先の種類を設定
	}
	else
	{
		pCurrent->nKey = 0;						// 最初から始める
		pCurrent->nCount = 0;					// 最初から始める
		pCurrent->nType = nType;				// ブレンドするモーションのタイプを代入
		pBlend->nTypeBlend = nType;				// ブレンドするモーションのタイプを代入
		pBlend->bFinish = false;
		pBlend->bFirst = false;
	}

	pBlend->bBlend = bBlend;					// ブレンドするかどうか判定
	pBlend->nCounterMotionBlend = 0;			// ブレンドモーションのカウンター
	MotionComp.nAllCounter = 0;					// 全体のフレームのカウンターをリセットする

	return true;
}

//===================================================
// モーションの全体フレームの割合の取得
//===================================================
float MotionManager::Helper::GetAnimationRatio(const MotionComponent& MotionComp, const int nType)
{
	// モーションが一致しないなら
	if (MotionComp.blend.nTypeBlend != nType)
	{
		return 0.0f;
	}

	// 全体のフレームの割合を求める
	float fRate = MotionComp.nAllCounter / static_cast<float>(MotionComp.nAllFrame);

	// 範囲制限
	fRate = math::Clamp(fRate, 0.0f, 1.0f);

	return fRate;
}

//===================================================
// モーションのイベントフレームの割合の取得
//===================================================
float MotionManager::Helper::GetEventFrameRatio(const MotionComponent& MotionComp, const int nType, const int nEventFrameIndex)
{
	// モーションが一致しないなら
	if (MotionComp.blend.nTypeBlend != nType)
	{
		return 0.0f;
	}

	// フレームのスタート地点を合わせる
	int nFrame = MotionComp.nAllCounter - MotionComp.aInfo[nType].aStartFrame[nEventFrameIndex];

	// -だったら計算しない
	if (nFrame < 0)
	{
		return 0.0f;
	}

	// 全体のフレームの割合を求める
	float fRate = nFrame / static_cast<float>(MotionComp.aInfo[nType].aEndFrame[nEventFrameIndex]);

	// 範囲制限
	fRate = math::Clamp(fRate, 0.0f, 1.0f);

	return fRate;
}

//===================================================
// モーションの種類の判定
//===================================================
bool MotionManager::Check::Type(const MotionComponent& MotionComp, const int nType)
{
	// 一致したら
	if (MotionComp.current.nType == nType) return true;

	return false;
}

//===================================================
// ブレンドモーションの種類の判定
//===================================================
bool MotionManager::Check::BlendType(const MotionComponent& MotionComp, const int nBlendType)
{
	// 一致したら
	if (MotionComp.blend.nTypeBlend == nBlendType) return true;

	return false;
}

//===================================================
// イベントフレームの判定
//===================================================
bool MotionManager::Check::EventFrame(const MotionComponent& MotionComp, const int nStart, const int nEnd, const int nType)
{
	// モーションの種類が違うなら
	if (MotionComp.blend.nTypeBlend != nType) return false;

	// 範囲内だったら
	if (MotionComp.nAllCounter >= nStart && MotionComp.nAllCounter <= nEnd)
	{
		return true;
	}
	return false;
}

//===================================================
// イベントフレームの判定
//===================================================
bool MotionManager::Check::EventFrame(const MotionComponent& MotionComp, const int nType)
{
	// モーションの種類が違うなら
	if (MotionComp.blend.nTypeBlend != nType) return false;

	// モーションの情報構造体
	MotionComponent::Info Info = MotionComp.aInfo[nType];

	// フレームの総数分回す
	for (int nCnt = 0; nCnt < Info.nNumEventFrame; nCnt++)
	{
		// 範囲内だったら
		if (MotionComp.nAllCounter >= Info.aStartFrame[nCnt] && MotionComp.nAllCounter <= Info.aEndFrame[nCnt])
		{
			return true;
		}
	}

	return false;
}

//===================================================
// イベントフレームの判定
//===================================================
bool MotionManager::Check::EventFrame(const MotionComponent& MotionComp, const int nType, const int nEventFrameIndex)
{
	// モーションの種類が違うなら
	if (MotionComp.blend.nTypeBlend != nType) return false;

	// モーションの情報構造体
	MotionComponent::Info Info = MotionComp.aInfo[nType];

	// 範囲内だったら
	if (MotionComp.nAllCounter >= Info.aStartFrame[nEventFrameIndex] && MotionComp.nAllCounter <= Info.aEndFrame[nEventFrameIndex])
	{
		return true;
	}

	return false;
}

//===================================================
// モーションが終わったかどうか
//===================================================
bool MotionManager::Check::Finish(const MotionComponent& MotionComp, const int nType)
{
	// モーションの種類が違うなら
	if (MotionComp.current.nType != nType) return false;

	// モーションの情報構造体
	MotionComponent::Info Info = MotionComp.aInfo[nType];

	// 最後のキーまで行ったら
	if (MotionComp.current.nKey >= Info.nNumkey - 1)
	{
		return true;
	}

	return false;
}
