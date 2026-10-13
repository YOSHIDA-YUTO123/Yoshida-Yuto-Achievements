//***********************************************************************
//
// イージング処理
// Aouther Tetsuji Yamamoto
// 
//***********************************************************************
#ifndef _EASING_H_
#define _EASING_H_

#include "main.h"

// 列挙型イージングの種類
typedef enum
{
	EASING_TYPE_NONE = 0,		// なし
	EASING_TYPE_Liner,			// 特に変化がない直線の動き
	EASING_TYPE_EaseInExpo,		// 前半はほとんど変化がなく,中盤に値が大きくなり,後半に急激に大きくなる
	EASING_TYPE_OutExpo,		// 前半は急激に大きくなる,中盤に値が穏やかになり,後半にほとんど変化がない
	EASING_TYPE_EaseInOutCirc,	// 前半は穏やかに増えるる,中盤に急激に増える,後半は穏やかになる
	EASING_TYPE_EaseInOutSine,	// 前半は穏やかに増え,中盤は一定の速度で,後半は穏やかになる
	EASING_TYPE_MAX
}EASING_TYPE;

// プロトタイプ宣言
float SelectEasing(int nType, float fp);	// お好みのイージング処理をします
float Liner(float fp);						// Liner
float EaseInExpo(float fp);					// easeInExpo 0 ? 0 : Math.pow(2, 10 * x - 10)
float OutExpo(float fp);					// outExpo fp ==  1.0f ? 1.0f : 1.0f - (float)pow(2, -10 * (double)fp)
float EaseInOutCirc(float fp);				// easeInOutCirc fp < 0.5 ? (1 - sqrtf(1 - pow(2 * P,2))) / 2 : (sqrtf(1 - pow(-2 * P + 2, 2)) + 1) / 2
float EaseInOutSine(float fp);				// easeInOutSine -(cosf(D3DX_PI * fp) - 1) / 2
#endif // !_EASING_H_
