//===================================================
//
// マルチレンダーターゲットのIDのコンポーネント [mrt_target_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MRT_TARGET_COMPONENT_HPP_
#define _MRT_TARGET_COMPONENT_HPP_

//***************************************************
// マルチレンダーターゲットのIDのデータの定義
//***************************************************
struct MRTTargetComponent
{
	MRTTargetComponent(const int _nTarget) : 
		nTarget(_nTarget)
	{}

	int nTarget; // ターゲット
};
#endif