//================================================
//
// シーンのプレイヤーの基底クラス [ScenePlayer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "ScenePlayer.h"
#include "character3D.h"
#include "player.h"

//=================================================
// コンストラクタ
//=================================================
CScenePlayer::CScenePlayer() : 
	m_pCharacter(nullptr)
{
}

//=================================================
// デストラクタ
//=================================================
CScenePlayer::~CScenePlayer()
{

}

//=================================================
// 生成処理
//=================================================
void CScenePlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	if (FAILED(Init()))
	{
		return;
	}

	m_pCharacter->SetPosition(pos);
	m_pCharacter->SetRotation(rot);
	m_pCharacter->SetRotDest(rot);
}

//=================================================
// 初期化処理
//================================================
HRESULT CScenePlayer::Init(void)
{
	// キャラクターの読み込み
	m_pCharacter = CCharacter3D::Create("motion.txt", Const::VEC3_NULL, Const::VEC3_NULL, CPlayer::MOTION_MAX);

	// 生成できなかったら
	if (m_pCharacter == nullptr)
	{
		MessageBox(NULL, "エラー", "ScenePlayer初期化失敗", MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CScenePlayer::Uninit(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		m_pCharacter.reset();
	}

	// 自分自身の破棄
	CObject::Release();
}

//=================================================
// 更新処理
//=================================================
void CScenePlayer::Update(void)
{
	// 更新処理
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
		m_pCharacter->UpdateMotion();
	}
}

//=================================================
// 描画処理
//=================================================
void CScenePlayer::Draw(void)
{
	// 描画処理
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}
