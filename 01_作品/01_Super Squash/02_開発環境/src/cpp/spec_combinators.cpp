//===================================================
//
// 合成条件 [spec_combinators.cppp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "spec_combinators.h"

//===================================================
// コンストラクタ
//===================================================
COrSpec::COrSpec(const CSpecification& left, const CSpecification& right) : 
	m_reft(left),
	m_right(right)
{
}

//===================================================
// 判定処理(OR)
//===================================================
bool COrSpec::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	return m_reft.IsSatisfiedBy(registry, entity) || m_right.IsSatisfiedBy(registry, entity);
}

//===================================================
// コンストラクタ
//===================================================
CAndSpec::CAndSpec(const CSpecification& left, const CSpecification& right) : 
	m_reft(left),
	m_right(right)
{
}

//===================================================
// 判定処理(AND)
//===================================================
bool CAndSpec::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	return m_reft.IsSatisfiedBy(registry, entity) && m_right.IsSatisfiedBy(registry, entity);
}

//===================================================
// コンストラクタ
//===================================================
CNotSpec::CNotSpec(const CSpecification& spec) : 
	m_spec(spec)
{
}

//===================================================
// 判定処理(NOT)
//===================================================
bool CNotSpec::IsSatisfiedBy(const entt::registry& registry, const entt::entity entity) const
{
	return !m_spec.IsSatisfiedBy(registry, entity);
}
