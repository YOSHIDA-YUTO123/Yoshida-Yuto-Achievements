//===================================================
//
// メッシュのサークルのデータ [mesh_circle_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_CIRCLE_COMPONENT_HPP_
#define _MESH_CIRCLE_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// メッシュのサークルのデータの定義
//***************************************************
struct MeshCircleComponent
{
	MeshCircleComponent(const float fAlphaLv, const float _fSpeed, const float _fInRadius, const float _fOutRadius, const int _nLife) :
		inColor(Const::WHITE), outColor(Const::WHITE)	, fDecAlphaLv(fAlphaLv / _nLife),
		fSpeed(_fSpeed)		 , fInRadius(_fInRadius)	, fOutRadius(_fOutRadius)		,
		fHeight(0.0f)		 , nLife(_nLife)
	{}

	D3DXCOLOR inColor;	// 内側の色
	D3DXCOLOR outColor;	// 外側の色
	float fDecAlphaLv;	// アルファ値の減少スピード
	float fSpeed;		// 速度
	float fInRadius;	// 内側の半径
	float fOutRadius;	// 外側の半径
	float fHeight;		// 高さ
	int nLife;			// 寿命
};
#endif