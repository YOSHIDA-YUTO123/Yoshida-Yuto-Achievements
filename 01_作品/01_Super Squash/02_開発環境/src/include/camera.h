//===================================================
//
// カメラの処理 [camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_system_base.h"

//***************************************************
// 前方宣言
//***************************************************
class CCameraAnimationSystem; // カメラのアニメーション

//***************************************************
// カメラクラスの定義
//***************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetCamera(const D3DXVECTOR3& posV, const D3DXVECTOR3& posR, const D3DXVECTOR3& rot);
	void AddSystem(std::unique_ptr<CCameraSystemBase> pNewSystem);
	void PlayAnimation(const int nType);

	CCameraAnimationSystem* GetAnimation(void) { return m_pAnimation.get(); }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
private:
	void DebugCamera(void);
private:
	std::vector<std::unique_ptr<CCameraSystemBase>> m_pSystem;	// 拡張機能
	std::unique_ptr<CCameraAnimationSystem> m_pAnimation;		// カメラのアニメーションへのポインタ

	D3DXMATRIX m_mtxView;										// ビューマトリックス
	D3DXMATRIX m_mtxProjection;									// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;											// 視点の座標
	D3DXVECTOR3 m_posR;											// 注視点の座標
	D3DXVECTOR3 m_vecU;											// 上方向ベクトル
	D3DXVECTOR3 m_rot;											// 向き
	float m_fDistance;											// 距離
};
#endif
