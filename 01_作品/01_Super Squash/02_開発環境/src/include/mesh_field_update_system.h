//===================================================
//
// メッシュフィールドの更新処理 [mesh_field_update_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESHFIELD_UPDATE_SYSTEM_H_
#define _MESHFIELD_UPDATE_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// メッシュフィールドの更新処理の定義
//***************************************************
struct MeshFieldUpdateSystem : SystemBase
{
	MeshFieldUpdateSystem() = default;
	~MeshFieldUpdateSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void ResetNormal(entt::entity entity, entt::registry& registry);
};
#endif