//===================================================
//
// ボールの更新処理 [update_ball_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BALL_SYSTEM_H_
#define _BALL_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct Transform3DComponent;		// 空間情報のコンポーネント
struct VelocityComponent;			// 移動量コンポーネント
struct MeshVtxComponent;			// メッシュの頂点コンポーネント(軌跡用)
struct BallComponent;				// ボールのコンポーネント
struct MeshWallCollisionComponent;	// メッシュの壁の当たり判定のコンポーネント
struct CharacterComponent;			// キャラクターのコンポーネント

//***************************************************
// ボールの更新処理の定義
//***************************************************
struct UpdateBallSystem : SystemBase
{
	UpdateBallSystem() = default;
	~UpdateBallSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void SetInterVal(entt::registry& registry, entt::entity ball);
	void SetCollisionWall(entt::registry& registry,const entt::entity ball, MeshWallCollisionComponent& collisionComp, BallComponent& ballComp);
	void SetCollisionEffectWall(entt::registry& registry, const entt::entity ball);
	bool CollisionRacket(entt::registry& registry, const entt::entity ball, D3DXVECTOR3& outDir, float& fOutShotPower);
	D3DXVECTOR3 SetShotAngle(entt::registry& registry, const entt::entity character, Transform3DComponent& characterTransform);
	void SetEffect(entt::registry& registry, const entt::entity ball, VelocityComponent& velocityComp, MeshVtxComponent& meshVtxComp, Transform3DComponent& transformComp);
	void SetHitStopEffect(entt::registry& registry, const entt::entity ball, Transform3DComponent& transformComp);
	void UpdateMotionBlurBySpeed(entt::registry& registry, const entt::entity ball, const VelocityComponent& velocityComp);
};
#endif