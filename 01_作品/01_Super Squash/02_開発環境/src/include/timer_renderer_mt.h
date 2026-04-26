//===================================================
//
// タイマーのMRTの描画 [timer_renderer_mt.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TIMER_RENDERER_MT_H_
#define _TIMER_RENDERER_MT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "renderer_mrt_system_base.h"

//***************************************************
// タイマーのMRTのテクスチャの作成処理の定義
//***************************************************
struct TimerRendererMT : RendererMRTSystemBase
{
	TimerRendererMT() = default;
	~TimerRendererMT() = default;

	void Renderer(entt::registry& registry, const int nType) override;
private:
	void Draw(entt::registry& registry,const entt::entity time_entity);
};
#endif
