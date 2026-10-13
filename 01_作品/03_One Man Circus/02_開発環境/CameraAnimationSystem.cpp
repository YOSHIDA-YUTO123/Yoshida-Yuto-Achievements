//===================================================
//
// カメラのアニメーションの処理 [CameraAnimationSystem.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "CameraAnimationSystem.h"
#include "math.h"
#include <fstream>
#include "json.hpp"
#include "camera.h"

//***************************************************
// 定数宣言
//***************************************************
namespace Animation_Constants
{
	constexpr const char* KEY_CAMERA_SET = "Camera_Set"; // カメラセット
	constexpr const char* KEY_KEY_INFO = "key_info";	 // キーの情報
}

//===================================================
// コンストラクタ
//===================================================
CCameraAnimationSystem::CCameraAnimationSystem(CCamera* pCamera) : 
	m_pCameraRef(pCamera),
	m_aInfo(),
	m_nCounter(NULL),
	m_nNextKey(NULL),
	m_nKey(NULL),
	m_nType(NULL),
	m_state(State::Idle)
{
}

//===================================================
// デストラクタ
//===================================================
CCameraAnimationSystem::~CCameraAnimationSystem()
{
}

//===================================================
// 再生
//===================================================
void CCameraAnimationSystem::Play(const int nType)
{
	// 再生状態だったら処理しない
	if (m_state == State::Play) return;

	m_state = State::Play;

	m_nKey = 0;
	m_nNextKey = 0;
	m_nCounter = 0;
	m_nType = nType;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCameraAnimationSystem::Init(const char* pFilePath)
{
	// ロード処理
	if (FAILED(Load(pFilePath)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCameraAnimationSystem::Uninit(void)
{
	// 情報分回す
	for (auto& info : m_aInfo)
	{
		// クリア
		info.aKey_Info.clear();
	}

	m_pCameraRef = nullptr;

	// 要素のクリア
	m_aInfo.clear();
}

//===================================================
// 更新処理
//===================================================
void CCameraAnimationSystem::Update(void)
{
	// 再生状態じゃないなら処理しない
	if (m_state != State::Play) return;

	// 0割り対策
	if (m_aInfo[m_nType].nNumKey == 0) return;

	// キーの総数分回す
	m_nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	// 次のキーの注視点までの差分を求める
	D3DXVECTOR3 DiffR = m_aInfo[m_nType].aKey_Info[m_nNextKey].key.posR - m_aInfo[m_nType].aKey_Info[m_nKey].key.posR;
	D3DXVECTOR3 DiffV = m_aInfo[m_nType].aKey_Info[m_nNextKey].key.posV - m_aInfo[m_nType].aKey_Info[m_nKey].key.posV;

	// 割合を求める
	float fRateFrame = m_nCounter / static_cast<float>(m_aInfo[m_nType].aKey_Info[m_nKey].nFrame);

	D3DXVECTOR3 posR,posV;

	// 視点の補間処理
	posV.x = m_aInfo[m_nType].aKey_Info[m_nKey].key.posV.x + DiffV.x * fRateFrame;
	posV.y = m_aInfo[m_nType].aKey_Info[m_nKey].key.posV.y + DiffV.y * fRateFrame;
	posV.z = m_aInfo[m_nType].aKey_Info[m_nKey].key.posV.z + DiffV.z * fRateFrame;

	// 注視点の補間処理
	posR.x = m_aInfo[m_nType].aKey_Info[m_nKey].key.posR.x + DiffR.x * fRateFrame;
	posR.y = m_aInfo[m_nType].aKey_Info[m_nKey].key.posR.y + DiffR.y * fRateFrame;
	posR.z = m_aInfo[m_nType].aKey_Info[m_nKey].key.posR.z + DiffR.z * fRateFrame;

	if (m_pCameraRef != nullptr)
	{
		// 位置の設定
		m_pCameraRef->SetPosR(posR);
		m_pCameraRef->SetPosV(posV);
	}

	// カウンターがフレームを超えたら
	if (m_nCounter >= m_aInfo[m_nType].aKey_Info[m_nKey].nFrame)
	{
		m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

		m_nCounter = 0;
	}

	m_nCounter++;

	// ループしないかつ最後のキーまでいったら
	if (m_nKey >= m_aInfo[m_nType].nNumKey - 1 && !m_aInfo[m_nType].bLoop)
	{
		// 止める
		m_state = State::Idle;
	}
}

//===================================================
// ロード処理
//===================================================
HRESULT CCameraAnimationSystem::Load(const char* pFilePath)
{
	// 名前空間の使用
	using namespace nlohmann;

	// ファイルをロードする
	std::ifstream file(pFilePath);

	// 開けなかったら
	if (file.is_open() == false)
	{
		MessageBox(NULL, "ファイルが開けません", pFilePath, MB_OK);
		return E_FAIL;
	}

	json load_info;

	// 情報を読み込む
	file >> load_info;

	// ファイルを閉じる
	file.close();
	file.clear();

	json camera_set = load_info[Animation_Constants::KEY_CAMERA_SET];

	int nCntMotion = 0;

	// メモリの確保
	m_aInfo.resize(5);

	// 情報の取得
	m_aInfo[nCntMotion].bLoop = camera_set["Loop"];

	int nCntKey = 0;

	// キーの情報分回す
	for (auto& key_info_json : camera_set[Animation_Constants::KEY_KEY_INFO])
	{
		Key_Info key_info;

		key_info.nFrame = key_info_json["frame"];

		// 視点の取得
		key_info.key.posV.x = key_info_json["posV"]["x"];
		key_info.key.posV.y = key_info_json["posV"]["y"];
		key_info.key.posV.z = key_info_json["posV"]["z"];

		// 注視点の取得
		key_info.key.posR.x = key_info_json["posR"]["x"];
		key_info.key.posR.y = key_info_json["posR"]["y"];
		key_info.key.posR.z = key_info_json["posR"]["z"];

		m_aInfo[nCntMotion].aKey_Info.push_back(key_info);

		nCntKey++;
	}

	m_aInfo[nCntMotion].nNumKey = nCntKey;

	return S_OK;
}
