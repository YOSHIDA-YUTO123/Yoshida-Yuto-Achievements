//==================================================
// 
// ゲーム終了ムービー [GameEndScene.h]
// Author：Aoto Kaichi
// 
//==================================================

// インクルード
#include "GameEndScene.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "particle.h"
#include "meshfield.h"
#include "AudienceManager.h"
#include "object3D.h"
#include "MapObject.h"
#include "StageLoader.h"
#include "Audience.h"
#include "functionBase.h"
#include "light.h"
#include "fade.h"
#include "result.h"
#include "sound.h"

std::unique_ptr<CCollisionMapObjManager> CGameEndScene::m_pCollMapObjManager = nullptr;// 当たり判定するマップオブジェクトのマネージャー

//===============================================
// コンストラクタ
//===============================================
CGameEndScene::CGameEndScene() : CScene(MODE_ENDING)
{
}
//===============================================
// デストラクタ
//===============================================
CGameEndScene::~CGameEndScene()
{
}
//===============================================
// 初期化
//===============================================
HRESULT CGameEndScene::Init(void)
{
	m_pCollMapObjManager = std::make_unique<CCollisionMapObjManager>();
	CStageLoader::Load(m_pCollMapObjManager.get(), "stage001.json");

	const D3DXVECTOR3 CAMERA_POSV = { 1500.0f, 50.0f, -300.0f };		// カメラの視点の座標			
	const D3DXVECTOR3 CAMERA_POSR = { 1500.0f, 50.0f, 0.0f };			// カメラの注視点の座標			
	const D3DXVECTOR3 CAMERA_ROT = { D3DX_PI / 2.0f, 0.0f, 0.0f };		// カメラの角度

	// カメラの位置の設定
	CCamera* pCamera = CManager::GetCamera();
	pCamera->SetCamera(CAMERA_POSV, CAMERA_POSR, CAMERA_ROT);
	pCamera->SetDistance(400.0f);

	// プレイヤーの生成
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CGameEndPlayer;
		m_pPlayer->CGameEndPlayer::Create(D3DXVECTOR3(1500.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	// フィールド
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.5f, 0.0f), 1, 1, D3DXVECTOR2(5000.0f, 2000.0f));

	// 観客
	CAudienceManager::Create(D3DXVECTOR3(1000.0f, 30.0f, 250.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);
	CAudienceManager::Create(D3DXVECTOR3(1000.0f, 40.0f, 280.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);
	CAudienceManager::Create(D3DXVECTOR3(1000.0f, 60.0f, 320.0f), D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);

	// 壁
	CObject3D* pWall = CObject3D::Create(D3DXVECTOR3(0.0f, 200.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3000.0f, 200.0f, 0.0f), "gameback.jpg");
	pWall->SetTexture(0.0f, 0.0f, 10.0f, 1.0f);

	CMapObject::Create(D3DXVECTOR3(1500.0f, 8.0f, 0.0f), GOAL_MODEL_PATH);

	//***************
	// 処理の追加
	AddFunction(std::make_unique<CFunctionGameEnd>());
	//***************

	CSound* pSound = CManager::GetSound();
	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_SE_Success, 0.3f);
		pSound->Play(CSound::SOUND_LABEL_SE_Appluse, 0.5f);
		pSound->Play(CSound::SOUND_LABEL_SE_Cheers, 0.7f);
	}

	return S_OK;
}
//===============================================
// 終了
//===============================================
void CGameEndScene::Uninit(void)
{

}
//===============================================
// 更新
//===============================================
void CGameEndScene::Update(void)
{
	//********************
	// 処理
	for (auto& fucs : m_vpFunction)
	{
		// 更新処理
		fucs->Update();
	}
	//********************

	const float CAMERADIS_DEST = 50.0f;

	// カメラの拡大
	CCamera* pCamera = CManager::GetCamera();
	pCamera->Zoom(CAMERADIS_DEST, 5.0f);

	if (pCamera->GetDistance() <= CAMERADIS_DEST)
	{
		m_nCntTime++;
		if (m_nCntTime > 120)
		{
			// 次に進む
			// フェードの取得
			CFade* pFade = CManager::GetFade();

			pFade->SetFade(std::make_unique<CResultWin>());
		}
	}

	// 観客を常に沸かせる
	CAudience::SetSuccess(true);
}
//===============================================
// 描画
//===============================================
void CGameEndScene::Draw(void)
{
}

//******************
//===================================================
// 処理の追加
//===================================================
void CGameEndScene::AddFunction(std::unique_ptr<CFunctionBase> pFunction)
{
	pFunction->Init();
	m_vpFunction.push_back(std::move(pFunction));
}
//*****************