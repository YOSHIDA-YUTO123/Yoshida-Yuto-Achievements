//===================================================
//
// メッシュの壁の読み込み処理 [mesh_wall_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WALL_LOADER_H_
#define _MESH_WALL_LOADER_H_

//***************************************************
// シーンのentitiyの生成処理の定義
//***************************************************
class CMeshWallLoader
{
public:
	static HRESULT FirstSceneLoad(entt::registry& registry);
	static HRESULT GameSceneLoad(entt::registry& registry);
private:
	CMeshWallLoader() = default;
	~CMeshWallLoader() = default;
};
#endif