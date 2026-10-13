//===================================================
//
// オープニングのボールの処理 [OpeningBall.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "OpeningBall.h"

//***************************************************
// 定数宣言
//***************************************************
namespace OpeningBall_Constants
{
	const char* MODEL_PATH		= "MapObjectFile/ball.x";	// モデルのパス
	const D3DXVECTOR3 MOVE_DIR	= { 1.0f,0.0f,0.0f };		// 移動方向
	constexpr float MOVE_SPEED	= 1.3f;						// 移動スピード			
}

//===================================================
// コンストラクタ
//===================================================
COpeningBall::COpeningBall() :
	m_bActive(false),
	m_move(Const::VEC3_NULL)
{
}

//===================================================
// デストラクタ
//===================================================
COpeningBall::~COpeningBall()
{
}

//===================================================
// 生成処理
//===================================================
COpeningBall* COpeningBall::Create(const D3DXVECTOR3& pos)
{
	COpeningBall* pInstance = new COpeningBall;

	// 位置の設定
	pInstance->SetPosition(pos);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 破棄
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT COpeningBall::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}

	// モデルのロード
	if (FAILED(CObjectX::LoadModel(OpeningBall_Constants::MODEL_PATH)))
	{
		return E_FAIL;
	}

	// 移動量の設定
	m_move = OpeningBall_Constants::MOVE_DIR * OpeningBall_Constants::MOVE_SPEED;

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void COpeningBall::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//===================================================
// 更新処理
//===================================================
void COpeningBall::Update(void)
{
	if (m_bActive)
	{
		// 位置の取得
		D3DXVECTOR3 pos = CObjectX::GetPosition();

		// 向きの取得
		D3DXVECTOR3 rot = CObjectX::GetRotation();

		// 位置の更新
		pos += m_move;

		// 回転させる
		rot.z += D3DXToRadian(-1);

		// 位置の設定
		CObjectX::SetPosition(pos);

		// 向きの設定
		CObjectX::SetRotation(rot);
	}
}

//===================================================
// 描画処理
//===================================================
void COpeningBall::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}
