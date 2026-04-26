//===================================================
//
// プレイヤーを追従するカメラの処理 [follow_camera.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "follow_camera.h"
#include "tag_component.hpp"
#include "transform_component.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace FollowCameraConst
{
	const D3DXVECTOR3 DEST_POS_V	= { 0.0f,233.0f,-360.0f };	// 目的の視点
	const D3DXVECTOR3 DEST_POS_R	= { 0.0f,185.0f,-231.0f };	// 目的の注視点
	const D3DXVECTOR3 DEST_ROT		= { 1.82f,0.0f,0.0f };		// 目的の角度

	const D3DXVECTOR3 DEFAULT_POS_V = { 0.0f,208.0f,-699.0f };	// 元の視点
	const D3DXVECTOR3 DEFAULT_POS_R = { 0.0f,131.0f,-395.0f };	// 元の注視点
	const D3DXVECTOR3 DEFAULT_ROT	= { 1.82f,0.0f,0.0f };		// 元の角度

	constexpr float FOLLOW_POS_Z = -100.0f;						// 追従する位置
	constexpr float FOLLOW_RATE = 0.04f;						// 減衰率
}

//===================================================
// コンストラクタ
//===================================================
CFollowCamera::CFollowCamera(entt::registry& registry) :
	m_registry(registry)
{}

//===================================================
// 更新処理
//===================================================
void CFollowCamera::Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot)
{
	// 定数の使用
	using namespace FollowCameraConst;

	// プレイヤーの取得
	auto playerID = m_registry.view<Tag::PlayerTag>().front();

	// 使用できないIDなら
	if (!m_registry.valid(playerID))
	{
		return;
	}

	// コンポーネントの取得
	auto& transformComp = m_registry.get<Transform3DComponent>(playerID);

	// プレイヤーが一定の位置まで行ったら
	if (transformComp.pos.z >= FOLLOW_POS_Z)
	{
		// 目的の位置、角度にする
		posV += (DEST_POS_V - posV) * FOLLOW_RATE;
		posR += (DEST_POS_R - posR) * FOLLOW_RATE;
		rot += (DEST_ROT - rot) * FOLLOW_RATE;
	}
	else
	{
		// もとに戻す
		posV += (DEFAULT_POS_V - posV) * FOLLOW_RATE;
		posR += (DEFAULT_POS_R - posR) * FOLLOW_RATE;
		rot += (DEFAULT_ROT - rot) * FOLLOW_RATE;
	}
}