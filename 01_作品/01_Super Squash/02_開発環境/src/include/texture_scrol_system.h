//===================================================
//
// テクスチャのスクロール処理 [TextureScrolSystem.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_SCROL_SYSTEM_H_
#define _TEXTURE_SCROL_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "system_base.h"

//***************************************************
// テクスチャのスクロールの処理の定義
//***************************************************
struct TextureScrolSystem : SystemBase
{
	TextureScrolSystem() = default;
	~TextureScrolSystem() = default;

	void Update(entt::registry& registry) override;
};
#endif