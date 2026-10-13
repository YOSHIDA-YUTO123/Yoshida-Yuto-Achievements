//==============================================
//
// ステージをロードするクラス [StageLoader.cpp]
// Author:YUTO YOSHIDA
// 
// << タイトル、ゲーム等のステージをロードするクラス >>
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "StageLoader.h"
#include <fstream>
#include "CollisionMapObjManager.h"
#include "CollisionMapObject.h"

//***************************************************
// 定数宣言
//***************************************************
namespace LOAD_INFO
{
	const char* FILE_GAME_INFO = "data/FILE/game.json"; // ゲームのオブジェクトの情報を読み込むjsonファイル
	const char* FILE_STAGE = "data/FILE/stage000.json"; // ステージの情報を読み込むjsonファイル
	const char* KEY_GAMEOBJECT = "GAME_OBJECT";			// ゲームオブジェクトのキー
	const char* KEY_POSITION = "position";				// 位置のキー
	const char* KEY_MESHFILED = "MESH_FIELD";			// メッシュフィールドのキー
	const char* KEY_SEGMENT = "segment";				// 分割数のキー
	const char* KEY_SIZE = "size";						// サイズのキー
	const char* KEY_TEXTUREID = "texture";				// テクスチャのキー
	const char* KEY_ROTATION = "rotation";				// 角度のキー
};

//***************************************************
// 静的メンバ変数
//***************************************************
CCollisionMapObjManager* CStageLoader::m_pMapObjectManager = nullptr; // マップオブジェクトのマネージャーの参照

//==============================================
// コンストラクタ
//==============================================
CStageLoader::CStageLoader()
{
}

//==============================================
// デストラクタ
//==============================================
CStageLoader::~CStageLoader()
{
}

//==============================================
// ロード処理
//==============================================
HRESULT CStageLoader::Load(CCollisionMapObjManager* pMapObjectManager, const char* pStageJsonPath)
{
	m_pMapObjectManager = pMapObjectManager;

	// GAME_INFOの使用
	using namespace LOAD_INFO;

	// jsonの使用
	using nlohmann::json;

	json config;

	// 省略用ファイルパス
	std::string Path = pStageJsonPath;
	std::string filePath;

	if (Path.find("data/FILE/") == std::string::npos)
	{
		filePath = "data/FILE/";
	}

	// 文字列の連結
	filePath += pStageJsonPath;

	// ファイルを開く
	std::ifstream file(filePath);

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "エラー", "ファイルが開けませんでした", MB_OK);
		return E_FAIL;
	}

	// 要素分調べる
	for (auto& obj : config["MODEL_INFO"])
	{
		std::string filepath = obj["file_path"];
		float posX = obj["position"]["x"];
		float posY = obj["position"]["y"];
		float posZ = obj["position"]["z"];

		float rotX = obj["rotation"]["x"];
		float rotY = obj["rotation"]["y"];
		float rotZ = obj["rotation"]["z"];

		std::string apType = obj["type"];

		// リストに追加
		m_pMapObjectManager->AddList(CCollisionMapObject::Create({ posX ,posY ,posZ }, { rotX ,rotY ,rotZ }, filepath.c_str()));
	}

	return S_OK;
}
