//===================================================
//
// ライト [light.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "light.h"
#include"manager.h"
#include"renderer.h"

//===================================================
// コンストラクタ
//===================================================
CLight::CLight() : 
	m_aLightInfo()
{
}

//===================================================
// デストラクタ
//===================================================
CLight::~CLight()
{
	
}

//===================================================
// 初期化処理
//===================================================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトを無効化
		pDevice->LightEnable(nCnt, FALSE);

		ZeroMemory(&m_aLightInfo[nCnt].aLight, sizeof(m_aLightInfo[nCnt].aLight));
		m_aLightInfo[nCnt].bUse = false;
		m_aLightInfo[nCnt].nID = -1;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CLight::Uninit(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトを無効化
		pDevice->LightEnable(nCnt, FALSE);
	}
}

//===================================================
// 更新処理
//===================================================
void CLight::Update(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nCnt = 0;

	for (auto& light : m_aLightInfo)
	{
		if (light.bUse)
		{
			// ライトの設定
			pDevice->SetLight(nCnt, &light.aLight);
		}

		nCnt++;
	}
}

//===================================================
// ライトの設定処理
//===================================================
void CLight::SetDirectional(const D3DXCOLOR& Diffuse,const D3DXVECTOR3& dir,const D3DXVECTOR3& pos)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライトの情報
	LightInfo info = {};

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// 使われていないなら
		if (m_aLightInfo[nCnt].bUse == false)
		{
			info.aLight.Type = D3DLIGHT_DIRECTIONAL;
			info.aLight.Diffuse = Diffuse;

			// ライトの方向ベクトル
			D3DXVECTOR3 Direction = dir;

			// 正規化する
			D3DXVec3Normalize(&Direction, &Direction);

			// 方向ベクトルの代入
			info.aLight.Direction = Direction;

			// 位置の設定
			info.aLight.Position = pos;

			info.bUse = true;

			// IDの設定
			info.nID = nCnt;

			// ライトの設定
			pDevice->SetLight(nCnt, &info.aLight);

			// ライトを有効化
			pDevice->LightEnable(nCnt, TRUE);

			// 情報を設定
			m_aLightInfo[nCnt] = info;

			break;
		}
	}
}

//===================================================
// ポイントライトの設定処理
//===================================================
int CLight::SetPoint(const D3DXVECTOR3& pos, const float fRange, const D3DXCOLOR& Diffuse, const D3DXCOLOR& Ambient)
{
	int nID = -1;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライトの情報
	LightInfo info = {};

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// 使われていないなら
		if (m_aLightInfo[nCnt].bUse == false && m_aLightInfo[nCnt].nID == -1)
		{
			info.aLight.Type = D3DLIGHT_POINT;
			info.aLight.Diffuse = Diffuse;

			// ライトの位置
			info.aLight.Position = pos;

			info.aLight.Diffuse = Diffuse;

			info.aLight.Ambient = Ambient;

			info.aLight.Specular = Diffuse;
			info.aLight.Attenuation0 = 0.0f;
			info.aLight.Attenuation1 = 0.002f;
			info.aLight.Attenuation2 = 0.00f;
			info.aLight.Range = fRange;

			info.bUse = true;

			nID = nCnt;

			// 自分のIDの設定
			info.nID = nCnt;

			// ライトの設定
			pDevice->SetLight(nCnt, &info.aLight);

			// ライトを有効化
			pDevice->LightEnable(nCnt, TRUE);

			// ライトの情報の設定
			m_aLightInfo[nCnt] = info;

			break;
		}
	}

	return nID;
}

//===================================================
// ライトの設定処理
//===================================================
void CLight::SetLight(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトの設定
		pDevice->SetLight(nCnt, &m_aLightInfo[nCnt].aLight);

		// ライトを有効化
		pDevice->LightEnable(nCnt, TRUE);
	}
}

//===================================================
// ライトの消去
//===================================================
void CLight::DeleteLight(const int nID)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライト分回す
	for (auto& light : m_aLightInfo)
	{
		// IDが一致していたら
		if (light.nID == nID)
		{
			light.bUse = false;

			// ライトを無効化
			pDevice->LightEnable(light.nID, FALSE);

			light.nID = -1;

			break;
		}
	}
}