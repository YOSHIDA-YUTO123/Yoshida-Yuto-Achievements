//**************************************************
// 
// 機能カメラの追従処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_ZOOM_CAMERA_H_
#define _FANCTION_ZOOM_CAMERA_H_

#include "functionBase.h"

// 機能カメラの追従処理クラス
class CFuncZoomwCamera : public CFunctionBase
{
public:
	static constexpr float CAMERADISTANCE = 300.0f;
	static constexpr float CAMERADISTANCE_PTOR = 70.0f;
	static constexpr float CAMERAHEIGHT = 30.0f;
	static constexpr float CAMERAHEIGHTV = 40.0f;
	static constexpr float CAMERADIPS = -100.0f;
	static constexpr int MAXTIME = 80;

	CFuncZoomwCamera();
	~CFuncZoomwCamera();

	void Init(void) override;
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
	D3DXVECTOR3 m_posBace;
	D3DXVECTOR3 m_posDest;
	int m_nCnt;
};

#endif
