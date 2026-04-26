//===================================================
//
// ミニゲームのプレイヤーの更新処理 [minigame_player_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MINI_GAME_PLAYER_SYSTEM_H_
#define _MINI_GAME_PLAYER_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct CapsuleColliderComponent;	// カプセルコライダーコンポーネント
struct MotionComponent;				// モーションコンポーネント
struct Transform3DComponent;		// 空間情報のコンポーネント
struct CharacterComponent;			// キャラクターコンポーネント
struct VelocityComponent;			// 移動量コンポーネント

//***************************************************
// ミニゲームのプレイヤーの処理の定義
//***************************************************
struct MinigamePlayerSystem : SystemBase
{
	MinigamePlayerSystem() = default;
	~MinigamePlayerSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void SetJetPack(entt::registry& registry, const entt::entity player, VelocityComponent& velocityComp, MotionComponent& motionComp);
	bool CollisionBall(entt::registry& registry, CapsuleColliderComponent& capsuleComp, Transform3DComponent& trasnformComp);
	void SetComboEffect(entt::registry& registry, const entt::entity player);
};
#endif