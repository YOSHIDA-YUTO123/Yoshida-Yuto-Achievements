//===================================================
//
// jsonの読み込みの補助 [json_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "json_manager.h"
#include "fstream"

//***************************************************
// 定数宣言
//***************************************************
namespace JsonManagerConstants
{
    constexpr const char* DATA_FILE_PATH = "data/SYSTEM/";  // 省略用ファイルパス
}

//==================================================
// vector3のロード処理
//==================================================
D3DXVECTOR3 JsonManager::LoadVector3(const nlohmann::json& json)
{
    D3DXVECTOR3 outVec3;

    outVec3.x = json[0].get<float>();
    outVec3.y = json[1].get<float>();
    outVec3.z = json[2].get<float>();

    return outVec3;
}

//==================================================
// vector2のロード処理
//==================================================
D3DXVECTOR2 JsonManager::LoadVector2(const nlohmann::json& json)
{
    D3DXVECTOR2 outVec2;

    outVec2.x = json[0].get<float>();
    outVec2.y = json[1].get<float>();

    return outVec2;
}

//===================================================
// クォータニオンのロード処理
//===================================================
D3DXQUATERNION JsonManager::LoadQuaternion(const nlohmann::json& json)
{
    D3DXQUATERNION outQuat;

    outQuat.x = json["x"];
    outQuat.y = json["y"];
    outQuat.z = json["z"];
    outQuat.w = json["w"];

    return outQuat;
}

//===================================================
// 色のロード処理
//===================================================
D3DXCOLOR JsonManager::LoadRBGA(const nlohmann::json& json)
{
    D3DXCOLOR outCol;

    outCol.r = json[0].get<float>();
    outCol.g = json[1].get<float>();
    outCol.b = json[2].get<float>();
    outCol.a = json[3].get<float>();

    return outCol;
}

//===================================================
// jsonファイルのロード処理
//===================================================
HRESULT JsonManager::LoadJson(const char* pFilePath, nlohmann::json& config)
{
    // 名前空間の使用
    using namespace nlohmann;
    using namespace JsonManagerConstants;

    // 省略用ファイルパス
    std::string filePath = pFilePath;

    // ファイルパスが省略されていたら
    if (filePath.find(DATA_FILE_PATH) == std::string::npos)
    {
        // 文字列を連結
        filePath = DATA_FILE_PATH + filePath;
    }

    // ファイルを開く
    std::fstream file(filePath);
    
    // ファイルが開けなかったら
    if (!file.is_open())
    {
        MessageBox(NULL, "ファイルが開けません", filePath.c_str(), MB_OK | MB_ICONWARNING);
        return E_FAIL;
    }

    // 読み込んだ値を設定
    file >> config;

    // ファイルをクリア
    file.close();
    file.clear();

    return S_OK;
}
