//===================================================
//
// カメラの揺れの処理 [camera_shake.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_SHAKE_H_
#define _CAMERA_SHAKE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_system_base.h"

//***************************************************
// カメラの揺れの処理の定義
//***************************************************
class CCameraShake : public CCameraSystemBase
{
public:
	CCameraShake();
	~CCameraShake();

	void Begin(const float fRange, const int nTime);
	void Update(D3DXVECTOR3 &posV, D3DXVECTOR3 &posR, D3DXVECTOR3 &rot) override;
private:
	D3DXVECTOR3 m_posRStock;	// 保存用の注視点
	float m_fRange;				// 揺れの範囲
	int m_nTime;				// 揺れの時間
	bool m_bStock;				// 保存したかどうか
};
#endif