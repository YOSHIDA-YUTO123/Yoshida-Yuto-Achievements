//===================================================
//
// 優先順位コンポーネント [layer_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LAYER_COMPONENT_HPP_
#define _LAYER_COMPONENT_HPP_

//***************************************************
// レイヤーのコンポーネント
//***************************************************
struct LayerComponent
{
	static constexpr int BASE_LAYER = 3;	// レイヤー

	LayerComponent(const int _nLayer = BASE_LAYER) : nLayer(_nLayer) {}

	int nLayer;			// 自分のレイヤー
};
#endif