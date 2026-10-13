//**************************************************
// 
// 機能デバッグ時のカメラの処理クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTION_DEBUG_CAMERA_H_
#define _FANCTION_DEBUG_CAMERA_H_

#include "functionBase.h"

// 機能デバッグ時のカメラの処理クラス
class CFuncDebCamera : public CFunctionBase
{
public:
	CFuncDebCamera() {}
	~CFuncDebCamera() {}

	void Init(void) override {}
	void Uninit(void)override { delete this; }
	void Update(void) override;

private:
};

#endif
