//===================================================
//
// メッシュのファクトリーの処理 [factory_mesh.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_mesh.h"
#include "transform_component.hpp"
#include "manager.h"
#include "renderer.h"
#include "layer_component.hpp"
#include "mesh_layer.hpp"
#include "texture_id_component.hpp"
#include "size_component.hpp"
#include "texture_manager.h"
#include "mesh_vtx_component.hpp"
#include "sphere_component.hpp"
#include "mesh_cylinder_component.hpp"
#include "vertex_build.h"
#include "vertex_buffer_component.hpp"
#include "tag_component.hpp"
#include "mesh_dome_component.hpp"
#include "utility_math.h"
#include "ball_component.hpp"
#include "renderer_component.hpp"
#include "velocity_component.hpp"
#include "field_collision_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "capsule_collider_component.hpp"
#include "mesh_wave_component.hpp"
#include "ball_field_collision_component.hpp"
#include "mesh_sphere_component.hpp"
#include "mesh_circle_component.hpp"
#include "mesh_orbit_component.hpp"
#include "init_mesh.h"
#include "move_to_rotation_component.hpp"
#include "factory_model.h"
#include "factory_mesh_quad.h"
#include "shadow_component.hpp"
#include "texture_mt_component.hpp"
#include "outline_shader_component.hpp"
#include "color_constants.hpp"
#include "texture_mrt_manager.h"
#include "collision_effect_wall_component.hpp"
#include "ui_wall_component.hpp"
#include "result_ball_component.hpp"
#include "sphere_collider_component.hpp"
#include "init_mesh_field.h"
#include "factory_system_entity.h"
#include "motion_blur_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace FactoryMeshConst
{
	constexpr float SHADOW_MAX_HEIGHT = 500.0f;	// 影を表示する最大の高さ
	constexpr float BALL_INERTIA = 0.0f;		// ボールの慣性
	constexpr float BALL_GRAVITY = 0.15f;		// ボールの重力
	constexpr float BALL_OUTLINE_SIZE = 0.3f;	// ボールのアウトラインの大きさ
	constexpr int NUM_BLUR = 5;				// ブラーの総数

}

//===================================================
// メッシュフィールドの生成
//===================================================
entt::entity FactoryMesh::Create::Field(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size, const char* pTexturePath)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<RendererTag::ShadowMapRecieveField>(entity);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Tag::MeshFieldTag>(entity);

	return entity;
}

//===================================================
// マルチテクスチャのメッシュフィールドの生成
//===================================================
entt::entity FactoryMesh::Create::FieldMT(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size, const char* pTexturePath)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<RendererTag::MeshMTTag>(entity);
	registry.emplace<LayerComponent>(entity, MeshLayer::LAYER_FIELD);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Tag::MeshFieldTag>(entity);

	return entity;
}

