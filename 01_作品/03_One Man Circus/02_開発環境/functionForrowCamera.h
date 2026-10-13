//**************************************************
// 
// 機能カメラの追従処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_FORROW_CAMERA_H_
#define _FANCTION_FORROW_CAMERA_H_

#include "functionBase.h"

// 機能カメラの追従処理クラス
class CFuncForrowCamera : public CFunctionBase
{
public:
	static constexpr float CAMERADISTANCE = 350.0f;
	static constexpr float CAMERADISTANCE_PTOR = 50.0f;
	static constexpr float CAMERAHEIGHT = 20.0f;
	static constexpr float CAMERAANGLE = D3DX_PI * 0.4f;
	static constexpr float PALAM = 0.03f;
	static constexpr float LIMITXA = -400.0f;
	static constexpr float LIMITXB = 1350.0f;
	static constexpr float LIMITZ = -400.0f;

	CFuncForrowCamera() {}
	~CFuncForrowCamera() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif
