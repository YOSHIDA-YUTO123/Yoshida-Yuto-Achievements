//===================================================
//
// リザルトシーン [result.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "objectX.h"
#include "game.h"
#include "light.h"
#include "sound.h"
#include "title.h"
#include "object2D.h"
#include <fstream>
#include "score.h"
#include "ranking.h"
#include "ResultPlayer.h"
#include "meshfield.h"
#include "camera.h"
#include "MapObject.h"
#include "AudienceManager.h"
#include "functionBase.h"
#include "FunctionResult.h"
#include "light.h"
#include "ScenePlayerFactory.h"
#include "timer.h"
#include "DualUnitTimer.h"
#include "ResultLoseMenuManager.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Result_Constants
{
	const D3DXVECTOR3 PLAYER_POS		= { 0.0f,16.0f,0.0f };				// プレイヤーの位置
	const D3DXVECTOR3 PLAYER_ROT		= { 0.0f,0.0f,0.0f };				// プレイヤーの向き
	const D3DXVECTOR3 AUDIENCCE_POS		= { -300.0f, 30.0f, 100.0f };		// 観客の位置
	const D3DXVECTOR3 WIN_LIGHT_POS		= { 0.0f,200.0f,0.0f };				// ライトの位置
	const D3DXVECTOR3 LOSE_LIGHT_POS	= { 0.0f,200.0f,0.0f };				// ライトの位置
	const D3DXVECTOR3 CAMERA_POSV		= { -1.65f, 102.3f, -130.1f };		// カメラの視点の座標			
	const D3DXVECTOR3 CAMERA_POSR		= { -1.65f, -410.0f, 672.0f };		// カメラの視点の座標			
	const D3DXVECTOR3 CAMERA_ROT		= { 2.14f, 0.0f, 0.0f };			// カメラの視点の座標
	const D3DXVECTOR3 GOAL_POS			= { 0.0f,8.0f,0.0f };				// ゴールの座標

	const char* GOAL_MODEL_PATH			= "MapObjectFile/goal.x";			// ゴールのモデルのパス
	constexpr float WIN_LIGHT_RANGE		= 350.0f;							// ライトの範囲
	constexpr float LOSE_LIGHT_RANGE	= 250.0f;							// ライトの範囲
}

CDualUnitTimer* CResultWin::m_pTimer = nullptr;

//===================================================
// コンストラクタ
//===================================================
CResultWin::CResultWin() : 
	CScene(MODE_RESULT),
	m_vpFunction()
{
}

//===================================================
// デストラクタ
//===================================================
CResultWin::~CResultWin()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CResultWin::Init(void)
{
	//	定数の名前空間の使用
	using namespace Result_Constants;

	// ライトの取得
	CLight* pLight = CManager::GetLight();

	pLight->Init();
	pLight->SetPoint(WIN_LIGHT_POS, WIN_LIGHT_RANGE, D3DCOLOR_RGBA(240, 128, 128,255), Const::WHITE);

	// スコアのロード
	int nScore = LoadScore();

	//// 2Dポリゴンの生成
	//CObject2D::Create(300.0f,100.0f,D3DXVECTOR3(640.0f,200.0f,0.0f))->SetTextureID("clear.png");

	// カメラの位置の設定
	CManager::GetCamera()->SetCamera(CAMERA_POSV, CAMERA_POSR, CAMERA_ROT);

	//小切手の生成
	CObject2D* pCheck = CObject2D::Create(1280 * 0.32f, 375 * 0.32f, D3DXVECTOR3(650.0f, 600.0f, 0.0f));
	pCheck->SetTextureID("RESULT/Check.png");


	// スコアの生成
	CScore::Create(D3DXVECTOR3(930.0f,600.0f,0.0f), D3DXVECTOR2(250.0f, 55.0f), nScore);

	// タイマー生成
	m_pTimer = CDualUnitTimer::Create(D3DXVECTOR3(745.0f, 675.0f, 0.0f), D3DXVECTOR2(50, 35), CTimer::Load("data\\TEXT\\NowTime.txt"));
	m_pTimer->Update();
	m_pTimer->SetLess(false);
	m_pTimer->SetTextureAll("number000.png", "Coron_Black.png");

	// 観客
	CAudienceManager::Create(AUDIENCCE_POS, D3DXVECTOR2(15.0f, 25.0f), 5.0f, 20.0f);

	CMeshField::Create(Const::VEC3_NULL, 64, 64, { 1000.0f,1000.0f });

	// プレイヤーの生成
	ScenePlayerFactory::Create(CScenePlayer::Type::ResultWin, PLAYER_POS, PLAYER_ROT);

	//CResultPlayer::Create({ 0.0f,16.0f,0.0f }, { 0.0f,0.0f,0.0f });

	CMapObject::Create(GOAL_POS, GOAL_MODEL_PATH);

	// 処理の追加
	AddFunction(std::make_unique<CFunctionResult>());

	CSound* pSound = CManager::GetSound();
	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_SE_Appluse,0.5f);
		pSound->Play(CSound::SOUND_LABEL_SE_Cheers,0.7f);

	}


	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CResultWin::Uninit(void)
{
	// 処理の破棄
	for (auto& fucs : m_vpFunction)
	{
		fucs->Uninit();
		fucs.reset();
	}

	m_vpFunction.clear();
}

