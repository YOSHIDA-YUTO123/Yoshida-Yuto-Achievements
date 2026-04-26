//===================================================
//
// テクスチャアニメーションの処理 [texture_animation_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_ANIMATION_SYSTEM_H_
#define _TEXTURE_ANIMATION_SYSTEM_H_

//***************************************************
// エンティティのプールクラスの定義
//***************************************************
#include "system_base.h"

//***************************************************
// テクスチャアニメーションの処理の定義
//***************************************************
struct TextureAnimationSystem : SystemBase
{
	TextureAnimationSystem() = default;
	~TextureAnimationSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif