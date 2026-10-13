//===============================================
//
// ランキング処理[ranking.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "ranking.h"
#include "manager.h"
#include "object2D.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "ScenePlayerFactory.h"
#include "MapObject.h"
#include "RankingScore.h"
#include "sound.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Ranking_Constants
{
	const D3DXVECTOR3 PLAYER_POS = { 0.0f,16.0f,0.0f };				// プレイヤーの位置
	const D3DXVECTOR3 PLAYER_ROT = { 0.0f,0.0f,0.0f };				// プレイヤーの向き
	const D3DXVECTOR3 AUDIENCCE_POS = { -300.0f, 30.0f, 100.0f };	// 観客の位置
	const D3DXVECTOR3 LIGHT_POS = { 0.0f,200.0f,0.0f };				// ライトの位置
	const D3DXVECTOR3 CAMERA_POSV = { 17.0f, 74.0f, -90.0f };		// カメラの視点の座標			
	const D3DXVECTOR3 CAMERA_POSR = { 154.0f, -306.0f, 707.0f };		// カメラの視点の座標			
	const D3DXVECTOR3 CAMERA_ROT = { 2.010f, 0.170f, 0.0f };			// カメラの視点の座標
	const D3DXVECTOR3 GOAL_POS = { 0.0f,8.0f,0.0f };				// ゴールの座標

	const char* GOAL_MODEL_PATH = "MapObjectFile/goal.x";			// ゴールのモデルのパス
	constexpr float LIGHT_RANGE = 350.0f;							// ライトの範囲
}

// 静的メンバ変数
CLoadStage* CRanking::m_pStage = NULL;
CRankMana* CRanking::m_pRank = NULL;

//===============================================
// コンストラクタ
//===============================================
CRanking::CRanking():CScene(MODE_RANKING)
{
	m_nCntStay = 0;
}

//===============================================
// デストラクタ
//===============================================
CRanking::~CRanking()
{
}

//===============================================
// 初期化
//===============================================
HRESULT CRanking::Init(void)
{
	// 定数の名前空間の使用
	using namespace Ranking_Constants;

	m_nCntStay = RANK_STAY;

	//CRankingScore::Create({ 800.0f,110.0f,0.0f }, { 200.0f,55.0f }, 500);

	// 背景の生成
	CObject2D::Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, Const::CENTER_POS_2D)->SetColor({1.0f,1.0f,1.0f,0.4f});

	//コルクの生成
	CObject2D* pCork = CObject2D::Create(1000 * 0.35f, 1009 * 0.35f, D3DXVECTOR3(928.0f, 360.0f, 0.0f));
	pCork->SetTextureID("Ranking/Cork.png");


	m_pRank = CRankMana::Create({ 1190.0f,199.0f,0.0f }, "data\\TEXT\\Ranking.txt");
	m_pRank->Set(CScore::Load("data\\TEXT\\NowScore.txt"));

	// カメラの位置の設定
	CManager::GetCamera()->SetCamera(CAMERA_POSV, CAMERA_POSR, CAMERA_ROT);

	// ライトの取得
	CLight* pLight = CManager::GetLight();

	pLight->Init();
	pLight->SetPoint(LIGHT_POS, LIGHT_RANGE, D3DCOLOR_RGBA(240, 128, 128, 255), Const::WHITE);

	CSound* pSound = CManager::GetSound();
	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_RANKING,0.3f);
	}


	CMeshField::Create(Const::VEC3_NULL, 64, 64, { 3000.0f,3000.0f });

	// プレイヤーの生成(リザルトのプレイヤーと処理が同じなので、つかい回す)
	ScenePlayerFactory::Create(CScenePlayer::Type::ResultLose, PLAYER_POS, PLAYER_ROT);

	CMapObject::Create(GOAL_POS, GOAL_MODEL_PATH);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRanking::Uninit(void)
{
	if (m_pRank != NULL)
	{
		m_pRank->Uninit();
		delete m_pRank;
		m_pRank = nullptr;
	}
}

//===============================================
// 更新
//===============================================
void CRanking::Update(void)
{
	//m_pRank->Update();

	m_nCntStay--;

	if (m_nCntStay <= 0)
	{
		m_nCntStay = 0;

		// キーボードの取得
		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();
		// ジョイパッドの取得
		CInputJoypad* pJoypad = CManager::GetInputJoypad();

		// フェードの取得
		CFade* pFade = CManager::GetFade();

		if ((pKeyboard != nullptr && pJoypad != nullptr) && pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A))
		{
			pFade->SetFade(std::make_unique<CTitle>());
		}
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

//===============================================
// 描画
//===============================================
void CRanking::Draw(void)
{

}