//===================================================
// 更新処理
//===================================================
void CResultWin::Update(void)
{
	// 処理
	for (auto &fucs : m_vpFunction)
	{
		// 更新処理
		fucs->Update();
	}
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	if ((pKeyboard != nullptr && pJoypad != nullptr) && pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A))
	{
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			// 新しいモードの設定
			pFade->SetFade(std::make_unique<CRanking>());
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

//===================================================
// 描画処理
//===================================================
void CResultWin::Draw(void)
{
}

//===================================================
// スコアのロード
//===================================================
int CResultWin::LoadScore(void)
{
	// ファイルを開く
	std::fstream file("data/TEXT/NowScore.txt");
	std::string line, input;

	std::string filepath;	// ファイルパス

	int nScore = 0;

	// nullじゃなかったら
	if (file.is_open())
	{
		file >> nScore;

		file.close();
		file.clear();
	}
	else
	{
		MessageBox(NULL, "エラー", "LoadScore", MB_OK);
		return 0;
	}


	return nScore;
}

//===================================================
// 処理の追加
//===================================================
void CResultWin::AddFunction(std::unique_ptr<CFunctionBase> pFunction)
{
	pFunction->Init();
	m_vpFunction.push_back(std::move(pFunction));
}

//===================================================
// コンストラクタ
//===================================================
CResultLose::CResultLose() : CScene(MODE_RESULT)
{

}

//===================================================
// デストラクタ
//===================================================
CResultLose::~CResultLose()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CResultLose::Init(void)
{
	//	定数の名前空間の使用
	using namespace Result_Constants;

	//// 2Dポリゴンの生成
	//CObject2D::Create(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, Const::CENTER_POS_2D)->SetTextureID("game over.png");

		// ライトの取得
	CLight* pLight = CManager::GetLight();

	pLight->Init();
	pLight->SetPoint(LOSE_LIGHT_POS, LOSE_LIGHT_RANGE, D3DCOLOR_RGBA(123, 104, 238, 255), Const::WHITE);

	CMeshField::Create(Const::VEC3_NULL, 64, 64, { 3000.0f,3000.0f });

	// カメラの位置の設定
	CManager::GetCamera()->SetCamera(CAMERA_POSV, CAMERA_POSR, CAMERA_ROT);

	// プレイヤーの生成
	ScenePlayerFactory::Create(CScenePlayer::Type::ResultLose, PLAYER_POS, PLAYER_ROT);

	CMapObject::Create(GOAL_POS, GOAL_MODEL_PATH);

	CSound* pSound = CManager::GetSound();

	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_TIMEOVER,0.2f);
	}

	CObject2D::Create(300.0f, 80.0f, D3DXVECTOR3(640.0f, 100.0f, 0.0f))->SetTextureID("Time_Up.png");

	CObjectX::Create({ 100.0f , 20.0f, 0.0f } ,  "MapObjectFile/ballJa.x", {});
	CObjectX::Create({ 100.0f , 20.0f, 30.0f }, "MapObjectFile/ballJa.x" , {});
	CObjectX::Create({ -100.0f, 20.0f, 30.0f }, "MapObjectFile/ballJa.x" , {});
	CObjectX::Create({ 20.0f  , 0.0f , 80.0f }, "Model()/hulahoop.x"	 , {0.0f,0.0f,D3DXToRadian(5)});
	CObjectX::Create({ -40.0f , 10.0f, 30.0f }, "Model()/drumcan.x"      , {0.0f,0.0f,D3DXToRadian(5)});
	
	// リザルトのメニューのマネージャーの生成(オブジェクト側がdeleteするからローカルで問題なし)
	CResultLoseMenuManager* pResultMenuManager = new CResultLoseMenuManager;
	pResultMenuManager->Init();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CResultLose::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CResultLose::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CResultLose::Draw(void)
{
}
