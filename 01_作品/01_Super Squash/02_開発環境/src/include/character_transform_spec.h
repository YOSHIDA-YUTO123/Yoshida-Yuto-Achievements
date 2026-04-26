//===================================================
//
// キャラクターの位置、向きの条件クラス [character_transform_spec.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_TRANSFORM_SPEC_H_
#define _CHARACTER_TRANSFORM_SPEC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// キャラクターの条件(位置,向き系)の名前空間の定義
//***************************************************
namespace CharacterSpec::Transform
{
	// ボールがキャラクターの左にいるか判定
	class CBallOnLeftSide : public CSpecification
	{
	public:
		CBallOnLeftSide() = default;
		~CBallOnLeftSide() = default;

		bool IsSatisfiedBy(const entt::registry& registry, const entt::entity character) const override;
	private:
	};

};
#endif