//==================================================
// 
// 観客 [Audience.cpp]
// Author：Rio Ohno
// 
//==================================================

// インクルード
#include"Audience.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "game.h"
#include "transform.h"
#include "renderer.h"
#include "sound.h"

// 静的メンバ変数
bool CAudience::m_bMiss = false;
bool CAudience::m_bSuccess = false;

//==================================================
// コンストトラクタ
//==================================================
CAudience::CAudience()
{
	// 各メンバ変数の初期化
	m_Animetype = ANIMETYPE_NONE;			// アニメーションの種類
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_nCntFream = 0;						// フレームカウント
	m_fJump = 0.0f;							// ジャンプ量
	m_fSpeed = 0.0f;						// 移動スピード

	m_bMiss = false;
	m_bSuccess = false;
}

//==================================================
// デストラクタ
//==================================================
CAudience::~CAudience()
{
	// なし
}

//==================================================
// 生成処理
//==================================================
CAudience* CAudience::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fJump, float fSpeed)
{
	CAudience* pAudience = nullptr;

	// メモリの確保
	pAudience = new CAudience;

	if (pAudience == nullptr)return nullptr;

	// 各メンバ変数の設定
	pAudience->m_fJump = fJump;
	pAudience->m_fSpeed = fSpeed;

	int nFrame;
	nFrame = rand() % 30;
	pAudience->m_nCntFream -= nFrame;

	// 高さ調整
	pos.y += size.y / 2.0f;
	pAudience->m_fBasePosY = pos.y;

	pAudience->CObjectBillboard::SetPosition(pos);
	pAudience->CObjectBillboard::SetSize(size);
	
	// 初期化処理
	pAudience->Init();

	// ポインタを返す
	return pAudience;
}

//==================================================
// 初期化処理
//==================================================
HRESULT CAudience::Init(void)
{
	// テクスチャの設定
	CObjectBillboard::SetTextureID(AUDIENCE_TEX_PASS);

	CObjectBillboard::Init();

	return S_OK;
}

//==================================================
// 終了処理
//==================================================
void CAudience::Uninit(void)
{
	// ビルボードの終了処理
	CObjectBillboard::Uninit();
}

//==================================================
// 更新処理
//==================================================
void CAudience::Update(void)
{

#ifdef _DEBUG

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard != nullptr && pKeyboard->GetTrigger(DIK_1))
	{
		SetAnimation(ANIMETYPE_SUCCESS);
	}
	if (pKeyboard != nullptr && pKeyboard->GetTrigger(DIK_2))
	{
		SetAnimation(ANIMETYPE_MISTAKE);
	}

#endif

	if (m_bMiss == true)
	{
		SetAnimation(ANIMETYPE_MISTAKE);
	}
	if (m_bSuccess == true)
	{
		SetAnimation(ANIMETYPE_SUCCESS);
	}
	Animation();
}

//==================================================
// 描画処理
//==================================================
void CAudience::Draw(void)
{
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ALPHAテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ビルボードの描画処理
	CObjectBillboard::Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//==================================================
// アニメーションの種類の設定
//==================================================
void CAudience::SetAnimation(ANIMETYPE animetype)
{
	if (m_Animetype != animetype)
	{
		// アニメーションの種類の設定
		m_Animetype = animetype;

		// フレームカウンターのリセット
		m_nCntFream = 0;

		if (animetype == ANIMETYPE_SUCCESS)
		{
			int nFrame;
			nFrame = rand() % 30;

			m_nCntReaction = nFrame;
		}
	}
}

//==================================================
// アニメーション
//==================================================
void CAudience::Animation(void)
{
	switch (m_Animetype)
	{
	case ANIMETYPE_NONE:
		m_bMiss = false;
		m_bSuccess = false;

		CObjectBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case ANIMETYPE_SUCCESS:

		// 着地と同時に再度ジャンプ
		if (CObjectBillboard::GetPosition().y <= m_fBasePosY)
		{
			if (m_nCntReaction <= 0)
			{
				m_bJump = true;

				int nFrame;
				nFrame = rand() % 30;
				m_nCntReaction = nFrame;
			}
		}

		if (m_nCntFream >= AUDIENCE_SUCANDMIS)
		{
			// アニメーション種類の設定（通常へ）
			m_Animetype = ANIMETYPE_NONE;

			// フレームカウンターのリセット(この後インクリメントするので-1)
			m_nCntFream = -1;

			m_bSuccess = false;
		}

		break;

	case ANIMETYPE_MISTAKE:

		Miss();

		if (m_nCntFream >= AUDIENCE_SUCANDMIS)
		{
			// アニメーション種類の設定（通常へ）
			m_Animetype = ANIMETYPE_NONE;

			// フレームカウンターのリセット(この後インクリメントするので-1)
			m_nCntFream = -1;

			m_bMiss = false;
		}

		break;

	default:

		break;
	}

	if (m_Animetype != ANIMETYPE_NONE)
	{
		// フレームカウントアップ
		++m_nCntFream;
	}

	m_nCntReaction--;

	// 位置の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();

	// ジャンプアニメーション
	isJump();

	float fGravity = AUDIENCE_GRAVITY;
	// 重力をかける
	m_move.y -= fGravity;

	// プレイヤー取得
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		// 当たり判定の取得
		CCollisionObject3D* pCollision = pPlayer->GetCollision();

		// 空間情報の取得
		CTransform* pTransform = pCollision->GetTransform();

		// 情報の取得
		CTransform::Info transform = pTransform->GetInfo();
	}

	pos += m_move;

	if (m_fBasePosY >= pos.y)
	{
		pos.y = m_fBasePosY;
		m_move.y = 0.0f;
	}

	// 位置の更新
	CObjectBillboard::SetPosition(pos);
}

//==================================================
// ジャンプのアニメーション
//==================================================
void CAudience::isJump(void)
{
	if (m_bJump)
	{
		if (CObjectBillboard::GetPosition().y <= m_fBasePosY)
		{
			m_move.y += m_fJump;

			// フラグ下げる
			m_bJump = false;
		}
	}
}
//==================================================
// ミスした時の処理
//==================================================
void CAudience::Miss(void)
{
	// 色変更
	CObjectBillboard::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.75f, 0.25f));
	// ジャンプさせない
	m_bJump = false;
}

//===================================================
// プレイヤーの上手さを調べる
//===================================================
void CAudience::SearchSkill(void)
{

}
