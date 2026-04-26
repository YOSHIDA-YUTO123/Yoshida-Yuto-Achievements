//===================================================
//
// ゲーム等の処理に関わるentityの生成 [factory_system_entity.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_SYSTEM_ENTITY_H_
#define _FACTORY_SYSTEM_ENTITY_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// ゲーム等の処理に関わるentityの生成名前空間の定義
//***************************************************
namespace FactorySystemEntity
{
	entt::entity CreateScoreArea(entt::registry& registry, const D3DXVECTOR3& centerPos);
	entt::entity CreateLocator(entt::registry& registry, const D3DXVECTOR3& offSetPos, const entt::entity parentID);
	entt::entity CreateMenuSelect(entt::registry& registry, const int nMaxMenu, const int nNextKey, const int nNextJoykey, const bool bWrap);
	entt::entity CreateTutorialMRT(entt::registry& registry);
	entt::entity CreateResultMRT(entt::registry& registry);
	entt::entity CreateTutorial(entt::registry& registry);
	entt::entity CreateTitleMRT(entt::registry& registry);
	entt::entity CreateSphereCollider(entt::registry& registry, const entt::entity parent, const float fRadius, const D3DXVECTOR3& pos);
	entt::entity CreateCapsuleCollider(entt::registry& registry, const entt::entity parent, const float fRadius, const D3DXVECTOR3& pos, const D3DXVECTOR3& start, const D3DXVECTOR3& end);

	void BuildMenuSelect(entt::registry& registry, const entt::entity entity,const int nPrevKey,const int nPrevJoyKey);
}
#endif