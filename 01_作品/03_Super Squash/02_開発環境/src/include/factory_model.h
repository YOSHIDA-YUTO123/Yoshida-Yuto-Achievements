//===================================================
//
// モデルのエンティティのファクトリーの処理 [factory_model.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_MODEL_H_
#define _FACTORY_MODEL_H_

//***************************************************
// モデルファクトリー名前空間の定義
//***************************************************
namespace FactoryModel
{
	const int Register(const char* pModelFilePath);
	entt::entity CreateModel(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath);
	entt::entity CreateOutLineModel(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath);
	entt::entity CreateModelMRT(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath, const int nMRTType);
	entt::entity CreateStencilShadow(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& scal);
	entt::entity CreateJetPack(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pModelFilePath, const entt::entity ownerID, const entt::entity parentID);
}
#endif