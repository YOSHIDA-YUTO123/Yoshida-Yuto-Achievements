//===================================================
//
// タイトルの壁のデータ [title_wall_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TITLE_WALL_COMPONENT_HPP_
#define _TITLE_WALL_COMPONENT_HPP_

//***************************************************
// タイトルの壁のデータの定義
//***************************************************
struct TitleWallComponent
{
	TitleWallComponent(const D3DXVECTOR3& _posDest, const D3DXVECTOR3& _rotDest) :
		posDest(_posDest),rotDest(_rotDest),nLerpTime(0)
	{
		// クォータニオンの生成
		D3DXQuaternionRotationYawPitchRoll(&rotDest, _rotDest.y, _rotDest.x, _rotDest.z);
	}

	D3DXVECTOR3 posDest;	// 目的の位置
	D3DXQUATERNION rotDest;	// 目的の向き
	int nLerpTime;			// 補間時間
};
#endif