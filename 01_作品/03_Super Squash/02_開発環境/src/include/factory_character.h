//===================================================
//
// キャラクターのファクトリーの処理 [factory_character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_CHARACTER_H_
#define _FACTORY_CHARACTER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// キャラクターのファクトリーの名前空間の定義
//***************************************************
namespace FactoryCharacter
{
	entt::entity CreateCharacter(entt::registry& registry, const char* pMotionFile, const int nNumMotion, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	void BuildPlayer(entt::registry& registry, entt::entity player);
	void BuildMinigamePlayer(entt::registry& registry, entt::entity player);
}
#endif