//===================================================
//
// カメラの拡張機能のインターフェース [camera_system_base.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_SYSTEM_BASE_H_
#define _CAMERA_SYSTEM_BASE_H_

//***************************************************
// カメラの拡張機能のインターフェースの定義
//***************************************************
class CCameraSystemBase
{
public:
	CCameraSystemBase() = default;
	virtual ~CCameraSystemBase() = default;

	virtual void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot) = 0;
private:
};
#endif