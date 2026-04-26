//===================================================
//
// ゲームに関わる処理の補助クラスのマネージャー [world_system_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WORLD_SYSTEM_MANAGER_H_
#define _WORLD_SYSTEM_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CWorldSystem; // ゲームの外のシステムのインターフェース

//***************************************************
// ゲームの外のシステムを担当するクラスのマネージャの定義
//***************************************************
class CWorldSystemManager
{
public:
	CWorldSystemManager();
	~CWorldSystemManager();

	void Register(std::unique_ptr<CWorldSystem> pNewSystem);
	void UnRegister(CWorldSystem* pNewSystem);

	void Clear(void);
	void Update(void);
	void Draw(void);
private:
	std::vector<std::unique_ptr<CWorldSystem>> m_pSystem; // システムのリスト
};
#endif