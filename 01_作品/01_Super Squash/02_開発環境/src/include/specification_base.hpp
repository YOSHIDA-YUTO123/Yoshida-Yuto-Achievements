//===================================================
//
// 仕様のインターフェースの処理 [specification_base.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SPECIFICATION_BASE_HPP_
#define _SPECIFICATION_BASE_HPP_

//***************************************************
// 仕様クラスの定義
//***************************************************
class CSpecification
{
public:
	CSpecification() = default;
	virtual ~CSpecification() = default;

	virtual bool IsSatisfiedBy(const entt::registry& registry,const entt::entity entity) const = 0;
};
#endif