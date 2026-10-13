//==============================================
//
// タイトルシーン [title.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "title.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "button.h"
#include "camera.h"
#include "CollisionMapObject.h"
#include "TitleUI.h"
#include "CollisionMapObjManager.h"
#include "StageLoader.h"
#include "object3D.h"
#include "meshfield.h"
#include "sound.h"
#include <Windows.h>
#include "light.h"
#include "ScenePlayerFactory.h"
#include "OpeningScene.h"
#include "result.h"

//**********************************************
// 定数宣言
//**********************************************
namespace TITLE_INFO
{
	const char* LOAD_STAGE_NAME = "stage001(Title).json"; // 読み込むステージのパス
};

//==============================================
// コンストラクタ
//==============================================
CTitle::CTitle() :
	CScene(MODE_TITLE),
	m_pCollMapObjManager(nullptr)
{
}

//==============================================
// デストラクタ
//==============================================
CTitle::~CTitle()
{
}

//==============================================
// 初期化処理
//==============================================
HRESULT CTitle::Init(void)
{
	// ライトの取得
	CLight* pLight = CManager::GetLight();

	pLight->Init();
	pLight->SetDirectional(D3DCOLOR_RGBA(255, 192, 203,255), D3DXVECTOR3(-0.52f, -0.59f, 0.62f), { 0.0f,0.0f,0.0f });

	//CObject2D::Create(50.0f, 50.0f, Const::CENTER_POS_2D);

	//CButton::INFO info;
	//info.colPolygon = Const::WHITE;
	//info.colPolygonEmp = Const::GRAY;
	//info.colWord = Const::BLACK;
	//info.colWordEmp = Const::GRAY;
	//info.fHeight = 50.0f;
	//info.fWidth = 500.0f;
	//info.pos = D3DXVECTOR3(600.0f,500.0f,0.0f);
	//info.rot = Const::VEC3_NULL;
	//CButton::Create(info, "Test テスト てすと　手酢斗",NULL,IDR_JF_DOT_M_10,FONT_NAME_JF_DOT_M_10);

	//CCollisionMapObject::Create(D3DXVECTOR3(0.0f,500.0f,0.0f), "player/testPlayer_000.x");

	// タイトルメニューの生成
	CTitleUIManager::Create();

	// フィールド
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, D3DXVECTOR2(2000.0f, 2000.0f))->SetTextureID("Title_Floor.png");

	// 壁
	CObject3D::Create(D3DXVECTOR3(0.0f, 250.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 350.0f, 0.0f), "TITLE/sky.png");

	// 当たり判定の生成
	m_pCollMapObjManager = std::make_unique<CCollisionMapObjManager>();

	// ステージの読み込み
	CStageLoader::Load(m_pCollMapObjManager.get(), TITLE_INFO::LOAD_STAGE_NAME);

	CManager::GetCamera()->SetCamera(D3DXVECTOR3(-1.95f, 30.0f, -600.0f), D3DXVECTOR3(-1.95f, 117.0f, 347.0f), D3DXVECTOR3(1.51f, 0.0f, 0.0f));

	// ロゴの生成
	CObject2D* pTitle = CObject2D::Create(1280 * 0.2f, 487 * 0.2f, D3DXVECTOR3(650.0f, 93.0f, 0.0f));
	pTitle->SetTextureID("TITLE/logo.png");
	
	// コピーライトの生成
	CObject2D* pCopyright = CObject2D::Create(480 * 0.3f, 210 * 0.3f, D3DXVECTOR3(160.0f, 640.0f, 0.0f));
	pCopyright->SetTextureID("TITLE/Copyright.png");

	CSound* pSound = CManager::GetSound();
	if (pSound != nullptr)
	{
		pSound->Play(CSound::SOUND_LABEL_TITLEBGM);
	}

	// プレイヤーの生成処理(伝言(プレイヤーはもう出てるけどサーカスの中にいて見えないだけだから位置調節頼む))
	ScenePlayerFactory::Create(CScenePlayer::Type::Title, D3DXVECTOR3(0.0f,0.0f,-500.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f));

	return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CTitle::Uninit(void)
{
	// マップオブジェクトのマネージャーの破棄
	if (m_pCollMapObjManager != nullptr)
	{
		m_pCollMapObjManager.reset();
	}

	// 音の取得
	CSound* pSound = CManager::GetSound();

	if (pSound != nullptr)
	{
		// 音の停止
		pSound->StopSound();
	}
}

//==============================================
// 更新処理
//==============================================
void CTitle::Update(void)
{

#ifdef _DEBUG

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// フェードの取得
	CFade* pFade = CManager::GetFade();

	if (pKeyboard->GetTrigger(DIK_O))
	{
		pFade->SetFade(std::make_unique<COpeningScene>(),40, { 1.0f,1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
	}
	if (pKeyboard->GetTrigger(DIK_F3))
	{
		pFade->SetFade(std::make_unique<CResultLose>());
	}

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->MouseView();
	}
#endif // _DEBUG
}

//==============================================
// 描画処理
//==============================================
void CTitle::Draw(void)
{
}