//===================================================
// メッシュフィールドの生成処理
//===================================================
void FactoryMesh::Build::Field(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	registry.emplace<MeshVtxComponent>(entity, nSegmentU, nSegmentV);

	// メッシュフィールドの初期化処理
	if (FAILED(InitMeshField::Field(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// マルチテクスチャのメッシュフィールドの構築処理
//===================================================
void FactoryMesh::Build::FieldMT(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	registry.emplace<MeshVtxComponent>(entity, nSegmentU, nSegmentV);

	// メッシュフィールドの初期化処理
	if (FAILED(InitMeshField::FieldMT(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// シリンダーの生成処理
//===================================================
entt::entity FactoryMesh::Create::Cylinder(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const char* pTexturePath)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshTag>(entity);
	registry.emplace<LayerComponent>(entity, MeshLayer::LAYER_CIRYNDER);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<SphereComponent>(entity, fRadius);
	registry.emplace<MeshCylinderComponent>(entity, fHeight);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	

	return entity;
}

//===================================================
// メッシュシリンダーの生成処理
//===================================================
void FactoryMesh::Build::Cylinder(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	registry.emplace<MeshVtxComponent>(entity, nSegmentU, nSegmentV);

	// メッシュシリンダーの初期化処理
	if (FAILED(InitMesh::Cylinder(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// メッシュウォールの生成処理
//===================================================
entt::entity FactoryMesh::Create::Wall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size, const char* pTexturePath, const D3DXVECTOR3& rot)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshTag>(entity);
	registry.emplace<Transform3DComponent>(entity, pos, rot);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_NONE);

	return entity;
}

//===================================================
// 演出用の壁の生成
//===================================================
entt::entity FactoryMesh::Create::EffectWall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size, const D3DXVECTOR3& rot, const char* pTexturePath, const int nSegmentUV)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshTag>(entity);
	registry.emplace<Tag::EffectMeshWallTag>(entity);
	registry.emplace<LayerComponent>(entity, MeshLayer::LAYER_WALL);
	registry.emplace<Transform3DComponent>(entity, pos, rot);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_ALPHA_BLEND | RendererComponent::TYPE_LIGHT_OFF);
	registry.emplace<MeshVtxComponent>(entity, nSegmentUV, nSegmentUV, D3DXCOLOR(Color::AQUA.r, Color::AQUA.g, Color::AQUA.b,0.005f));
	
	// メッシュウォールの初期化処理
	if (FAILED(InitMesh::Wall(registry, entity)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// マルチテクスチャの壁の生成
//===================================================
entt::entity FactoryMesh::Create::WallMT(
	entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& Size,
	const char* pTexturePath, const char* pTexturePath2, const D3DXVECTOR3& rot)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;
	int nTextureIDMT = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
		nTextureIDMT = pTextureManager->Register(pTexturePath2);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshWallTag>(entity);
	registry.emplace<Transform3DComponent>(entity, pos, rot);
	registry.emplace<Size3DComponent>(entity, Size);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_NONE | RendererComponent::TYPE_LIGHT_OFF);
	auto& uiWallComp = registry.emplace<UIWallComponent>(entity, D3DXVECTOR2(0.001f, 0.0f));

	uiWallComp.pTexture = pTextureManager->GetAdress(nTextureID);
	uiWallComp.pTextureMT = pTextureManager->GetAdress(nTextureIDMT);

	return entity;
}

//===================================================
// メッシュウォールの生成処理
//===================================================
void FactoryMesh::Build::Wall(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV)
{
	registry.emplace<MeshVtxComponent>(entity, nSegmentU, nSegmentV);

	// メッシュウォールの初期化処理
	if (FAILED(InitMesh::Wall(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// マルチテクスチャの壁
//===================================================
void FactoryMesh::Build::WallMT(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV, const D3DXVECTOR2& OffPosUVLeft, const D3DXVECTOR2& OffPosUVRight)
{
	registry.emplace<MeshVtxComponent>(entity, nSegmentU, nSegmentV);

	// メッシュウォールの初期化処理
	if (FAILED(InitMesh::WallMT(registry, entity, OffPosUVLeft, OffPosUVRight)))
	{
		assert(false);
	}
}

//===================================================
// メッシュドームの生成処理
//===================================================
entt::entity FactoryMesh::Create::Dome(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const char* pTexturePath, const bool bReverse)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshDomeTag>(entity);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<SphereComponent>(entity, fRadius);
	registry.emplace<VertexBufferComponent>(entity);
	auto& dome = registry.emplace<MeshDomeComponent>(entity, bReverse);

	dome.vtxEntity[dome.TYPE_TOP] = registry.create();;
	dome.vtxEntity[dome.TYPE_SIDE] = registry.create();;

	registry.emplace<MeshVtxComponent>(dome.vtxEntity[dome.TYPE_TOP]);
	registry.emplace<MeshVtxComponent>(dome.vtxEntity[dome.TYPE_SIDE]);

	return entity;
}

//===================================================
// メッシュドームのビルド処理
//===================================================
void FactoryMesh::Build::Dome(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV, const D3DXCOLOR& col)
{
	// ドームのコンポーネントを取得
	auto& dome = registry.get<MeshDomeComponent>(entity);

	auto& FanVtx = registry.get<MeshVtxComponent>(dome.vtxEntity[dome.TYPE_TOP]);
	FanVtx.nSegmentU = nSegmentU;
	FanVtx.nSegmentV = 1;
	FanVtx.col = col;

	auto& SideVtx = registry.get<MeshVtxComponent>(dome.vtxEntity[dome.TYPE_SIDE]);
	SideVtx.nSegmentU = nSegmentU;
	SideVtx.nSegmentV = nSegmentV;
	SideVtx.col = col;

	// メッシュウォールの初期化処理
	if (FAILED(InitMesh::Dome(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// サークルの生成処理
//===================================================
entt::entity FactoryMesh::Create::Circle(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXCOLOR& col, const char* pTexturePath, const int nSegment)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshTag>(entity);
	registry.emplace<LayerComponent>(entity, MeshLayer::LAYER_CIRCLE);
	registry.emplace<Transform3DComponent>(entity, pos, rot);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<MeshVtxComponent>(entity, nSegment, 1, col);

	return entity;
}

//===================================================
// メッシュのサークルの構築処理
//===================================================
void FactoryMesh::Build::Circle(entt::registry& registry, const entt::entity entity, const float fSpeed, const float fInRadius, const float fOutRadius, const int nLife)
{
	auto& MeshVtx = registry.get<MeshVtxComponent>(entity);

	registry.emplace<MeshCircleComponent>(entity, MeshVtx.col.a, fSpeed, fInRadius, fOutRadius, nLife);
	registry.emplace<RendererComponent>(entity,
		RendererComponent::TYPE_ALPHA_BLEND |
		RendererComponent::TYPE_LIGHT_OFF |
		RendererComponent::TYPE_NO_CULL | 
		RendererComponent::TYPE_ZTEST);
}

//===================================================
// メッシュサークルの構築処理
//===================================================
void FactoryMesh::Build::Circle(entt::registry& registry, const entt::entity entity, const float fHeight)
{
	auto& meshCircleComp = registry.get<MeshCircleComponent>(entity);
	meshCircleComp.fHeight = fHeight;

	// サークルの初期化処理
	if (FAILED(InitMesh::Circle(registry, entity)))
	{
		assert(false);
	}
}

//===================================================
// ボールの構築処理
//===================================================
void FactoryMesh::Build::Ball(entt::registry& registry, const entt::entity entity, const float fRestitution)
{
	// コンポーネントの取得
	auto& ballComp = registry.get<BallComponent>(entity);

	// 反発係数の設定
	ballComp.fRestitution = fRestitution;
}

//===================================================
// メッシュボールの生成処理
//===================================================
entt::entity FactoryMesh::Create::Ball(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const float fRadius, const char* pTexturePath, const int nSegmentUV)
{
	// entityの生成
	const entt::entity entity = registry.create();

	// ボールの設定
	auto& ballComp = registry.emplace<BallComponent>(entity);
	
	// 一周の大きさを求める
	float fCircumference = D3DX_PI * 2.0f * fRadius;

	registry.emplace<MoveToRotationComponent>		(entity, fCircumference);
	registry.emplace<Transform3DComponent>			(entity, pos);
	registry.emplace<VelocityComponent>				(entity, FactoryMeshConst::BALL_INERTIA, FactoryMeshConst::BALL_GRAVITY, move);
	registry.emplace<BallFieldCollisionComponent>	(entity, entity);
	registry.emplace<MeshWallCollisionComponent>	(entity, entity);
	registry.emplace<SphereComponent>				(entity, fRadius);
	registry.emplace<Tag::BallTag>					(entity);
	registry.emplace<Tag::HitStopTag>				(entity);
	registry.emplace<RendererTag::OutLineSphereTag>	(entity);
	registry.emplace<RendererTag::ShadowMapSphere>	(entity);
	registry.emplace<OutLineShaderComponent>		(entity,Color::AQUA, FactoryMeshConst::BALL_OUTLINE_SIZE);
	registry.emplace<CollisionEffectWallComponent>	(entity);
	registry.emplace<MotionBlurComponent>			(entity, FactoryMeshConst::NUM_BLUR);

	// 球のコライダーの生成
	auto sphereID = FactorySystemEntity::CreateSphereCollider(
		registry,
		entity,
		fRadius,
		Const::VEC3_NULL);

	registry.emplace<ColliderTag::Wall>			(sphereID);
	registry.emplace<ColliderTag::BallSphere>	(sphereID);

	// 球体の生成
	FactoryMesh::Create::Sphere(
		registry,
		entity,
		fRadius,
		fRadius,
		nSegmentUV,
		pTexturePath);

	// 軌跡の生成
	ballComp.orbitID = FactoryMesh::Create::Orbit(
		registry, 
		entity, 
		D3DXVECTOR3(fRadius * 0.5f, 0.0f, 0.0f), 
		D3DXVECTOR3(-fRadius * 0.5f, 0.0f, 0.0f), 
		Const::WHITE,
		"");

	return entity;
}

//===================================================
// リザルトのボールの生成処理
//===================================================
entt::entity FactoryMesh::Create::ResultBall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const float fRadius, const char* pTexturePath, const int nSegmentUV, const int nLife)
{
	// entityの生成
	const entt::entity entity = registry.create();

	// ボールの設定
	registry.emplace<BallComponent>					(entity);

	registry.emplace<Transform3DComponent>			(entity, pos);
	registry.emplace<VelocityComponent>				(entity, FactoryMeshConst::BALL_INERTIA, FactoryMeshConst::BALL_GRAVITY, move);
	registry.emplace<MeshWallCollisionComponent>	(entity, entity);
	registry.emplace<SphereComponent>				(entity, fRadius);
	registry.emplace<SphereColliderComponent>		(entity, fRadius, entity);
	registry.emplace<Tag::BallTag>					(entity);
	registry.emplace<ColliderTag::BallSphere>		(entity);
	registry.emplace<ColliderTag::Wall>				(entity);

	registry.emplace<OutLineShaderComponent>		(entity, Color::AQUA, FactoryMeshConst::BALL_OUTLINE_SIZE);
	registry.emplace<RendererTag::OutLineSphereTag>	(entity);
	registry.emplace<CollisionEffectWallComponent>	(entity);
	registry.emplace<ResultBallComponent>			(entity, nLife);

	// 球体の生成
	FactoryMesh::Create::Sphere(
		registry, 
		entity, 
		fRadius,
		fRadius,
		nSegmentUV, 
		pTexturePath);

	return entity;
}

//===================================================
// 軌跡の生成処理
//===================================================
entt::entity FactoryMesh::Create::Orbit(entt::registry& registry, const entt::entity ownerID, const D3DXVECTOR3& top, const D3DXVECTOR3& bottom, const D3DXCOLOR& col, const char* pTexturePath, const int nSegment)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshTag>(entity);
	registry.emplace<LayerComponent>(entity, MeshLayer::LAYER_ORBIT);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform3DComponent>(entity);
	registry.emplace<MeshVtxComponent>(entity, nSegment, 1, col);
	registry.emplace<MeshOrbitComponent>(entity, ownerID, top, bottom);
	registry.emplace<RendererComponent>(
		entity,
		RendererComponent::TYPE_NO_CULL |
		RendererComponent::TYPE_LIGHT_OFF | 
		RendererComponent::TYPE_ALPHA_BLEND | 
		RendererComponent::TYPE_ZTEST);
	

	// 初期化処理
	if (FAILED(InitMesh::Orbit(registry, entity)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// 球体の生成
//===================================================
entt::entity FactoryMesh::Create::Sphere(
	entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight,
	const int nSegmentUV, const char* pTexturePath, const bool bReverse, const D3DXCOLOR& col)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MeshSphereTag>(entity);
	registry.emplace<RendererTag::ShadowMapSphere>(entity);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<MeshVtxComponent>(entity, nSegmentUV, nSegmentUV, col);
	registry.emplace<MeshSphereComponent>(entity, fRadius, fHeight, bReverse);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_NONE);

	// 球体の初期化処理
	if (FAILED(InitMesh::Sphere(registry, entity)))
	{
		// 破棄
		registry.destroy(entity);
		assert(false);
	}
	return entity;
}

//===================================================
// エンティティ番号指定の球体の生成
//===================================================
entt::entity FactoryMesh::Create::Sphere(entt::registry& registry, entt::entity entity, const float fRadius, const float fHeight, const int nSegmentUV, const char* pTexturePath, const bool bReverse)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	registry.emplace<Tag::MeshSphereTag>(entity);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<MeshVtxComponent>(entity, nSegmentUV, nSegmentUV);
	registry.emplace<MeshSphereComponent>(entity, fRadius, fHeight, bReverse);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_LIGHT_OFF);

	// 球体の初期化処理
	if (FAILED(InitMesh::Sphere(registry, entity)))
	{
		// 破棄
		registry.destroy(entity);
		assert(false);
	}
	return entity;
}

//===================================================
// MRTのメッシュの球体の描画処理
//===================================================
entt::entity FactoryMesh::Create::SphereMRT(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nSegmentUV, const char* pTexturePath, const bool bReverse, const D3DXCOLOR& col)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = CTextureManager::INVALID_ID;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(pTexturePath);
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<RendererTag::MeshSphereMRT>(entity);
	registry.emplace<LayerComponent>(entity);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform3DComponent>(entity, pos);
	registry.emplace<MeshVtxComponent>(entity, nSegmentUV, nSegmentUV, col);
	registry.emplace<MeshSphereComponent>(entity, fRadius, fHeight, bReverse);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_NONE);

	// 球体の初期化処理
	if (FAILED(InitMesh::Sphere(registry, entity)))
	{
		// 破棄
		registry.destroy(entity);
		assert(false);
	}
	return entity;
}
