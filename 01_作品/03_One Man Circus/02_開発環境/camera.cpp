//===================================================
//
// カメラ [camera.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "camera.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "math.h"
#include "meshfield.h"
#include "input.h"
#include "pause.h"
#include "Collider.h"
#include "easing.h"
#include "particle.h"

using namespace Const; // 名前空間Constを使用

namespace
{
	constexpr float MAX_VIEW_TOP = 2.90f;	// カメラの制限(上)
	constexpr float MAX_VIEW_BOTTOM = 0.0f; // カメラの制限(下)
	constexpr float HEIGHT_OFFSET = 20.0f;	// カメラの高さのオフセット
	constexpr float ROCKON_HEIGHT = 200.0f;	// ロックオンの時のカメラの高さ
	constexpr float FOV_BASE = 45.0f;		// 視野角ベース
	constexpr float FOV_COLLISION = 65.0f;	// 視野角壁際の時
	constexpr float WHEEL_ZOOM = 100.0f;	// wheelのズーム
}

//===================================================
// コンストラクタ
//===================================================
CCamera::CCamera()
{
	m_posVOld = VEC3_NULL;
	m_fFov = FOV_BASE;
	m_bCollision = false;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
	m_posR = VEC3_NULL;
	m_posV = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_posRDest = VEC3_NULL;
	m_posVDest = VEC3_NULL;
	m_vecU = VEC3_NULL;
	m_fDistance = NULL;
}

//===================================================
// デストラクタ
//===================================================
CCamera::~CCamera()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCamera::Init(void)
{
	m_posR = VEC3_NULL;

	m_posV = VEC3_NULL;		// 視点

	m_rot = VEC3_NULL;	// 角度

	//m_posV = D3DXVECTOR3(0.0f, 250.0f, -350.0f);		// 視点

	//m_rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	// 角度

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCamera::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CCamera::Update(void)
{
	//if (m_fDistance <= 0.0f)
	//{
	//	auto p = CParticle::Create({ 0.0f,0.0f,0.0f }, Const::WHITE, 10.0f, 100, CParticle::TYPE_NORMAL);
	//	p->SetParam({ 2.0f,2.0f,2.0f }, 1, 60);
	//}
	
	// 角度の正規化
	math::NormalizeRot(&m_rot.x);
	math::NormalizeRot(&m_rot.y);
	math::NormalizeRot(&m_rot.z);
}

//===================================================
// 設定,描画処理
//===================================================
void CCamera::SetCamera(void)
{
	CRenderer* pRenderer = CManager::GetRenderer();

	// レンダラーがnullだったら処理しない
	if (pRenderer == nullptr) return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(m_fFov),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2500.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//===================================================
// カメラの設定処理
//===================================================
void CCamera::SetCamera(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot)
{
	// 設定処理
	m_posV = posV;
	m_posR = posR;
	m_rot = rot;

	float fRotX = m_posV.x - m_posR.x;
	float fRotY = m_posV.y - m_posR.y;
	float fRotZ = m_posV.z - m_posR.z;

	// 距離を求める
	m_fDistance = sqrtf((fRotX * fRotX) + (fRotY * fRotY) + (fRotZ * fRotZ));

	//カメラの注視点初期座標を設定
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	// カメラの視点の情報
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// カメラの設定
//===================================================
void CCamera::SetCamera(const D3DXVECTOR3 posR)
{
	// 注視点の設定
	m_posR = posR;

	// カメラの視点の情報
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// ズームの設定
//===================================================
void CCamera::SetZoom(const float fDestDistance, const int nFream)
{
	// 元のカメラ距離保存
	m_fOldDistance = m_fDistance;

	// 目的の距離設定
	m_fDestDistance = fDestDistance;

	// フレーム設定
	m_nZoomFream = nFream;

	// フレームカウンタリセット
	m_nZoomCount = 0;
}

//===================================================
// マウスホイールの処理
//===================================================
void CCamera::MouseWheel(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	// マウスホイールの回転量を取得
	int nWheel = pMouse->GetMouseWeel();

	// ホイールを回してなかったら
	if (nWheel == 0)
	{
		return;
	}

	if (nWheel < 0)
	{
		m_fDistance += m_fDistance * 0.01f;
	}
	else if (nWheel > 0)
	{
		m_fDistance -= m_fDistance * 0.01f;
	}
	
	// 範囲の制限
	m_fDistance = math::Clamp(m_fDistance, 5.0f, 999999.0f);

	// 視点の更新処理
	UpdatePositionV();
}

//===================================================
// マウスの視点移動処理
//===================================================
void CCamera::MouseView(void)
{
#ifdef _DEBUG

	// マウスの取得
	CInputMouse* pMouse = CManager::GetInputMouse();

	// マウスホイール
	CCamera::MouseWheel();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_LALT) == false) return;

	D3DXVECTOR2 Move = pMouse->GetVelocity();
	D3DXVECTOR2 MoveOld = pMouse->GetOldVelocity();

	D3DXVECTOR2 fAngle = Move - MoveOld;

	CCamera::PadView();

	// 同時押し
	if (pMouse->OnMousePress(0) && pMouse->OnMousePress(1))
	{
		D3DXVECTOR3 eye = m_posR - m_posV;
		D3DXVec3Normalize(&eye, &eye);

		D3DXVECTOR3 cross,cameraUp;
		D3DXVec3Cross(&cross, &m_vecU, &eye);
		
		D3DXVec3Cross(&cameraUp, &eye, &cross);
		D3DXVECTOR3 move = cross * (-fAngle.x);

		// 縦移動
		move += cameraUp * fAngle.y;

		// カメラの位置更新
		m_posV += move;
		m_posR += move;
	}
	else if (pMouse->OnMousePress(1))
	{
		// 回転量を更新
		m_rot.y += fAngle.x * 0.01f;
		m_rot.x += fAngle.y * 0.01f;

		// 回転量を制限
		if (m_rot.x > MAX_VIEW_TOP)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}
		else if (m_rot.x < MAX_VIEW_BOTTOM)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}

		//カメラ座標を更新
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	}
	else if (pMouse->OnMousePress(0))
	{
		//回転量を更新
		m_rot.y += fAngle.x * 0.01f;
		m_rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (m_rot.x > MAX_VIEW_TOP)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}
		else if (m_rot.x < MAX_VIEW_BOTTOM)
		{
			m_rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
		m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	}
#endif // _DEBUG
}

