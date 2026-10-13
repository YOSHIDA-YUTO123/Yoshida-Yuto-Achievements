//===================================================
//
// 回る床のリストを管理するクラス [RotationFloorManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ROTATION_FLOOR_MANAGER_H_
#define _ROTATION_FLOOR_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include <list>
#include <memory>

//***************************************************
// 前方宣言
//***************************************************
class CRotationFloor;	// 回る床
class CCollider;		// コライダー

//***************************************************
// 回る床を管理するマネージャークラスの定義
//***************************************************
class CRotationFloorManager
{
public:
	~CRotationFloorManager();

	/// <summary>
	/// マネージャーの生成処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>マネージャーのインスタンス</returns>
	static CRotationFloorManager *Create(void);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="fMove"></param>
	/// <param name="fRot"></param>
	/// <returns>回転床のインスタンス</returns>
	CRotationFloor* CreateFloor(const D3DXVECTOR3 pos, const float fMove, const float fRot);

	void AddList(CRotationFloor* pRotationFloor);
	void Erase(CRotationFloor* pRotationFloor);
	bool Collision(CCollider* pAABB, D3DXMATRIX* pOutMatrix, int *pFace, bool* pRanding);
	std::list<CRotationFloor*>& GetList(void) { return m_pList; }

	void Uninit(void);
private:
	CRotationFloorManager();

	static std::unique_ptr<CRotationFloorManager> m_pInstance; // 自分のインスタンス
	std::list<CRotationFloor*> m_pList;		   // 回転床のリスト
};

#endif
