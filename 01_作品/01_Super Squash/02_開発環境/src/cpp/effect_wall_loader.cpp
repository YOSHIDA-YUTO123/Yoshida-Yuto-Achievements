//================================================
//
// エフェクトの壁の読み込み処理 [effect_wall_loader.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "effect_wall_loader.h"
#include "json_manager.h"
#include "factory_mesh.h"
#include "loader_constants.hpp"

//================================================
// 読み込み処理
//================================================
HRESULT CEffectWallLoader::Load(entt::registry& registry)
{
	// 名前空間の使用
	using namespace nlohmann;
	using namespace LoaderConst;

	json config;

	// 読み込み処理
	if (FAILED(JsonManager::LoadJson(EFFECT_WALL_PATH, config)))
	{
		return E_FAIL;
	}

	// キーが読み取れなかったら
	if (!config.contains("effect_walls"))
	{
		return E_FAIL;
	}

	// 壁の数分回す
	for (auto walls : config["effect_walls"])
	{
		// 位置、向き、大きさの取得
		D3DXVECTOR3 pos = JsonManager::LoadVector3(walls["pos"]);
		D3DXVECTOR3 rot = JsonManager::LoadVector3(walls["rot"]);
		D3DXVECTOR3 size = JsonManager::LoadVector3(walls["size"]);

		// 分割数の取得
		int nSegment = walls["segment"];

		// エフェクトの壁の生成処理
		FactoryMesh::Create::EffectWall(registry, pos, size, rot, nullptr, nSegment);	// 右の壁
	}

	return S_OK;
}
