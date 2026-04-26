//===================================================
//
// メッシュの地面の読み込み処理 [mesh_field_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_FIELD_LOADER_H_
#define _MESH_FIELD_LOADER_H_

//***************************************************
// メッシュの地面の読み込み処理の定義
//***************************************************
class CMeshFieldLoader
{
public:
	static HRESULT FirstSceneLoad(entt::registry& registry, const char* pJsonFilePath);
private:
	CMeshFieldLoader() = default;
	~CMeshFieldLoader() = default;
};
#endif