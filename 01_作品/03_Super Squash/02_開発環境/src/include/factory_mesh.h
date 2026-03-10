//===================================================
//
// メッシュのファクトリーの処理 [factory_mesh.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_MESH_H_
#define _FACTORY_MESH_H_

//***************************************************
// ファクトリー名前空間の定義
//***************************************************
namespace FactoryMesh
{
	// 生成処理
	namespace Create
	{
		static constexpr int BASE_SEGMENT = 32; // 分割数の基準

		entt::entity Field(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath);
		entt::entity FieldMT(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath);
		entt::entity Cylinder(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const char* pTexturePath);
		entt::entity Wall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath, const D3DXVECTOR3& rot);
		entt::entity ShaderWall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nMRTType, const D3DXVECTOR3& rot);
		entt::entity EffectWall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size,const D3DXVECTOR3& rot, const char* pTexturePath, const int nSegmentUV);
		entt::entity WallMT(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pTexturePath, const char* pTexturePath2, const D3DXVECTOR3& rot = Const::VEC3_NULL);
		entt::entity Dome(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const char* pTexturePath, const bool bReverse = false);
		entt::entity Circle(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXCOLOR& col, const char* pTexturePath, const int nSegment = BASE_SEGMENT);
		entt::entity Ball(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const float fRadius, const char* pTexturePath, const int nSegmentUV);
		entt::entity ResultBall(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const float fRadius, const char* pTexturePath, const int nSegmentUV, const int nLife);
		entt::entity Orbit(entt::registry& registry, const entt::entity ownerID, const D3DXVECTOR3& top, const D3DXVECTOR3& bottom, const D3DXCOLOR& col, const char* pTexturePath, const int nSegment = BASE_SEGMENT);
		entt::entity Sphere(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nSegmentUV, const char* pTexturePath, const bool bReverse = false, const D3DXCOLOR& col = Const::WHITE);
		entt::entity Sphere(entt::registry& registry, entt::entity entity, const float fRadius, const float fHeight, const int nSegmentUV, const char* pTexturePath, const bool bReverse = false);
		entt::entity SphereMRT(entt::registry& registry, const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const int nSegmentUV, const char* pTexturePath, const bool bReverse = false, const D3DXCOLOR& col = Const::WHITE);
	}

	// ビルド処理
	namespace Build
	{
		static constexpr float BASE_RESTITUTION = 0.5f;	// 反発係数の基準

		void Field(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV);
		void FieldMT(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV);
		void Cylinder(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV);
		void Wall(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV);
		void WallMT(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV, const D3DXVECTOR2& offPosUVLeft, const D3DXVECTOR2& offPosUVRight);
		void Dome(entt::registry& registry, entt::entity entity, const int nSegmentU, const int nSegmentV, const D3DXCOLOR& col = Const::WHITE);
		void Circle(entt::registry& registry, const entt::entity entity, const float fSpeed, const float fInRadius, const float fOutRadius, const int nLife);
		void Circle(entt::registry& registry, const entt::entity entity, const float fHeight);
		void Ball(entt::registry& registry, const entt::entity entity, const float fRestitution = BASE_RESTITUTION);
	}
}
#endif