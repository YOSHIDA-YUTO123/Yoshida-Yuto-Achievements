//===================================================
//
// jsonの読み込みの補助 [json_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _JSON_MANAGER_H_
#define _JSON_MANAGER_H_

//***************************************************
// jsonのマネージャーの名前空間の定義
//***************************************************
namespace JsonManager
{
	D3DXVECTOR3 LoadVector3(const nlohmann::json& json);
	D3DXVECTOR2 LoadVector2(const nlohmann::json& json);
	D3DXQUATERNION LoadQuaternion(const nlohmann::json& json);
	D3DXCOLOR LoadRBGA(const nlohmann::json& json);
	HRESULT LoadJson(const char* pFilePath, nlohmann::json& config);
}
#endif
