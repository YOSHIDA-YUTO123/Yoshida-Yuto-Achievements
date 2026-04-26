//===================================================
//
// ビルボードのファクトリーの処理 [factory_billboard.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_BILLBOARD_H_
#define _FACTORY_BILLBOARD_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "particle_component.hpp"

//***************************************************
// ビルボードのファクトリー処理
//***************************************************
namespace FactoryBillboard
{
	// 生成
	namespace Create
	{
		entt::entity Billboard(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const char* pTexturePath);
		entt::entity Effect(entt::registry& registry, const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const char* pTexturePath);
		entt::entity Particle(entt::registry& registry, ParticleComponent::Param& param);
	}

	// 構築
	namespace Build
	{
		void Effect(entt::registry& registry, entt::entity entity, const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const char* pTexturePath);
		void Effect(entt::registry& registry, entt::entity entity, const int nLife, const D3DXVECTOR3 move, const unsigned int nFlag = 0);
		void Effect(entt::registry& registry, entt::entity entity, const float fGravity);
		void AnimationEffect(entt::registry& registry, entt::entity entity, const int nSpeed, const bool bLoop, const int nSegmentU, const int nSegmentV);
		void AnimationParticle(entt::registry& registry, entt::entity entity, const int nSpeed, const bool bLoop, const int nSegmentU, const int nSegmentV);
	}
};

//***************************************************
// ビルボードの初期化処理
//***************************************************
namespace InitBillboard
{
	static constexpr int MIN_SEGMENT = 1;	// 最小の分割数

	HRESULT CreateBillboard(entt::registry& registry, entt::entity entity, const int nSegmentU = MIN_SEGMENT, const int nSegmentV = MIN_SEGMENT);
};

#endif