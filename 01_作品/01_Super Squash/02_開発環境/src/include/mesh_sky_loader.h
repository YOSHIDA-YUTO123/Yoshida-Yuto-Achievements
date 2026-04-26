//===================================================
//
// メッシュの地面の読み込み処理 [mesh_sky_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_SKY_LOADER_H_
#define _MESH_SKY_LOADER_H_

//***************************************************
// メッシュの空の読み込み処理の定義
//***************************************************
class CMeshSkyLoader
{
public:
	static HRESULT Load(entt::registry& registry);
private:
	CMeshSkyLoader() = default;
	~CMeshSkyLoader() = default;
};
#endif