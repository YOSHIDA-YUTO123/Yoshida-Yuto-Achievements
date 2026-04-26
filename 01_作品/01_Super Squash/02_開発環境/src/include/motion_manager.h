//===================================================
//
// モーションのマネージャー [motion_manager.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_

//***************************************************
// 前方宣言
//***************************************************
struct MotionComponent; // モーションのコンポーネント

//***************************************************
// モーションのマネージャー名前空間の定義
//***************************************************
namespace MotionManager
{
	// 補助
	namespace Helper
	{
		bool Play(MotionComponent& MotionComp, const int nType, const int nFrameBlend, bool bBlend);
		float GetAnimationRatio(const MotionComponent& motionComp, const int nType);
		float GetEventFrameRatio(const MotionComponent& motionComp, const int nType, const int nEventFrameIndex);
	}

	// 判定系
	namespace Check
	{
		bool Type(const MotionComponent& motionComp, const int nType);
		bool BlendType(const MotionComponent& motionComp, const int nBlendType);
		
		// 直接指定
		bool EventFrame(const MotionComponent& motionComp, const int nStart, const int nEnd, const int nType);

		// ファイルから読み取ったすべてのイベントフレームで判定
		bool EventFrame(const MotionComponent& motionComp, const int nType);

		// ファイルから読み取ったイベントフレームを指定して判定
		bool EventFrame(const MotionComponent& motionComp, const int nType, const int nEventFrameIndex);
		bool Finish(const MotionComponent& motionComp, const int nType);
	}
}
#endif