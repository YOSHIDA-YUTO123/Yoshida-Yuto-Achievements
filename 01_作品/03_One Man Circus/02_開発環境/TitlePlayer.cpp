//================================================
//
// タイトルのプレイヤー [TitlePlayer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "TitlePlayer.h"
#include "TitleUI.h"
#include "manager.h"
#include "player.h"
#include <d3dx9math.h>
#include "character3D.h"
#include "motion.h"
//=================================================
// コンストラクタ
//=================================================
CTitlePlayer::CTitlePlayer()
	: m_bStartAnim(false),
	m_fMoveSpeed(-300.0f)
{
}

//=================================================
// デストラクタ
//=================================================
CTitlePlayer::~CTitlePlayer()
{
}

//=================================================
// 初期化処理
//=================================================
HRESULT CTitlePlayer::Init(void)
{
	// 初期化処理
	if (FAILED(CScenePlayer::Init()))
	{
		return E_FAIL;
	}

	m_bStartAnim = false;


	return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CTitlePlayer::Uninit(void)
{
	// 終了処理
	CScenePlayer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CTitlePlayer::Update(void)
{
	// 更新処理
	CScenePlayer::Update();

	// Title UI 管理クラスから Start が押されたか確認
	auto pTitleMgr = CTitleUIManager::GetInstance();
	if (pTitleMgr != nullptr)
	{
		// Start が押された瞬間に演出開始フラグを立てる
		if (!m_bStartAnim && pTitleMgr->CheckStart())
		{
			m_bStartAnim = true;

			// モーションを走る系に切り替える（任意）
			CCharacter3D* pChar = GetCharacter();
			if (pChar != nullptr && pChar->GetMotion() != nullptr)
			{
				pChar->GetMotion()->SetMotion(CPlayer::MOTION_MOVE, true, 8);
			}
		}
	}

	// 演出中はキャラクターを前方へ移動させる
	if (m_bStartAnim)
	{
		CCharacter3D* pChar = GetCharacter();
		if (pChar != nullptr)
		{
			// 現在の位置・回転を取得
			D3DXVECTOR3 pos = pChar->GetPosition();
			D3DXVECTOR3 rot = pChar->GetRotation();

			// 回転 y を向きとして前方ベクトルを計算
			D3DXVECTOR3 forward;
			forward.x = sinf(rot.y);
			forward.y = 0.0f;
			forward.z = cosf(rot.y);

			// 簡易フレーム補正（60FPS 想定）。環境に合わせてデルタタイムに置き換えてください。
			const float dt = 1.0f / 60.0f;

			// 前進
			pos += forward * (m_fMoveSpeed * dt);

			// 位置をセット（CScenePlayer 内部の m_pCharacter を直接操作）
			pChar->SetPosition(pos);
		}
	}
}

//=================================================
// 描画処理
//=================================================
void CTitlePlayer::Draw(void)
{
	// 描画処理
	CScenePlayer::Draw();
}
