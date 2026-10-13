//===================================================
//
// 当たり判定をするマップオブジェクトクラス [CollisionMapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "CollisionMapObject.h"
#include "CollisionObject3D.h"
#include "transform.h"
#include "Collider.h"
#include "modelManager.h"
#include "manager.h"
#include <string>
#include "renderer.h"
#include "textureManager.h"
#include "camera.h"
#include "player.h"
#include "game.h"

//===================================================
// コンストラクタ
//===================================================
CCollisionMapObject::CCollisionMapObject() :
	m_pCollisionObj(nullptr),
	m_nModelID(-1),
	m_nLightID(-1)
{
	m_bRanding = false;	// 乗ったことがあるかどうか
}

//===================================================
// デストラクタ
//===================================================
CCollisionMapObject::~CCollisionMapObject()
{
}

//===================================================
// 生成処理
//===================================================
CCollisionMapObject* CCollisionMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName)
{
	CCollisionMapObject* pInstance = new CCollisionMapObject;

	// 当たり判定オブジェクトの生成
	pInstance->m_pCollisionObj = CCollisionObject3D::Create();

	// 空間情報の取得
	CTransform* pTransform = pInstance->m_pCollisionObj->GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	transform.pos = pos;
	transform.rot = rot;

	pTransform->SetInfo(transform);

	if (FAILED(pInstance->LoadModel(pModelFileName)))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}
	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCollisionMapObject::Init(void)
{
	// モデルマネージャーの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	if (pModelManager == nullptr)
	{
		return E_FAIL;
	}

	CTransform::Info transform;

	transform = m_pCollisionObj->GetTransform()->GetInfo();

	D3DXVECTOR3 Size = pModelManager->GetModelInfo(m_nModelID).Size;

	if (transform.rot.y != 0.0f)
	{
		D3DXVECTOR3 newSize;

		D3DXMATRIX mtxRot;

		D3DXMatrixIdentity(&mtxRot);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, transform.rot.y, transform.rot.x, transform.rot.z);

		// 回転軸を取得
		D3DXVECTOR3 right(mtxRot._11, mtxRot._12, mtxRot._13);
		D3DXVECTOR3 up(mtxRot._21, mtxRot._22, mtxRot._23);
		D3DXVECTOR3 front(mtxRot._31, mtxRot._32, mtxRot._33);

		// 回転軸から新しいサイズを計算
		newSize.x = fabsf(Size.x * right.x) + fabsf(Size.y * up.x) + fabsf(Size.z * front.x);
		newSize.y = fabsf(Size.x * right.y) + fabsf(Size.y * up.y) + fabsf(Size.z * front.y);
		newSize.z = fabsf(Size.x * right.z) + fabsf(Size.y * up.z) + fabsf(Size.z * front.z);

		// 計算後のサイズを設定
		transform.Size = newSize;
	}
	else
	{
		// 計算後のサイズを設定
		transform.Size = Size;
	}

	m_pCollisionObj->GetTransform()->SetInfo(transform);

	m_pCollisionObj->AddCollider(CColliderAABB::Create(Const::VEC3_NULL));

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCollisionMapObject::Uninit(void)
{
	// 終了処理
	if (m_pCollisionObj != nullptr)
	{
		m_pCollisionObj->Uninit();
		m_pCollisionObj.reset();
	}

	// 終了処理
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CCollisionMapObject::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = m_pCollisionObj->GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	transform.posOld = transform.pos;

	// 更新処理
	if (m_pCollisionObj != nullptr)
	{
		m_pCollisionObj->Update();
	}

	pTransform->SetInfo(transform);
}

//===================================================
// 描画処理
//===================================================
void CCollisionMapObject::Draw(void)
{
	// プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// マトリックスの処理
	if (m_pCollisionObj != nullptr)
	{
		m_pCollisionObj->SetMatrix();
	}

	if (pPlayer != nullptr)
	{
		// 位置の取得
		D3DXVECTOR3 pos = m_pCollisionObj->GetTransform()->GetInfo().pos;

		// カメラからの一定の範囲外だったら描画しない
		if (pPlayer->GetDistance(pos) >= 500.0f)
		{
			return;
		}
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelID == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelID);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		if (modelInfo.nTextureIdx.empty())
		{
			break;
		}

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		//モデル(パーツ)の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// モデルのファイル名
//===================================================
HRESULT CCollisionMapObject::LoadModel(const char* pModelFileName)
{
	// 省略用ファイルパス
	std::string Path = pModelFileName;
	std::string filePath;
	m_Xfilepath = pModelFileName;

	if (Path.find("data/MODEL/") == std::string::npos)
	{
		filePath = "data/MODEL/";
	}

	// 文字列の連結
	filePath += pModelFileName;

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// モデルのIDの取得
	m_nModelID = pModelManager->Register(filePath.c_str());

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelID == -1)
	{
		return E_FAIL;
	}

	return S_OK;
}
