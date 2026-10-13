//==================================================
// 
// 観客マネージャー [AudienceManager.cpp]
// Author：Aoto Kaichi
// 
//==================================================

// インクルード
#include "AudienceManager.h"
#include "player.h"
#include "game.h"
#include "transform.h"
#include "object3D.h"

//===============================================
// コンストラクタ
//===============================================
CAudienceManager::CAudienceManager()
{
}
//===============================================
// デストラクタ
//===============================================
CAudienceManager::~CAudienceManager()
{
}
//===============================================
// 生成
//===============================================
CAudienceManager* CAudienceManager::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fJump, float fSpeed, const bool bLight_Off)
{
	CAudienceManager* pManager = new CAudienceManager;

	// 左から観客を生成
	for (int nCnt = 0; nCnt < MAX_AUDIENCE; nCnt++)
	{
		pManager->m_pAudiences[nCnt] = CAudience::Create(pos, size, fJump, fSpeed);
		pos.x += AUDIENCE_SPACE;
	}

    // 客席
    CObject3D* pWallAudience = CObject3D::Create(D3DXVECTOR3(0.0f, pos.y - size.y / 2.0f, pos.z - 15.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3000.0f, 30.0f, 0.0f), "Audience_Back01.jpg", bLight_Off);
    pWallAudience->SetTexture(0.0f, 0.0f, 25.0f, 1.0f);


	return pManager;
}
//===============================================
// 初期化
//===============================================
HRESULT CAudienceManager::Init(void)
{
	return S_OK;
}
//===============================================
// 終了
//===============================================
void CAudienceManager::Uninit(void)
{
	CObject::Release();
}
//===============================================
// 更新
//===============================================
void CAudienceManager::Update(void)
{
	// プレイヤー取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pPlayer != nullptr)
	{
		CCollisionObject3D* pCollision = pPlayer->GetCollision();

		// 空間情報の取得
		CTransform* pTransform = pCollision->GetTransform();

		// 情報の取得
		CTransform::Info transform = pTransform->GetInfo();
		PlayerPos = transform.pos;


        // 左端と右端を取得
        int nLeftIndex = 0;
        int nRightIndex = 0;
        float fLeftX = FLT_MAX;
        float fRightX = -FLT_MAX;

        for (int nCnt = 0; nCnt < MAX_AUDIENCE; nCnt++)
        {
            float x = m_pAudiences[nCnt]->GetPosition().x;

            if (x < fLeftX) {
                fLeftX = x;
                nLeftIndex = nCnt;
            }
            if (x > fRightX) {
                fRightX = x;
                nRightIndex = nCnt;
            }
        }

        // 現在のラインの中心
        float centerX = (fLeftX + fRightX) * 0.5f;

        float threshold = AUDIENCE_SPACE * 0.5f;

        // 右方向へ進んだ場合
        if (PlayerPos.x > centerX + threshold)
        {
            D3DXVECTOR3 pos = m_pAudiences[nLeftIndex]->GetPosition();
            pos.x = fRightX + AUDIENCE_SPACE;
            m_pAudiences[nLeftIndex]->SetPosition(pos);
        }

        // 左方向へ進んだ場合
        if (PlayerPos.x < centerX - threshold)
        {
            D3DXVECTOR3 pos = m_pAudiences[nRightIndex]->GetPosition();
            pos.x = fLeftX - AUDIENCE_SPACE;
            m_pAudiences[nRightIndex]->SetPosition(pos);
        }
	}

}

//===============================================
// 描画
//===============================================
void CAudienceManager::Draw(void)
{
}

//===============================================
// 観客の設定
//===============================================
void CAudienceManager::SetAnimation(CAudience::ANIMETYPE animetype)
{
    for (int nCnt = 0; nCnt < MAX_AUDIENCE; nCnt++)
    {
        m_pAudiences[nCnt]->SetAnimation(animetype);
    }
}