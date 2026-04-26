//===================================================
//
// 合成条件 [spec_combinators.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SPEC_COMBINATORS_H_
#define _SPEC_COMBINATORS_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "specification_base.hpp"

//***************************************************
// 条件ORのクラスの定義
//***************************************************
class COrSpec : public CSpecification
{
public:
	COrSpec(const CSpecification& left, const CSpecification& right);
	~COrSpec() = default;

	bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
private:
	const CSpecification& m_reft;	// 条件1
	const CSpecification& m_right;	// 条件2
};

//***************************************************
// 条件ANDのクラスの定義
//***************************************************
class CAndSpec : public CSpecification
{
public:
	CAndSpec(const CSpecification& left, const CSpecification& right);
	~CAndSpec() = default;

	bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
private:
	const CSpecification& m_reft;	// 条件1
	const CSpecification& m_right;	// 条件2
};

//***************************************************
// 条件NOTのクラスの定義
//***************************************************
class CNotSpec : public CSpecification
{
public:
	explicit CNotSpec(const CSpecification& spec);
	~CNotSpec() = default;

	bool IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const override;
private:
	const CSpecification& m_spec;	// 条件1
};

#endif