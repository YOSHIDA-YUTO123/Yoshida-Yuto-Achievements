//===================================================
//
// 煙、爆発のマネージャーの定義 [explotion_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "explotion_manager.h"
#include "factory_billboard.h"
#include "color_constants.hpp"
#include "effect_constants.hpp"
#include "effect_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace ExplotionConst
{
	constexpr float RADIUS			= 30.0f;		// 煙の大きさ
	constexpr int LIFE				= 30;			// 寿命
	constexpr int SPEED				= 3;			// アニメーションの速さ
	constexpr int TEXTURE_SEGMENT_U	= 4;			// 横の分割数
	constexpr int TEXTURE_SEGMENT_V	= 3;			// 縦の分割数
}

//===================================================
// ボール着地時の煙
//===================================================
void ExplotionManager::BallLandingExplotion(entt::registry& registry, const D3DXVECTOR3& pos, const int nNum, const D3DXVECTOR3& move)
{
	// 名前空間の使用
	using namespace ExplotionConst;

	// 総数分回す
	for (int nCnt = 0; nCnt <= nNum; nCnt++)
	{
		// 一周の割合を求める
		float fAngle = Const::TWO_PI / nNum * nCnt;

		D3DXVECTOR3 moveWk = Const::VEC3_NULL;

		// 移動量の設定
		moveWk.x = sinf(fAngle) * move.x;
		moveWk.y = move.y;
		moveWk.z = cosf(fAngle) * move.z;

		// エフェクトの生成
		auto effectID = FactoryBillboard::Create::Effect(registry, pos, Color::WHITE, RADIUS, EffectConstants::TEXTURE_SMOKE);
		FactoryBillboard::Build::Effect(registry, effectID, LIFE, moveWk, EffectComponent::FLAG_DEC_ALPHA | EffectComponent::FLAG_DEC_RADIUS);
		FactoryBillboard::Build::AnimationEffect(registry, effectID, SPEED, false, TEXTURE_SEGMENT_U, TEXTURE_SEGMENT_V);
	}
}