//================================================
//
// オープニングのカメラの処理 [OpeningCamera.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _OPENING_CAMERA_H_
#define _OPENING_CAMERA_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "camera.h"
#include <memory>

//*************************************************
// 前方宣言
//*************************************************
class CCameraAnimationSystem; // カメラのアニメーション

//*************************************************
// オープニングのカメラのクラスの定義
//*************************************************
class COpeningCamera : public CCamera
{
public:
	COpeningCamera();
	~COpeningCamera();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
private:
	std::unique_ptr<CCameraAnimationSystem> m_pAnimation; // アニメーションへのポインタ
};
#endif