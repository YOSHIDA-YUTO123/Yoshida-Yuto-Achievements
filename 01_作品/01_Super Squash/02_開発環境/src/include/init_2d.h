//===================================================
//
// 2Dのエンティティの初期化の処理 [init_2d.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _INIT_2D_H_
#define _INIT_2D_H_

//***************************************************
// 2Dの初期化名前空間の定義
//***************************************************
namespace Init2D
{
	static constexpr int MIN_SEGMENT = 1; // 最小の分割数

	HRESULT Sprite(entt::registry& registry, entt::entity entity, const int nSegmentU = MIN_SEGMENT, const int nSegmentV = MIN_SEGMENT);
};
#endif