//===================================================
// パッドの視点移動処理
//===================================================
void CCamera::PadView(void)
{
	// コントローラーの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// スティック
	XINPUT_STATE* pStick = pJoypad->GetJoyStickAngle();

	if (pJoypad->GetJoyStickR() == true)
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag >= DeadZone)
		{
			float NormalizeX = RStickAngleX / fMag;
			float NormalizeY = RStickAngleY / fMag;

			float fAngle = fMag * 0.000003f;

			// 回転量を計算
			float fAngleX = NormalizeX * 0.5f * fAngle;
			float fAngleY = NormalizeY * 0.5f * fAngle;

			m_rot.y += fAngleX;
			m_rot.x -= fAngleY;

			// 回転量を制限
			if (m_rot.x > MAX_VIEW_TOP)
			{
				m_rot.x += fAngleY;
			}
			else if (m_rot.x < MAX_VIEW_BOTTOM)
			{
				m_rot.x += fAngleY;
			}

			// 視点の更新処理
			UpdatePositionV();
		}
	}
}

//===================================================
// 視点の更新処理
//===================================================
void CCamera::UpdatePositionV(void)
{
	// 前回の位置の設定
	m_posVOld = m_posV;

	// カメラの視点の更新
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// 注視点の更新処理
//===================================================
void CCamera::UpdatePositionR(void)
{
	// カメラの注視点の更新
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// 位置を目的の位置に近づける
//===================================================
void CCamera::LerpPos(const D3DXVECTOR3 posRDest, const D3DXVECTOR3 posVDest, const float fCoef)
{
	// 前回の位置の設定
	m_posVOld = m_posV;

	// 目的の注視点の設定
	m_posRDest.x = posRDest.x;
	m_posRDest.y = posRDest.y;
	m_posRDest.z = posRDest.z;

	// 目的の視点の設定
	m_posVDest.x = posVDest.x;
	m_posVDest.y = posVDest.y;
	m_posVDest.z = posVDest.z;

	// 目的の注視点に近づける
	m_posR.x += (m_posRDest.x - m_posR.x) * fCoef;
	m_posR.y += (m_posRDest.y - m_posR.y) * fCoef;
	m_posR.z += (m_posRDest.z - m_posR.z) * fCoef;

	// 目的の視点に近づける
	m_posV.x += (m_posVDest.x - m_posV.x) * fCoef;
	m_posV.y += (m_posVDest.y - m_posV.y) * fCoef;
	m_posV.z += (m_posVDest.z - m_posV.z) * fCoef;
}

//===================================================
// 追従カメラ処理
//===================================================
void CCamera::TrackingCamera(D3DXVECTOR3 posR,float fPalam)
{
	// 注視点の移動量を計算
	D3DXVECTOR3 vecPosR = posR - m_posR;
	
	vecPosR = vecPosR * fPalam* fPalam;

	// 注視点設定
	m_posR += vecPosR;
	
	// 視点に注視点のずらした量を足す
	m_posV += vecPosR;
}

//===============================================
// 目標地点までズームする(イージング)
//===============================================
void CCamera::Zoom(void)
{
	//if (m_nZoomFream <= m_nZoomCount)// ズームし終わってるなら
	//{
	//	return;
	//}

	//// フレームカウント
	//++m_nZoomCount;

	//// 進行度を計算 (0〜1)
	//float t = 0.0f;

	//if (m_nZoomCount > 0.0f)
	//{
	//	t = (float)m_nZoomCount / (float)m_nZoomFream;
	//	if (t < 0.0f) t = 0.0f;
	//	if (t > 1.0f) t = 1.0f;
	//}

	//// 新しい距離を計算
	//m_fDistance = m_fOldDistance - (m_fOldDistance - m_fDestDistance) * SelectEasing(EASING_TYPE_EaseInOutSine, t);

	//// カメラの視点の更新
	//m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	//m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	//m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===============================================
// 目標地点までズームする
//===============================================
void CCamera::Zoom(float fDisDest, const float fZoomSpeed)
{
	m_fDistance -= fZoomSpeed;

	if (m_fDistance <= fDisDest)
	{
		m_fDistance = fDisDest;
	}

	// カメラの視点の更新
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// カメラ距離変更処理
//===================================================
void CCamera::SetRot(const D3DXVECTOR3 rot) 
{ 
	m_rot = rot; 

	// カメラの視点の更新
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//===================================================
// カメラ距離変更処理
//===================================================
void CCamera::SetDistance(const float fDistance)
{
	m_fDistance = fDistance; 

	// カメラの視点の更新
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}