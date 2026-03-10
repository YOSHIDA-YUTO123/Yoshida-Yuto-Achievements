//===================================================
//
// 当たり判定の処理 [collision.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "collision.h"
#include "utility_math.h"
#include "capsule_collider_component.hpp"
#include "sphere_collider_component.hpp"
#include "transform_component.hpp"
#include "debug_proc.h"
#include "factory_billboard.h"
#include "line.h"
#include "color_constants.hpp"
#include "box_collider_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace Collision_Constants
{
	constexpr float HALF_VALUE = 0.5f; // 半分
}

//===================================================
// カプセルとカプセルの当たり判定
//===================================================
bool Collision::Capsule::Collision(const entt::registry& registry, const CapsuleComp& self, const CapsuleComp& target, Vec3* pClosePos1, Vec3* pClosePos2)
{
	(void)registry;

	ClosestData closestData; // 最接近点の計算用データ

	// 自分の始点と終点の設定
	closestData.start1 = self.startPos;
	closestData.end1 = self.endPos;

	// 相手の始点と終点の設定
	closestData.start2 = target.startPos;
	closestData.end2 = target.endPos;

	// 線分と円の距離を求める
	float fDistance = ClosestPtSegmentSegment(closestData);

	if (pClosePos1 != nullptr)
	{
		*pClosePos1 = closestData.closestPos1;
	}
	if (pClosePos2 != nullptr)
	{
		*pClosePos2 = closestData.closestPos2;
	}
	// 距離を求める
	fDistance = sqrtf(fDistance);

	// 二つの円の半径を渡す
	float fRadius = self.fRadius + target.fRadius;

	// 距離が半径以下だったら当たっている
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//===================================================
// カプセルと球の当たり判定
//===================================================
bool Collision::Capsule::CollisionSphere(const entt::registry& registry, const CapsuleComp& capsuleComp, const SphereComp& sphere, Vec3* pClosePos)
{
	// 球の位置の取得
	auto& sphereTransform = registry.get<Transform3DComponent>(sphere.ownerID);

	// カプセルコライダーの線分を求める
	D3DXVECTOR3 vecLine = capsuleComp.endPos - capsuleComp.startPos;
	D3DXVECTOR3 vecSphere = sphereTransform.pos - capsuleComp.startPos;

	// 線分の上をどれだけ進んでいるか求める
	float fDot = math::Dot(vecSphere, vecLine);

	// 線分の長さを求める
	float fLength = math::Dot(vecLine, vecLine);

	// 点が線分上のどこにいるか割合を求める
	float t = fDot / fLength;

	// 線分の上にとどめる
	t = math::Clamp(t, 0.0f, 1.0f);

	// 最接近点を求める
	D3DXVECTOR3 closestPos = capsuleComp.startPos + vecLine * t;

	// 最接近点から球の中心までの距離を求める
	float fDistanceSq = math::Dot(sphereTransform.pos - closestPos, sphereTransform.pos - closestPos);

	// 半径を足し合わせる
	float fRadius = capsuleComp.fRadius + sphere.fRadius;

	// 半径を2乗する
	float fRadiussq = fRadius * fRadius;

	// 最接近点を設定
	if (pClosePos != nullptr)
	{
		*pClosePos = closestPos;
	}

	// 距離が半径以下だったら
	if (fDistanceSq <= fRadiussq)
	{
		return true;
	}
	return false;
}

//===================================================
// 線分と線分の最接近点の取得
//===================================================
float Collision::Capsule::ClosestPtSegmentSegment(ClosestData& closestData)
{
	D3DXVECTOR3 dir1 = closestData.start1 - closestData.end1;			// 線分S1のベクトル
	D3DXVECTOR3 dir2 = closestData.start2 - closestData.end2;			// 線分S2のベクトル
	D3DXVECTOR3 diffStart = closestData.start1 - closestData.start2;

	float fLengthSq1 = D3DXVec3Dot(&dir1, &dir1); // 線分S1の方向ベクトル
	float fLengthSq2 = D3DXVec3Dot(&dir2, &dir2); // 線分S2の方向ベクトル

	float fDotVec2DiffStart = D3DXVec3Dot(&dir2, &diffStart);

	const float EPSILON = 1e-6f; // チェック用

	// 片方あるいは両方の線分が点に対して縮退しているかどうかチェック
	if (fLengthSq1 <= EPSILON && fLengthSq2 <= EPSILON)
	{
		// 両方の線分が点に縮退
		closestData.fParam1 = closestData.fParam2 = 0.0f;
		closestData.closestPos1 = closestData.start1;
		closestData.closestPos2 = closestData.start2;

		// 最接近点との差分を求める
		D3DXVECTOR3 diff = closestData.closestPos1 - closestData.closestPos2;

		// 距離を返す
		return D3DXVec3Dot(&diff, &diff);
	}

	if (fLengthSq1 <= EPSILON)
	{
		// 最初の線分が点に縮退
		closestData.fParam1 = 0.0f;
		closestData.fParam2 = fDotVec2DiffStart / fLengthSq2; // s = o >= t = (b*s + f) / e = f / e
		closestData.fParam2 = math::Clamp(closestData.fParam2, 0.0f, 1.0f);
	}
	else
	{
		float fDotDir1Diff = D3DXVec3Dot(&dir1, &diffStart);

		if (fLengthSq2 <= EPSILON)
		{
			// 2番目の線分が点に縮退
			closestData.fParam2 = 0.0f;
			closestData.fParam1 = math::Clamp(-fDotDir1Diff / fLengthSq1, 0.0f, 1.0f); // t = 0 >= s = (b * t - c) / a = -c / a
		}
		else
		{
			float fDotDir1Dir2 = D3DXVec3Dot(&dir1, &dir2);
			float fDenom = fLengthSq1 * fLengthSq2 - fDotDir1Dir2 * fDotDir1Dir2; // 常に非負

			// 線分が平行ではない場合、L1上のL2に対する最近接点を計算、そして線分1に対してクランプ。そうでない場合は任意のsを選択
			if (fDenom != 0.0f)
			{
				closestData.fParam1 = math::Clamp((fDotDir1Dir2 * fDotVec2DiffStart - fDotDir1Diff * fLengthSq2) / fDenom, 0.0f, 1.0f);
			}
			else
			{
				closestData.fParam1 = 0.0f;
			}

			// L2上のS1(s)に対する最近接点を以下を用いて計算
			// t = dot((P1 + D1 * s) - P2,P2) / dot(D2,D2) = (b * s + f) / e
			closestData.fParam2 = (fDotDir1Dir2 * (closestData.fParam1) + fDotVec2DiffStart) / fLengthSq2;

			// tが[0,1]の中にあれば終了。そうなればｔをクランプ、ｓをｔの新しい値に対して以下を用いて計算
			// s = Dot((P2 + D2 * t) - P1,D1) / Dot(D1,D1) = (t * b - c) / a
			// そしてsを[0,1]に対してクランプ
			if (closestData.fParam2 < 0.0f)
			{
				closestData.fParam2 = 0.0f;
				closestData.fParam1 = math::Clamp(-fDotDir1Diff / fLengthSq1, 0.0f, 1.0f);
			}
			else if (closestData.fParam2 > 1.0f)
			{
				closestData.fParam2 = 1.0f;
				closestData.fParam1 = math::Clamp((fDotDir1Dir2 - fDotDir1Diff) / fLengthSq1, 0.0f, 1.0f);
			}
		}
	}

	// 最接近点を求める
	closestData.closestPos1 = closestData.start1 + dir1 * closestData.fParam1;
	closestData.closestPos2 = closestData.start2 + dir2 * closestData.fParam2;

	// 差分を求める
	D3DXVECTOR3 diff = closestData.closestPos1 - closestData.closestPos2;

	// 距離を返す
	return D3DXVec3Dot(&diff, &diff);;
}

//===================================================
// 視界の判定(コライダー無し)
//===================================================
bool Collision::FOV::Collision(const D3DXVECTOR3& targetPos, const Transform3DComponent& myTransform, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 現在の向いている方向を取得
	D3DXVECTOR3 forward = math::GetForward(myTransform.quaternion);
	D3DXVec3Normalize(&forward, &forward);

	// yの回転(forwardを求める)を求める
	float fForwardAngle = atan2f(forward.x, forward.z);

	D3DXVECTOR3 fovLeft, fovRight;

	// 左側の視野角の端を求める
	fovLeft.x = myTransform.pos.x + sinf(fForwardAngle - fAngleLeft) * fLength;
	fovLeft.y = myTransform.pos.y;
	fovLeft.z = myTransform.pos.z + cosf(fForwardAngle - fAngleLeft) * fLength;

	// 右側の視野角の端を求める
	fovRight.x = myTransform.pos.x - sinf(fForwardAngle + fAngleRight) * fLength;
	fovRight.y = myTransform.pos.y;
	fovRight.z = myTransform.pos.z - cosf(fForwardAngle + fAngleRight) * fLength;

	// ターゲットまでのベクトルを求める
	D3DXVECTOR3 dirTarget = math::GetVector(targetPos, myTransform.pos);

	// 視野角までのベクトルを求める
	D3DXVECTOR3 dirFovLeft = math::GetVector(fovLeft, myTransform.pos);
	D3DXVECTOR3 dirFovRight = math::GetVector(fovRight, myTransform.pos);

	// 左側の視界との外積
	D3DXVECTOR3 crossLeft = math::Cross(dirFovLeft, dirTarget);

	// 右側の視界との外積
	D3DXVECTOR3 crossRight = math::Cross(dirTarget, dirFovRight);

	// 二つとも上向きなら視界の中
	if (crossRight.y >= 0.0f && crossRight.y >= 0.0f)
	{
#ifdef _DEBUG
		auto line = CLineRenderer::GetInstance();

		if (line != nullptr)
		{
			line->AddLine(myTransform.pos, fovLeft, Color::MAROON);
			line->AddLine(myTransform.pos, fovRight, Color::MAROON);
		}
#endif // _DEBUG

		return true;
	}

#ifdef _DEBUG
	auto line = CLineRenderer::GetInstance();

	if (line != nullptr)
	{
		line->AddLine(myTransform.pos, fovLeft, Color::GREENYELLOW);
		line->AddLine(myTransform.pos, fovRight, Color::GREENYELLOW);
	}
#endif // _DEBUG

	return false;
}

//===================================================
// 球と球の判定 ***
//===================================================
bool Collision::Sphere::Collision(const entt::registry& registry, const SphereComp& self, const SphereComp& target)
{
	// 空間情報の取得
	auto& myTransformComp = registry.get<Transform3DComponent>(self.ownerID);
	auto& OthertransformComp = registry.get<Transform3DComponent>(target.ownerID);

	// 差分を求める
	D3DXVECTOR3 diff = myTransformComp.pos - OthertransformComp.pos;

	// 距離を求める
	float fDistance = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	// 半径を足す
	float fRadius = self.fRadius + target.fRadius;
	fRadius *= fRadius;

	if (fDistance <= fRadius)
	{
		return true;
	}
	return false;
}

//===================================================
// 矩形と球の判定
//===================================================
bool Collision::Box::CollisionSphere(const entt::registry& registry, const BoxComp& box, const SphereComp& sphere)
{
	auto& sphereTransform = registry.get<Transform3DComponent>(sphere.ownerID);
	auto& aabbTransform = registry.get<Transform3DComponent>(box.ownerID);

	D3DXVECTOR3 sphereLocal = sphereTransform.pos - aabbTransform.pos;

	// 球から見て最も近い点を求める
	D3DXVECTOR3 closest = Const::VEC3_NULL;

	// 矩形の範囲内にクランプ
	closest.x = math::Clamp(sphereLocal.x, -box.size.x * 0.5f, box.size.x * 0.5f);
	closest.y = math::Clamp(sphereLocal.y, -box.size.y * 0.5f, box.size.y * 0.5f);
	closest.z = math::Clamp(sphereLocal.z, -box.size.z * 0.5f, box.size.z * 0.5f);

	// 最接近点と球の中心の距離を求める
	D3DXVECTOR3 distance = sphereLocal - closest;

#ifdef _DEBUG

	// 線のインスタンスの取得
	auto line = CLineRenderer::GetInstance();

	if (line != nullptr)
	{
		line->AddLine(sphereTransform.pos, aabbTransform.pos + closest, Color::CHOCOLATE);
	}

#endif // _DEBUG
	// 距離が半径以下だったら
	const bool bHit = D3DXVec3LengthSq(&distance) <= (sphere.fRadius * sphere.fRadius);

	return bHit;
}
