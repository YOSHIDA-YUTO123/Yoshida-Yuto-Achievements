//===================================================
//
//  プレイヤーの読み込み処理 [player_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_LOADER_H_
#define _PLAYER_LOADER_H_

//***************************************************
// プレイヤーの読み込み処理の定義
//***************************************************
class CPlayerLoader
{
public:
	static HRESULT Load(entt::registry& registry);
private:
	CPlayerLoader() = default;
	~CPlayerLoader() = default;
};
#endif