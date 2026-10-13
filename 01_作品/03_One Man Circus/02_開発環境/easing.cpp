//***********************************************************************
//
// イージング処理
// Aouther Tetsuji Yamamoto
// 
//***********************************************************************
#include "easing.h"

//***********************************************************************
// お好みのイージング処理をします
//***********************************************************************
float SelectEasing(int nType, float fp)
{
	// 計算結果
	float fAnswer = 0.0f;

	switch (nType)
	{
	case EASING_TYPE_NONE:
		fAnswer = fp;
		break;

	case EASING_TYPE_Liner:
		fAnswer = Liner(fp);
		break;

	case EASING_TYPE_EaseInExpo:
		fAnswer = EaseInExpo(fp);
		break;

	case EASING_TYPE_OutExpo:
		fAnswer = OutExpo(fp);
		break;

	case EASING_TYPE_EaseInOutCirc:
		fAnswer = EaseInOutCirc(fp);
		break;

	case EASING_TYPE_EaseInOutSine:
		fAnswer = EaseInOutSine(fp);
		break;
	default:
		break;
	}
	return fAnswer;
}

//***********************************************************************
// Liner P
//***********************************************************************
float Liner(float fp)
{
	return (fp);
}

//***********************************************************************
// easeInExpo 0 ? 0 : Math.pow(2, 10 * x - 10)
//***********************************************************************
float EaseInExpo(float fp)
{
	return 0 ? 0 : (float)pow(2, 10 * (double)fp - 10);
}

//***********************************************************************
// outExpo fp ==  1.0f ? 1.0f : 1.0f - (float)pow(2, -10 * (double)fp)
//***********************************************************************
float OutExpo(float fp)
{
	return fp ==  1.0f ? 1.0f : 1.0f - (float)pow(2, -10 * (double)fp);
}

//***********************************************************************
// easeInOutCirc fp < 0.5 ? (1 - sqrtf(1 - pow(2 * P,2))) / 2 : (sqrtf(1 - pow(-2 * P + 2, 2)) + 1) / 2
//***********************************************************************
float EaseInOutCirc(float fp)
{
	return fp < 0.5 ? (1.0f - sqrtf(1.0f - (float)pow((double)2.0f * (double)fp, 2.0))) / 2.0f : (sqrtf(1.0f - (float)pow((double)-2.0f * (double)fp + 2.0f, 2.0)) + 1.0f) / 2.0f;
}

//***********************************************************************
// easeInOutSine -(cosf(D3DX_PI * fp) - 1) / 2
//***********************************************************************
float EaseInOutSine(float fp)
{
	return -(cosf(D3DX_PI * fp) - 1) / 2;
}