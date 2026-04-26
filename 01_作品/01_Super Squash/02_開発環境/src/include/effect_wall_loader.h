//================================================
//
// エフェクトの壁の読み込み処理 [effect_wall_loader.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _EFFECT_WALL_LOADER_H_
#define _EFFECT_WALL_LOADER_H_

//************************************************
// エフェクトの壁の読み込み処理の定義
//************************************************
class CEffectWallLoader
{
public:
	static HRESULT Load(entt::registry& registry);
private:
	CEffectWallLoader() = default;
	~CEffectWallLoader() = default;
};
#endif