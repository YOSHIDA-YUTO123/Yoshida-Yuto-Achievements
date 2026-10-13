//===================================================
//
// オブジェクトXファイル [objectX.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "modelManager.h"
#include "textureManager.h"

//===================================================
// コンストラクタ
//===================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	memset(m_mtxWorld, NULL, sizeof(D3DXMATRIX));
	m_nModelIdx = -1;
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	m_max = Const::VEC3_NULL;	// 最大値
	m_min = Const::VEC3_NULL;	// 最小値
}

//===================================================
// デストラクタ
//===================================================
CObjectX::~CObjectX()
{
}

//===================================================
// 生成処理
//===================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const char* pModelName,const D3DXVECTOR3 rot)
{
	CObjectX* pObjectX = nullptr;

	// 3Dオブジェクトの生成
	pObjectX = new CObjectX;

	if (pObjectX == nullptr) return nullptr;

	pObjectX->Init();
	pObjectX->m_pos = pos;
	pObjectX->m_rot = rot;
	pObjectX->LoadModel(pModelName);

	return pObjectX;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CObjectX::Init(void)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CObjectX::Uninit(void)
{	
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CObjectX::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CObjectX::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向き
	D3DXVECTOR3 rot = m_rot;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

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
// 描画処理(オーバーロード)
//===================================================
void CObjectX::Draw(const float Diffuse)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetTextureManager();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向き
	D3DXVECTOR3 rot = m_rot;

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse.a = Diffuse;

		//マテリアルの設定
		pDevice->SetMaterial(&Mat.MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		//モデル(パーツ)の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// 影の描画処理
//===================================================
void CObjectX::DrawShadow(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// ライトの方向
	D3DXVECTOR4 lightDir = D3DXVECTOR4(0.707f, -0.707f, 0.707f, 0.0f);

	// 影を落とす地面
	D3DXPLANE plane = D3DXPLANE(0, -1.0f, 0, 0);

	// 影用のマトリックスを作成
	D3DXMATRIX mtxShadow, mtxShadowWorld;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	// 影用のマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &lightDir, &plane);

	// 影のマトリックスと自分のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&mtxShadowWorld, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadowWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 半透明の黒マテリアルをセット
	D3DMATERIAL9 matShadow = {};

	// 影の色の設定
	matShadow.Diffuse.r = 0.0f;
	matShadow.Diffuse.g = 0.0f;
	matShadow.Diffuse.b = 0.0f;
	matShadow.Diffuse.a = 1.0f;

	// 影の描画
	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		// 影のマテリアルの設定
		pDevice->SetMaterial(&matShadow);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// 影の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}
	
	// マテリアルをもとに戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// ワールドマトリックスの設定
//===================================================
void CObjectX::SetUpMatrix(const D3DXVECTOR3 Scal)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向き
	D3DXVECTOR3 rot = m_rot;

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, Scal.x, Scal.y, Scal.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScal);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// ワールドマトリックスの設定
//===================================================
void CObjectX::SetUpMatrix(const D3DXMATRIX mtxRot, const D3DXMATRIX mtxTrans, const D3DXVECTOR3 Scal)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックス
	D3DXMATRIX mtxScal;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, Scal.x, Scal.y, Scal.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScal);

	// 向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// 描画までの設定処理
//===================================================
void CObjectX::SetUpDraw(const float Diffuse)
{
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
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		if (modelInfo.nTextureIdx.empty())
		{
			break;
		}

		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse.a = Diffuse;

		//マテリアルの設定
		pDevice->SetMaterial(&Mat.MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		//モデル(パーツ)の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// モデルのロード処理
//===================================================
HRESULT CObjectX::LoadModel(const char* pXFileName)
{
	// 省略用ファイルパス
	std::string Path = pXFileName;
	std::string filePath;

	if (Path.find("data/MODEL/") == std::string::npos)
	{
		filePath = "data/MODEL/";
	}

	// 文字列の連結
	filePath += pXFileName;

	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// モデルのIDの取得
	m_nModelIdx = pModelManager->Register(filePath.c_str());

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	// 最大値と最小値を作成する
	CreateMaxMin();

	return S_OK;
}

//===================================================
// 大きさの取得
//===================================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr || m_nModelIdx == -1)
	{
		return Const::VEC3_NULL;
	}

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// 大きさを返す
	return modelInfo.Size;
}

//===================================================
// 最大値と最小値の取得
//===================================================
void CObjectX::CreateMaxMin()
{
	// マップのオブジェクトの取得
	CModelManager* pModelManager = CManager::GetModelManager();

	// モデルマネージャーの取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// 頂点フォーマットのサイズを取得
	WORD sizeFVF = (WORD)D3DXGetFVFVertexSize(modelInfo.pMesh->GetFVF());

	// メッシュの頂点数の取得
	int nNumVtx = modelInfo.pMesh->GetNumVertices();

	// 頂点バッファのロック
	BYTE* pVtxBuff = NULL;
	modelInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	
	// 頂点の数分回す
	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3 pos = *(D3DXVECTOR3*)pVtxBuff;

		if (m_max.x < pos.x)
		{
			m_max.x = pos.x;
		}
		
		if (m_max.y < pos.y)
		{
			m_max.y = pos.y;
		}
		
		if (m_max.z < pos.z)
		{
			m_max.z = pos.z;
		}

		if (m_min.x > pos.x)
		{
			m_min.x = pos.x;
		}
			
		if (m_min.y > pos.y)
		{
			m_min.y = pos.y;
		}
			
		if (m_min.z > pos.z)
		{
			m_min.z = pos.z;
		}

		// サイズ分ずらす
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファのアンロック
	modelInfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
}

