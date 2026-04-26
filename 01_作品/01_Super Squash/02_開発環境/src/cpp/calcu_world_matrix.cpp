//===================================================
//
// ワールドマトリックスの計算処理 [calcu_world_matrix.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "calcu_world_matrix.h"
#include "transform_component.hpp"
#include "tag_component.hpp"
#include "renderer.h"
#include "manager.h"
#include "parent_component.hpp"
#include "entity_manager.h"

//===================================================
// 更新処理
//===================================================
void CalcuWorldMatrix::Update(entt::registry& registry)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "CalcuWorldMatrix_GetRenderer");
#endif // _DEBUG	

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// 指定したコンポーネントをもつentityの取得
	auto view = registry.view<Transform3DComponent>();

	// 取得したエンティティ分回す
	for (auto entity : view)
	{
		// コンポーネントの取得
		auto& transformComp = registry.get<Transform3DComponent>(entity);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&transformComp.mtxWorld);

		// ビルボードなら
		if (registry.all_of<Tag::BillboardTag>(entity))
		{
			// ビューマトリックスを宣言
			D3DXMATRIX mtxView;

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// カメラの逆行列を設定
			transformComp.mtxWorld._11 = mtxView._11;
			transformComp.mtxWorld._12 = mtxView._21;
			transformComp.mtxWorld._13 = mtxView._31;
			transformComp.mtxWorld._21 = mtxView._12;
			transformComp.mtxWorld._22 = mtxView._22;
			transformComp.mtxWorld._23 = mtxView._32;
			transformComp.mtxWorld._31 = mtxView._13;
			transformComp.mtxWorld._32 = mtxView._23;
			transformComp.mtxWorld._33 = mtxView._33;
		}

		// クォータニオンから回転行列の作成、反映
		D3DXMatrixRotationQuaternion(&mtxRot, &transformComp.quaternion);
		D3DXMatrixMultiply(&transformComp.mtxWorld, &transformComp.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, transformComp.pos.x, transformComp.pos.y, transformComp.pos.z);
		D3DXMatrixMultiply(&transformComp.mtxWorld, &transformComp.mtxWorld, &mtxTrans);
	}

	auto parentView = registry.view<ParentComponent>();

	// vector にコピー
	std::vector<entt::entity> entities(parentView.begin(), parentView.end());

	// 生成順にソート
	std::sort(entities.begin(), entities.end(), [&registry](entt::entity self, entt::entity other)
		{
			return registry.get<ParentComponent>(self).nCreateIndex < registry.get<ParentComponent>(other).nCreateIndex;
		});

	// エンティティ分回す
	for (auto entity : entities)
	{
		// 親子関係のコンポーネント
		auto& transformComp = registry.get<Transform3DComponent>(entity);
		auto& parentComp = registry.get<ParentComponent>(entity);

		// 親が存在するなら
		if (registry.valid(parentComp.parentID))
		{
			// 親の空間情報を取得
			auto& parentTransform = registry.get<Transform3DComponent>(parentComp.parentID);

			// 親のマトリックスと掛け合わせる
			D3DXMatrixMultiply(&transformComp.mtxWorld, &transformComp.mtxWorld, &parentTransform.mtxWorld);
		}
	}
}