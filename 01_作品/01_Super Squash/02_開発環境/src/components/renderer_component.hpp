//===================================================
//
// 描画のコンポーネント [renderer_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_COMPONENT_HPP_
#define _RENDERER_COMPONENT_HPP_

//***************************************************
// 描画のコンポーネントの定義
//***************************************************
struct RendererComponent
{
	// 種類
	enum FLAG
	{
		TYPE_NONE  			= 0 << 0,	// 通常状態
		TYPE_LIGHT_OFF		= 1 << 1,	// ライトの影響を受けない
		TYPE_ZTEST			= 1 << 2,	// Zテスト
		TYPE_ALPHA_TEST		= 1 << 3,	// Aテスト		
		TYPE_ALPHA_BLEND	= 1 << 4,	// 加算合成
		TYPE_NO_CULL		= 1 << 5,	// カリングオフ
		TYPE_CULL_FRONT		= 1 << 6,	// 表面をカリング
		TYPE_SUB_BLEND		= 1 << 7,	// 減算合成
		TYPE_NO_DISP		= 1 << 8,	// 表示しない
		TYPE_MAX
	};
	
	RendererComponent(const int _nFlag = TYPE_NONE) :
		nFlag(_nFlag) {}

	int nFlag; // フラグ
};
#endif