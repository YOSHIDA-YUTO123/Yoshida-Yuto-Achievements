//================================================
//
// オープニングのカメラの処理 [OpeningCamera.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "OpeningCamera.h"
#include "CameraAnimationSystem.h"

//=================================================
// コンストラクタ
//=================================================
COpeningCamera::COpeningCamera() : 
	m_pAnimation(nullptr)
{
}

//=================================================
// デストラクタ
//=================================================
COpeningCamera::~COpeningCamera()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT COpeningCamera::Init(void)
{
	// 初期化処理
	if (FAILED(CCamera::Init()))
	{
		return E_FAIL;
	}

	// カメラのアニメーションの生成
	m_pAnimation = std::make_unique<CCameraAnimationSystem>(this);
	m_pAnimation->Init("data/SYSTEM/Opening_Camera.json");

#if 1
	m_pAnimation->Play(0);
#endif // 0

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void COpeningCamera::Uninit(void)
{
	// アニメーションの破棄
	if (m_pAnimation != nullptr)
	{
		m_pAnimation->Uninit();
		m_pAnimation.reset();
	}

	// 終了処理
	CCamera::Uninit();
}

//=================================================
// 更新処理
//=================================================
void COpeningCamera::Update(void)
{
	// アニメーションの更新
	if (m_pAnimation != nullptr)
	{
		m_pAnimation->Update();
	}

	// 更新処理
	CCamera::Update();
}
