//===================================================
//
// キャラクター3D [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character3D.h"
#include"manager.h"
#include"renderer.h"
#include"math.h"
#include"shadowS.h"
#include"model.h"
#include"motion.h"
#include<string>
#include "CharacterManager.h"

using namespace math;	// 名前空間mathを使用
using namespace Const;	// 名前空間Constを使用
using namespace std;	// 名前空間stdを使用

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D()
{
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_ShadowScal = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_nHitStopTime = NULL;
	m_rotDest = VEC3_NULL;
}

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D(const TYPE type)
{
	m_rotDest = VEC3_NULL;
	m_type = type;
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_ShadowScal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nHitStopTime = NULL;
}

//===================================================
// デストラクタ
//===================================================
CCharacter3D::~CCharacter3D()
{
	for (auto& model : m_apModel)
	{
		// モデルの破棄
		if (model != nullptr)
		{
			// 終了処理
			model->Uninit();

			delete model;

			model = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		// モーションの終了処理
		m_pMotion->Uninit();
	}

	// 影の破棄
	if (m_pShadowS != nullptr)
	{
		m_pShadowS = nullptr;
	}
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CCharacter3D> CCharacter3D::Create(const char* pMotionFileName, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nNumMotion)
{
	auto pInstance = std::make_unique<CCharacter3D>();

	pInstance->m_pos = pos;
	pInstance->m_rot = rot;
	
	// モーションのロード処理
	pInstance->LoadMotion(pMotionFileName, nNumMotion);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		// 破棄
		pInstance.reset();
		return nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCharacter3D::Init(void)
{
	// 影の生成
	m_pShadowS = CShadowS::Create(m_pos,&m_ShadowScal);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCharacter3D::Uninit(void)
{

}

//===================================================
// 更新処理
//===================================================
void CCharacter3D::Update(void)
{
	// 状態の遷移
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_MOVE:
		break;
	case STATE_ACTION:

		// 状態のカウンターを減らす
		m_nCounterState--; 

		// カウンターが0になったら
		if (m_nCounterState <= 0)
		{
			// 状態をノーマルに戻す
			m_state = STATE_NORMAL;
		}

		break;
	case STATE_DAMAGE:

		// 状態のカウンターを減らす
		m_nCounterState--; 

		// カウンターが0になったら
		if (m_nCounterState <= 0)
		{
			// 状態をノーマルに戻す
			m_state = STATE_NORMAL;
		}

		break;
	default:
		break;
	}


	if (m_pShadowS != nullptr)
	{
		m_pShadowS->SetPosition(m_pos);
	}

	// 差分
	float Diff = m_rotDest.y - m_rot.y;

	// 角度の正規化
	NormalizeDiffRot(Diff, &m_rot.y);

	// 目的の角度に近づける
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.3f;
}

//===================================================
// 描画処理
//===================================================
void CCharacter3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (auto &model : m_apModel)
	{
		if (model != nullptr)
		{
			// 描画処理
			model->Draw();
		}
	}
}

//===================================================
// 計算後のマトリックスを設定できる描画処理
//===================================================
void CCharacter3D::Draw(const D3DXMATRIX mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	m_mtxWorld = mtxWorld;

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (auto& model : m_apModel)
	{
		if (model != nullptr)
		{
			// 描画処理
			model->Draw();
		}
	}
}

//===================================================
// 描画処理(透明度設定)
//===================================================
void CCharacter3D::Draw(const float fAvl)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きの取得
	D3DXVECTOR3 rot = m_rot;

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->Draw(fAvl);
		}
	}
}

//===================================================
// モーションのロード
//===================================================
CMotion* CCharacter3D::LoadMotion(const char* pFileName,const int nNumMotion)
{
	// 省略用
	std::string string = "data/MOTION/";

	// 文字列を合成
	string += pFileName;

	// キャラクターのマネージャーの取得
	auto pCharacterManager = CCharacterManager::GetInstance();

	if (pCharacterManager == nullptr) return nullptr;

	// 自分の種類の取得
	TYPE type = GetType();

	// モーションのロード処理
	m_pMotion = CMotion::Load(string.c_str(), m_apModel, &m_nNumModel, nNumMotion, CMotion::LOAD_TEXT);

	// モーションの設定
	pCharacterManager->SetMotion(m_pMotion.get(), type);

	int nCnt = 0;

	// モデルの要素分調べる
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); ++itr)
	{
		// nullなら処理しない
		if ((*itr) == nullptr) continue;

		// モーションの設定
		pCharacterManager->SetModel((*itr), type, m_nNumModel,nCnt);

		nCnt++;
	}

	return m_pMotion.get();
}

//===================================================
// モデルの位置の取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelPos(const int nIdx)
{
	// モデルのマトリックスの取得
	D3DXMATRIX mtx = m_apModel[nIdx]->GetMatrixWorld();

	// ワールドマトリックスの位置の取得
	D3DXVECTOR3 modelPos = GetPositionFromMatrix(mtx);

	return modelPos;
}

//===================================================
// モデルの向きの取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelRot(const int nIdx)
{
	// モデルの位置の取得
	D3DXVECTOR3 modelRot = m_apModel[nIdx]->GetRotation();

	return modelRot;
}

//===================================================
// モデルの大きさの取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelSize(const int nIdx)
{
	// モデルの大きさの取得
	D3DXVECTOR3 modelSize = m_apModel[nIdx]->GetSize();

	return modelSize;
}

//===================================================
// キャラクターの設定処理
//===================================================
void CCharacter3D::SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 ShadowScal, const D3DXVECTOR3 Size)
{
	m_nLife = nLife;
	m_fSpeed = fSpeed;
	m_ShadowScal = ShadowScal;
	m_Size = Size;
}

//===================================================
// キャラクターのヒット処理
//===================================================
bool CCharacter3D::Hit(int nDamage)
{
	// 体力を減らす
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{
		m_state = STATE_DEATH;

		return false;
	}

	return true;
}

//===================================================
// 生きているかどうか
//===================================================
bool CCharacter3D::GetAlive(void)
{
	// 死んでいるなら
	if (m_nLife <= 0)
	{
		return false;
	}

	return true;
}

//===================================================
// ヒットストップの設定処理
//===================================================
bool CCharacter3D::HitStop(void)
{
	// カウンターを減らす
	m_nHitStopTime--;

	// 0以下だったら
	if (m_nHitStopTime <= 0)
	{
		// ヒットストップしていない
		return false;
	}
	return true;
}

//===================================================
// モーションの更新処理
//===================================================
void CCharacter3D::UpdateMotion(void)
{
	if (m_pMotion != nullptr)
	{
		// モーションの更新処理
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}
}

//===================================================
// 状態の設定処理
//===================================================
void CCharacter3D::SetState(const STATE state, const int nTime)
{
	m_state = state;
	m_nCounterState = nTime;
}

//===================================================
// 影の消去
//===================================================
void CCharacter3D::DeleteShadow(void)
{
	// 影の破棄
	if (m_pShadowS != nullptr)
	{
		m_pShadowS->Uninit();
		m_pShadowS = nullptr;
	}
}
