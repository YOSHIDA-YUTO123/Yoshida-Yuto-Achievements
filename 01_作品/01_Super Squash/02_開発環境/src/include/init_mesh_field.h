//===================================================
//
// メッシュフィールドの初期化処理 [init_mesh_field.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _INIT_MESH_FIELD_H_
#define _INIT_MESH_FIELD_H_

//***************************************************
// メッシュフィールドの初期化処理
//***************************************************
namespace InitMeshField
{
	HRESULT Field(entt::registry& registry, entt::entity entity);
	HRESULT FieldMT(entt::registry& registry, entt::entity entity);
};
#endif