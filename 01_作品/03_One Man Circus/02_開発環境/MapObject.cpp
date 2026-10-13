//================================================
//
// マップのオブジェクトクラス [MapObject.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "MapObject.h"

//================================================
// コンストラクタ
//================================================
CMapObject::CMapObject()
{
}

//================================================
// デストラクタ
//================================================
CMapObject::~CMapObject()
{
}

//================================================
// 生成処理
//================================================
CMapObject* CMapObject::Create(const D3DXVECTOR3 pos, const char* pModelFileName)
{
	CMapObject* pInstance = new CMapObject;

	// 位置の設定
	pInstance->SetPosition(pos);

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	// 初期化処理
	if (FAILED(pInstance->LoadModel(pModelFileName)))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//================================================
// 初期化処理
//================================================
HRESULT CMapObject::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CMapObject::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//================================================
// 更新処理
//================================================
void CMapObject::Update(void)
{
}

//================================================
// 描画処理
//================================================
void CMapObject::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}
