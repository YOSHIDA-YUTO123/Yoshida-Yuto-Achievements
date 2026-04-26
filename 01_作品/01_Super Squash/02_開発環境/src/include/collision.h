//===================================================
//
// 当たり判定の処理 [collision.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
struct CapsuleColliderComponent;	// カプセルコライダーデータ
struct SphereColliderComponent;		// 球コライダーデータ
struct BoxColliderComponent;		// AABBのコライダー
struct Transform3DComponent;		// 空間情報

//***************************************************
// 当たり判定の名前空間の処理
//***************************************************
namespace Collision
{
	// 省略する
	using Vec3 = D3DXVECTOR3;

	// 型名を省略
	using BoxComp = BoxColliderComponent;

	// 型名を省略
	using SphereComp = SphereColliderComponent;

	// 型名を省略
	using CapsuleComp = CapsuleColliderComponent;

	// 箱
	namespace Box
	{
		bool CollisionSphere(const entt::registry& registry, const BoxComp& box, const SphereComp& sphere);
	}

	// 球
	namespace Sphere
	{
		bool Collision(const entt::registry& registry, const SphereComp& self, const SphereComp& target);
	}

	// カプセル
	namespace Capsule
	{
		// 線分と線分との最接近点のデータ
		struct ClosestData
		{
			Vec3 start1;		// 線分の始点1
			Vec3 start2;		// 線分の始点2
			Vec3 end1;			// 線分の終点1
			Vec3 end2;			// 線分の終点2
			Vec3 closestPos1;	// 最接近点1
			Vec3 closestPos2;	// 最接近点2
			float fParam1;		// パラメータ1
			float fParam2;		// パラメータ2
		};

		bool Collision(const entt::registry& registry, const CapsuleComp& self, const CapsuleComp& target, Vec3* pClosePos1 = nullptr, Vec3* pClosePos2 = nullptr);
		bool CollisionSphere(const entt::registry& registry, const CapsuleComp& capsuleComp, const SphereComp& sphere, Vec3* pClosePos = nullptr);
		float ClosestPtSegmentSegment(ClosestData& closestData);
	}

	// 視界
	namespace FOV
	{
		bool Collision(const Vec3& targetPos, const Transform3DComponent& myTransform, const float fAngleLeft, const float fAngleRight, const float fLength);
	}
};

#endif