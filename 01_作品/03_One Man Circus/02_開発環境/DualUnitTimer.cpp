//================================================
//
// 分と秒を扱うタイマーのクラスの処理 [DualUnitTimer.cpp]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// インクルードファイル
//*************************************************
#include "DualUnitTimer.h"
#include "object2D.h"
#include "score.h"
#include "manager.h"
#include "game.h"
#include "result.h"
#include "GameManager.h"

//*************************************************
// 名前空間
//*************************************************
using namespace Const; // 名前空間Constの使用
using namespace std;   // 名前空間stdの使用

//=================================================
// コンストラクタ
//=================================================
CDualUnitTimer::CDualUnitTimer() :
    m_pGameManagerRef(nullptr),
    m_bLess(true),
    m_nCurrentTime(NULL),
    m_nCounter(NULL),
    m_pMinute(nullptr),
    m_pCoron(nullptr)
{
}

//=================================================
// デストラクタ
//=================================================
CDualUnitTimer::~CDualUnitTimer()
{
}

//=================================================
// 初期化処理
//=================================================
CDualUnitTimer* CDualUnitTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const int nTime, CGameManager* pGameManager)
{
    CDualUnitTimer* pTimer = new CDualUnitTimer;

    pTimer->SetPosition(pos);
    pTimer->SetSize(Size);
    pTimer->SetTime(nTime % FRAME);
    pTimer->m_pGameManagerRef = pGameManager;

    // タイマークラスの生成
    pTimer->m_pMinute = new CTimer;

    // 分の設定
    pTimer->m_pMinute->SetTime(nTime / FRAME);

    // 初期化処理
    if (FAILED(pTimer->Init()))
    {
        pTimer->Uninit();
        pTimer = nullptr;
        return nullptr;
    }

    return pTimer;
}

//=================================================
// 初期化処理
//=================================================
HRESULT CDualUnitTimer::Init(void)
{
    // 初期化処理
    if (FAILED(CTimer::Init()))
    {
        return E_FAIL;
    }

    // テクスチャのIDの設定
    CTimer::SetTexture("number001.png");

    // 大きさの取得
    D3DXVECTOR2 Size = CTimer::GetSize();

    // 位置の取得
    D3DXVECTOR3 pos = CTimer::GetPosition();

    // コロンの位置X
    float fCoronOffPosX = pos.x - Size.x * 2.0f;

    // コロンの生成
    m_pCoron = CObject2D::Create(Size.x * 0.25f, Size.y, D3DXVECTOR3(fCoronOffPosX, pos.y, pos.z));
    if (m_pCoron != nullptr)
    {
        m_pCoron->SetTextureID("Coron_White.png");
    }

    // 分の横のオフセットの位置を求める
    float fMinuteOffPosX = pos.x - Size.x * 3.0f;

    if (m_pMinute != nullptr)
    {
        // 位置の設定
        m_pMinute->SetPosition(D3DXVECTOR3(fMinuteOffPosX, pos.y, pos.z));
        m_pMinute->SetSize(Size);

        // 初期化処理
        if (FAILED(m_pMinute->Init()))
        {
           m_pMinute->Uninit();
           m_pMinute = nullptr;

            return E_FAIL;
        }

        // テクスチャのIDの設定
        m_pMinute->SetTexture("number001.png");
    }

    m_bLess = true;

    return S_OK;
}

//=================================================
// 終了処理
//=================================================
void CDualUnitTimer::Uninit(void)
{
    if (m_pGameManagerRef != nullptr)
    {
        int nTimer = 0;

        // 秒の取得
        int nSecond = CTimer::GetTime();

        // 分の取得
        int nMinute = m_pMinute->GetTime();

        nTimer = nSecond + (nMinute * 60);

        // タイマーのセーブ
        m_pGameManagerRef->SaveTime(CGameManager::SAVE_TIMER_FILE, nTimer);
    }

    // 終了処理
    CTimer::Uninit();
}

//=================================================
// 更新処理
//=================================================
void CDualUnitTimer::Update(void)
{
    if (m_bLess == true)
    {
        if (m_pMinute == nullptr) return;

        m_nCounter++;

        // 秒の取得
        int nSecond = CTimer::GetTime();

        // 分の取得
        int nMinute = m_pMinute->GetTime();

        if (m_nCounter >= FRAME)
        {
            // 秒が0じゃなかったら
            if (nSecond > 0)
            {
                // 秒を減らす
                nSecond--;
            }
            // 分が0じゃなかったら
            else if (nSecond <= 0 && nMinute != 0)
            {
                // もう一周する
                nSecond = FRAME - 1;

                // 分を減らす
                nMinute--;
            }
            m_nCounter = 0;
        }

        // 60を超えたら
        if (nSecond >= FRAME)
        {
            nMinute++; // 分を1増やす

            // 余りが0じゃなかったら(60より上だったら)
            if (nSecond % FRAME != 0)
            {
                int Time = nSecond % FRAME; // 余りを求める

                nSecond = 0; // 0にする
                nSecond += Time; // 余りを加算する
            }
            else
            {
                nSecond = 0; // 0にする
            }
        }

        m_nCurrentTime = nSecond + (nMinute * FRAME);

        if (m_pGameManagerRef != nullptr)
        {
            // 現在の時間の取得
            m_pGameManagerRef->SetCurrentTime(m_nCurrentTime);
        }

        // 秒の設定
        CTimer::SetTime(nSecond);

        // 分の設定
        m_pMinute->SetTime(nMinute);

        // 更新処理
        m_pMinute->Update();

        // 更新処理
        CTimer::Update();
    }
}

//=================================================
// 描画処理
//=================================================
void CDualUnitTimer::Draw(void)
{
    // 描画処理
    CTimer::Draw();

    // 描画処理
    if (m_pMinute != nullptr)
    {
        m_pMinute->Draw();
    }
}
//===============================================
// 保存
//===============================================
void CDualUnitTimer::Save(const char* pFilename)
{
    int nTimer = 0;

    // 秒の取得
    int nSecond = CTimer::GetTime();

    // 分の取得
    int nMinute = m_pMinute->GetTime();

    nTimer = nSecond + (nMinute * 60);

    FILE* pFile;
    //ファイルに書き込む
    pFile = fopen(pFilename, "w");
    if (pFile != NULL)
    {
        // 今回のスコアを保存
        fprintf(pFile, "%d", nTimer);
        // 閉じる
        fclose(pFile);
    }
    else
    {// エラーチェック
    }
}

//===============================================
//両方のテクスチャを差し替える処理
//===============================================
void CDualUnitTimer::SetTextureAll(const char* pTexturePath, const char* pCoronTexture)
{
    // 秒（自身）の数値テクスチャを差し替え
    CTimer::SetTexture(pTexturePath);

    // 分タイマーが存在すればそちらも差し替え
    if (m_pMinute != nullptr)
    {
        m_pMinute->SetTexture(pTexturePath);
    }

    // コロンを差し替える（指定があれば）
    if (pCoronTexture != nullptr && m_pCoron != nullptr)
    {
        m_pCoron->SetTextureID(pCoronTexture);
    }
}