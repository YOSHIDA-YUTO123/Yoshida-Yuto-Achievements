//===================================================
//
// ボールのデータ [ball_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_COMPONENT_HPP_
#define _BALL_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************
#include "array"

//***************************************************
// ボールのコンポーネントの定義
//***************************************************
struct BallComponent
{
	static constexpr float BACE_RESTITUTION = 0.8f;		// 反発係数
	static constexpr float MIN_SPEED		= 1.0f;		// 最小の速度

	BallComponent(const float _fRestitution = BACE_RESTITUTION) :
		orbitID(entt::null), shadowID(entt::null), landingPos(Const::VEC3_NULL),
		fRestitution(_fRestitution), fSpeed(MIN_SPEED), nInterval(0),
		bBound(false), bTargetHit(false)
	{}

	entt::entity orbitID;			// 軌跡のID
	entt::entity shadowID;			// 影のID
	D3DXVECTOR3 landingPos;			// 着地地点
	float fRestitution;				// 反発係数
	float fSpeed;					// 速さ
	int nInterval;					// 撃たれてから次打てるまで
	bool bBound;					// バウンドをしたかどうか
	bool bTargetHit;				// ターゲットに当たったかどうか
};
#endif