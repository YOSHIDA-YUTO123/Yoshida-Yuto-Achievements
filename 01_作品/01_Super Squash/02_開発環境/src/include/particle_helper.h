//===================================================
//
// パーティクルのヘルパークラス [particle_helper.h]
// Author:YUTO YOSHIDA
// 
// << パラメータをあらかじめ読み取って保存 >>
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PARTICLE_HELPER_H_
#define _PARTICLE_HELPER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "particle_component.hpp"

//***************************************************
// 前方宣言
//***************************************************

//***************************************************
// パーティクルのヘルパークラスの定義
//***************************************************
class CParticleHelper
{
public:
	CParticleHelper();
	~CParticleHelper();

	HRESULT Load(const char* pJsonFileName);
	void GetParam(ParticleComponent::Param& param, const D3DXVECTOR3& pos, const std::string& key, const unsigned int nFlag);
private:
	std::unordered_map<std::string, ParticleComponent::Param> m_param; // 読み取ったパラメータ
};
#endif