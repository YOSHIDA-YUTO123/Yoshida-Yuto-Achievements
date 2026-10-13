//===================================================
//
// オープニングシーンクラス [OpeningScene.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "scene.h"
#include "OpeningScene.h"
#include "AudienceManager.h"
#include "camera.h"
#include "manager.h"
#include "meshfield.h"
#include "ScenePlayerFactory.h"
#include "object3D.h"
#include "OpeningCamera.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "MapObject.h"
#include "sound.h"
#include "OpeningBall.h"
#include "OpeningUIManager.h"

//===================================================
// コンストラクタ
//===================================================
COpeningScene::COpeningScene() : 
	CScene(MODE_OPENING)
{
}

//===================================================
// デストラクタ
//===================================================
COpeningScene::~COpeningScene()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT COpeningScene::Init(void)
{	
	// ライトの取得
	CLight* pLight = CManager::GetLight();

	pLight->Init();
	pLight->SetDirectional(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f), { 0.0f,-0.1f,0.0f }, Const::VEC3_NULL);

	// カメラの変更
	CCamera *pCamera = CManager::ChangeCamera(std::make_unique<COpeningCamera>());

	// カメラの設定
	pCamera->SetCamera(D3DXVECTOR3(0.0f, 650.0f, 0.0f), Const::VEC3_NULL, D3DXVECTOR3(D3DX_PI * 0.80f, 0.0f, 0.0f));

	// フィールド
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.5f, 0.0f), 72, 72, D3DXVECTOR2(5000.0f, 2000.0f));

    // 観客
	CAudienceManager::Create(D3DXVECTOR3(-660.0f, 30.0f, 250.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);
	CAudienceManager::Create(D3DXVECTOR3(-680.0f, 40.0f, 280.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);
	CAudienceManager::Create(D3DXVECTOR3(-660.0f, 60.0f, 320.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);


	// 壁
	CObject3D* pWall = CObject3D::Create(D3DXVECTOR3(0.0f, 200.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3000.0f, 200.0f, 0.0f), "gameback.jpg");
	pWall->SetTexture(0.0f, 0.0f, 10.0f, 1.0f);

	CScenePlayer *pPlayer = ScenePlayerFactory::Create(CScenePlayer::Type::Opening, { -420.0f,0.0f,0.0f }, { 0.0f,-D3DX_PI * 0.5f,0.0f });

	// ボールの生成
	COpeningBall *pOpeningBall = COpeningBall::Create({ -170.0f,30.0f,0.0f });

	// イベントの登録
	pPlayer->RegisterEvent(std::bind(&COpeningBall::Active, pOpeningBall));

	//// イベントを登録
	//pPlayer->RegisterEvent([pOpeningBall]()
	//	{
	//		pOpeningBall->Active();
	//	}
	//);

	CSound* pSound = CManager::GetSound();
	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_SE_Cheers);
	}

	COpeningUIManager::Create();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void COpeningScene::Uninit(void)
{
	// カメラの変更
	CManager::ChangeCamera(std::make_unique<CCamera>());
}

//===================================================
// 更新処理
//===================================================
void COpeningScene::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// ムービーを飛ばす
	if (pKeyboard->GetTrigger(DIK_RETURN) || pKeyboard->GetTrigger(DIK_SPACE) || 
		pJoypad->GetTrigger(pJoypad->JOYKEY_B) || pJoypad->GetTrigger(pJoypad->JOYKEY_START))
	{
		// シーンの遷移
		CManager::GetFade()->SetFade(std::make_unique<CGame>());
	}

#ifdef _DEBUG

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->MouseView();
	}

#endif // _DEBUG
}

//===================================================
// 描画処理
//===================================================
void COpeningScene::Draw(void)
{
}
