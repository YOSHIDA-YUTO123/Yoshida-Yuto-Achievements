//===================================================
//
// シーンのentityの生成処理 [scene_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCENE_LOADER_H_
#define _SCENE_LOADER_H_

//***************************************************
// シーンのentitiyの生成処理の定義
//***************************************************
namespace Loader::Scene
{
	HRESULT FirstScene(entt::registry& registry);
	HRESULT Minigame(entt::registry& registry);
	HRESULT BaseScene(entt::registry& registry);
}
#endif