//===================================================
//
// デバッグ表示用コライダーの描画処理 [collider_renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLIDER_RENDERER_H_
#define _COLLIDER_RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_system_base.h"

//***************************************************
// 前方宣言
//***************************************************
struct BoxColliderComponent;		// 矩形コライダー
struct CapsuleColliderComponent;	// 矩カプセルコライダー
struct SphereColliderComponent;		// 球のコライダー

//***************************************************
// デバッグ表示用コライダーの描画処理の定義
//***************************************************
struct ColliderRenderer : RendererSystemBase
{
	ColliderRenderer() = default;
	~ColliderRenderer() = default;

	void Renderer(entt::registry& registry) override;
private:
	void Draw(std::vector<LINE_LIST>& lineList);
	void SetBoxVertex(entt::registry& registry, BoxColliderComponent& box);
	void SetCapsuleVertex(entt::registry& registry, CapsuleColliderComponent& capsuleComp);
	void SetSphereVertex(entt::registry& registry, SphereColliderComponent& sphereComp, const entt::entity sphereID);
};
#endif