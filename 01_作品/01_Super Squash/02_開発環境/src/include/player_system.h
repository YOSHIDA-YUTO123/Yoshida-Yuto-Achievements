//===================================================
//
// プレイヤー制御処理 [player_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_SYSTEM_H_
#define _PLAYER_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// 前方宣言
//***************************************************
class CInputKeyboard;		// キーボード
class CInputJoypad;			// パッド
struct MotionComponent;		// モーションのデータ
struct VelocityComponent;	// 移動量のデータ
struct CharacterComponent;	// キャラクターのコンポーネント

//***************************************************
// プレイヤーの更新処理の定義
//***************************************************
struct PlayerSystem : SystemBase
{
	PlayerSystem() = default;
	~PlayerSystem() = default;

	void Update(entt::registry& registry) override;
private:
	void InputMove(entt::registry& registry, const entt::entity player, CInputKeyboard* pKeyboard, CInputJoypad* pJoypad, MotionComponent& motionComp, VelocityComponent& velocityComp);
	bool KeyboardMove(entt::registry& registry, const entt::entity player, CInputKeyboard* pKeyboard, CInputJoypad* pJoypad, VelocityComponent& velocityComp);
	bool JoyPadMove(entt::registry& registry, const entt::entity player, CInputJoypad* pJoypad, VelocityComponent& velocityComp);
	void StartShotOrServeMotion(entt::registry& registry, const entt::entity player, MotionComponent& motionComp, CharacterComponent& characterComp);
	void ReleaseChargeKey(entt::registry& registry, entt::entity player, MotionComponent& motionComp, CharacterComponent& characterComp);
	void SetShotPitchAngle(entt::registry& registry, const entt::entity player,CharacterComponent& characterComp, CInputKeyboard* pKeyboard, CInputJoypad* pJoyPad);
	void SetCollisionEffectWall(entt::registry& registry, const entt::entity player);
};
#endif