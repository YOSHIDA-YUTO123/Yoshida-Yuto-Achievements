//===================================================
//
// systemの基底クラス [SystemBase.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SYSTEMBASE_H_
#define _SYSTEMBASE_H_

//***************************************************
// systemの基底構造体の定義
//***************************************************
struct SystemBase
{
	SystemBase() = default;
	virtual ~SystemBase() = default;
	
	virtual void Update(entt::registry& registry) = 0;
};
#endif