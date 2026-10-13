//===================================================
//
// 回る床のリストを管理するクラス [RotationFloorManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "RotationFloorManager.h"
#include "RotationFloor.h"
#include "Collision.h"

//***************************************************
// 静的メンバ変数宣言
//***************************************************
std::unique_ptr<CRotationFloorManager> CRotationFloorManager::m_pInstance = nullptr; // 自分のインスタンス

//===================================================
// コンストラクタ
//===================================================
CRotationFloorManager::CRotationFloorManager() : 
	m_pList()
{
}

//===================================================
// デストラクタ
//===================================================
CRotationFloorManager::~CRotationFloorManager()
{
	// 要素を調べる
	for (auto& list : m_pList)
	{
		list = nullptr;
	}

	m_pList.clear();
}

//===================================================
// インスタンスの取得
//===================================================
CRotationFloorManager* CRotationFloorManager::Create(void)
{
	// 生成されていないなら
	if (m_pInstance == nullptr)
	{
		m_pInstance.reset(new CRotationFloorManager);
	}

	return m_pInstance.get();
}

//===================================================
// 生成処理
//===================================================
CRotationFloor* CRotationFloorManager::CreateFloor(const D3DXVECTOR3 pos, const float fMove, const float fRot)
{
	// 回転床の生成
	CRotationFloor* pInstance = CRotationFloor::Create(pos, fMove, fRot);

	// リストに追加
	AddList(pInstance);

	return pInstance;
}

//===================================================
// リストの追加
//===================================================
void CRotationFloorManager::AddList(CRotationFloor* pRotationFloor)
{
	// リストに追加
	m_pList.push_back(pRotationFloor);
}

//===================================================
// 破棄して並び替え処理
//===================================================
void CRotationFloorManager::Erase(CRotationFloor* pRotationFloor)
{
	// 要素を調べる
	for (auto list = m_pList.begin() ; list != m_pList.end() ; list++)
	{
		// nullなら処理しない
		if ((*list) == nullptr) continue;

		if (pRotationFloor == (*list))
		{
			// 破棄
			(*list)->Uninit();
			list = m_pList.erase(list);

			// 並び替えて最後の列だったらfor分を抜ける
			if (list == m_pList.end()) break;
		}
	}
}

//===================================================
// 当たり判定処理
//===================================================
bool CRotationFloorManager::Collision(CCollider* pAABB, D3DXMATRIX* pOutMatrix, int* pFace, bool* pRanding)
{
	bool bResult = false;

	// 要素を調べる
	for (auto& list : m_pList)
	{
		// nullなら処理しない
		if (list == nullptr) continue;

		// 当たり判定処理
		if (list->Collision(pAABB, pOutMatrix, pFace))
		{
			if (*pFace == CCollisionOBB::FACE_TOP)
			{
				// まだ乗られていない
				if (list->GetIsRanding() == false)
				{
					list->SetIsRanding(true);
					*pRanding = false;
				}
				// 乗られていた
				else
				{
					*pRanding = true;
				}
			}

			bResult = true;
		}
	}

	return bResult;
}

//===================================================
// 終了処理
//===================================================
void CRotationFloorManager::Uninit(void)
{
	// 破棄
	m_pInstance.reset();
}
