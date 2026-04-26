//===================================================
//
// プレイヤーの判定処理をまとめた名前空間 [player_spec.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player_spec.h"
#include "minigame_player_component.hpp"
#include "manager.h"
#include "input.h"
#include "velocity_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace PlayerSpec_Const
{
	constexpr float PAD_LEFT_ANGLE_MIN = -D3DXToRadian(150);	// パッドの左移動判定の最小角度
	constexpr float PAD_LEFT_ANGLE_MAX = -D3DXToRadian(30);		// パッドの左移動判定の最大角度

	constexpr float PAD_RIGHT_ANGLE_MIN = D3DXToRadian(30);		// パッドの右移動判定の最小角度
	constexpr float PAD_RIGHT_ANGLE_MAX = D3DXToRadian(150);	// パッドの右移動判定の最大角度

	constexpr float PAD_FORWARD_ANGLE_MIN = -D3DXToRadian(90);	// パッドの正面移動判定の最小角度
	constexpr float PAD_FORWARD_ANGLE_MAX = D3DXToRadian(90);	// パッドの正面移動判定の最大角度
}

//===================================================
// 素早い移動のインターバルの判定
//===================================================
bool PlayerSec::CQuickMoveInterval::IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const
{
	// ミニゲームのプレイヤーの取得
	auto& minigamePlayerComp = registry.get<MinigamePlayerComponent>(player);

	// インターバルがあるなら
	if (minigamePlayerComp.nQuickMoveInterval > 0)
	{
		return true;
	}
	return false;
}

//===================================================
// 素早い移動時間中かどうかの判定
//===================================================
bool PlayerSec::CCheckQuickMoveTime::IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const
{
	// ミニゲームのプレイヤーの取得
	auto& minigamePlayerComp = registry.get<MinigamePlayerComponent>(player);

	// 移動時間中なら
	if (minigamePlayerComp.nQuickMoveTime > 0)
	{
		return true;
	}
	return false;
}

//===================================================
// パッドで左移動をしているかどうか
//===================================================
bool PlayerSec::CJoyPadMoveLeft::IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const
{
	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	if (pJoyPad == nullptr) return false;

	// 移動量の取得
	const auto& velocityComp = registry.get<VelocityComponent>(player);

	D3DXVECTOR3 dir = velocityComp.move;
	D3DXVec3Normalize(&dir, &dir);

	// 移動方向の設定
	float fMoveAngle = atan2f(dir.x, dir.z);

	// 左移動だったら
	if (fMoveAngle >= PlayerSpec_Const::PAD_LEFT_ANGLE_MIN && fMoveAngle <= PlayerSpec_Const::PAD_LEFT_ANGLE_MAX)
	{
		return true;
	}

	return false;
}

//===================================================
// パッドで右移動をしているかどうか
//===================================================
bool PlayerSec::CJoyPadMoveRight::IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const
{
	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	if (pJoyPad == nullptr) return false;

	// 移動量の取得
	const auto& velocityComp = registry.get<VelocityComponent>(player);

	D3DXVECTOR3 dir = velocityComp.move;
	D3DXVec3Normalize(&dir, &dir);

	// 移動方向の設定
	float fMoveAngle = atan2f(dir.x, dir.z);

	// 右移動だったら
	if (fMoveAngle <= PlayerSpec_Const::PAD_RIGHT_ANGLE_MAX && fMoveAngle >= PlayerSpec_Const::PAD_RIGHT_ANGLE_MIN)
	{
		return true;
	}

	return false;
}

//===================================================
// パッドで正面に移動しているか
//===================================================
bool PlayerSec::CJoyPadMoveForward::IsSatisfiedBy(const entt::registry& registry, const entt::entity player) const
{
	// パッドの取得
	CInputJoypad* pJoyPad = CManager::GetInstance()->GetInputJoypad();

	if (pJoyPad == nullptr) return false;

	// 移動量の取得
	const auto& velocityComp = registry.get<VelocityComponent>(player);

	D3DXVECTOR3 dir = velocityComp.move;
	D3DXVec3Normalize(&dir, &dir);

	// 移動方向の設定
	float fMoveAngle = atan2f(dir.x, dir.z);

	// 正面移動だったら
	if (fMoveAngle >= PlayerSpec_Const::PAD_FORWARD_ANGLE_MIN && fMoveAngle <= PlayerSpec_Const::PAD_FORWARD_ANGLE_MAX)
	{
		return true;
	}

	return false;
}
