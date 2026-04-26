//===================================================
//
// カメラが壁の裏側にいるか判定する処理 [check_camera_behind_wall.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHECK_CAMERA_BEHIND_WALL_H_
#define _CHECK_CAMERA_BEHIND_WALL_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_system_base.h"

//***************************************************
// カメラが壁の裏側にいるか判定するクラスの定義
//***************************************************
class CCheckCameraBehindWall : public CCameraSystemBase
{
public:
	explicit CCheckCameraBehindWall(entt::registry& registry);
	~CCheckCameraBehindWall() = default;

	void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot) override;
private:
	entt::registry& m_registry; // 参照用
};
#endif