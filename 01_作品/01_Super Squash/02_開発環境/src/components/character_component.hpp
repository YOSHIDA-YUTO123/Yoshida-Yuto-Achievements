//===================================================
//
// キャラクターのデータ [character_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_COMPONENT_HPP_
#define _CHARACTER_COMPONENT_HPP_

//***************************************************
// キャラクターのデータの定義
//***************************************************
struct CharacterComponent
{
	static constexpr float BASE_ROT_SPEED	= 0.2f; // 目的の向きに回転するまでの速度
	static constexpr float SHOT_POWER		= 5.0f; // 球を打つ力

	// 状態
	enum class State : uint8_t
	{
		Normal = 0, // 通常状態
		Move,		// 移動状態
		Action,		// 攻撃
		Jump,		// ジャンプ
		Max
	};

	CharacterComponent(const float _fSpeed, const float _fRotSpeed = BASE_ROT_SPEED) :
		state(State::Normal) , shadowID(entt::null)			, shotAngle(Const::VEC3_NULL),
		fRotDest(0.0f)		 , fSpeed(_fSpeed)				, fShotPower(SHOT_POWER),
		fRotSpeed(_fRotSpeed), bServe(false)
	{}
	
	State state;			// 状態
	entt::entity shadowID;	// 影のID
	D3DXVECTOR3 shotAngle;	// 上下方向の発射角度		
	float fRotDest;			// 目的の向き
	float fSpeed;			// 移動速度
	float fRotSpeed;		// 回転スピード
	float fShotPower;		// 打つ力
	bool bServe;			// サーブできるかどうか
};
#endif