//===================================================
//
// アウトラインシェーダーを適応するためのデータ [outline_shader_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OUTLINE_SHADER_COMPONENT_HPP_
#define _OUTLINE_SHADER_COMPONENT_HPP_

//***************************************************
// アウトラインシェーダーを適応するためのデータの定義
//***************************************************
struct OutLineShaderComponent
{
	static  constexpr float BASE_SIZE = 1.0f;	// アウトラインの大きさ

	OutLineShaderComponent(const D3DXCOLOR& _col = Const::WHITE, const float _fSize = BASE_SIZE) :
		col(_col), fSize(_fSize)
	{}

	D3DXCOLOR col;	// 色
	float fSize;	// 大きさ
};
#endif