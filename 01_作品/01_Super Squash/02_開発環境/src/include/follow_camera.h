//===================================================
//
// プレイヤーを追従するカメラの処理 [follow_camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FOLLOW_CAMERA_H_
#define _FOLLOW_CAMERA_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_system_base.h"

//***************************************************
// 追従するカメラのクラスの定義
//***************************************************
class CFollowCamera : public CCameraSystemBase
{
public:
	CFollowCamera(entt::registry& registry);
	~CFollowCamera() = default;

	void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot) override;
private:
	entt::registry& m_registry;	// レジストリー
};
#endif