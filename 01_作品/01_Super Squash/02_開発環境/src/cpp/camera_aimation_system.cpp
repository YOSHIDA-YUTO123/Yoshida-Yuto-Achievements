//===================================================
//
// カメラのアニメーションの処理 [camera_aimation_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_animation_system.h"
#include "utility_math.h"
#include "fstream"
#include "json.hpp"
#include "camera.h"
#include "json_manager.h"

//***************************************************
// 定数宣言
//***************************************************
namespace AnimationConstants
{
	constexpr const char* KEY_CAMERA_SET	= "camera_set";		// カメラセット
	constexpr const char* KEY_KEY_INFO		= "key_info";		// キーの情報
	constexpr int RESERVE					= 10;				// 初期化で確保するメモリ
}

//===================================================
// コンストラクタ
//===================================================
CCameraAnimationSystem::CCameraAnimationSystem() :
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
	// 情報分回す
	for (auto& info : m_aInfo)
	{
		// クリア
		info.aKeyInfo.clear();
	}

	// 要素のクリア
	m_aInfo.clear();
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
// 更新処理
//===================================================
void CCameraAnimationSystem::Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot)
{
	// 再生状態じゃないなら処理しない
	if (m_state != State::Play) return;

	// 0割り対策
	if (m_aInfo[m_nType].nNumKey == 0) return;

	// キーの総数分回す
	m_nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	// 次のキーの注視点までの差分を求める
	D3DXVECTOR3 DiffR = m_aInfo[m_nType].aKeyInfo[m_nNextKey].key.posR - m_aInfo[m_nType].aKeyInfo[m_nKey].key.posR;
	D3DXVECTOR3 DiffV = m_aInfo[m_nType].aKeyInfo[m_nNextKey].key.posV - m_aInfo[m_nType].aKeyInfo[m_nKey].key.posV;
	D3DXVECTOR3 DiffRot = m_aInfo[m_nType].aKeyInfo[m_nNextKey].key.rot - m_aInfo[m_nType].aKeyInfo[m_nKey].key.rot;

	// 割合を求める
	float fRateFrame = m_nCounter / static_cast<float>(m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);

	// 視点の補間処理
	posV.x = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posV.x + DiffV.x * fRateFrame;
	posV.y = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posV.y + DiffV.y * fRateFrame;
	posV.z = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posV.z + DiffV.z * fRateFrame;

	// 注視点の補間処理
	posR.x = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posR.x + DiffR.x * fRateFrame;
	posR.y = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posR.y + DiffR.y * fRateFrame;
	posR.z = m_aInfo[m_nType].aKeyInfo[m_nKey].key.posR.z + DiffR.z * fRateFrame;	

	math::NormalizeRot(&DiffRot.x);
	math::NormalizeRot(&DiffRot.y);
	math::NormalizeRot(&DiffRot.z);

	// 向きの補間処理
	rot.x = m_aInfo[m_nType].aKeyInfo[m_nKey].key.rot.x + DiffRot.x * fRateFrame;
	rot.y = m_aInfo[m_nType].aKeyInfo[m_nKey].key.rot.y + DiffRot.y * fRateFrame;
	rot.z = m_aInfo[m_nType].aKeyInfo[m_nKey].key.rot.z + DiffRot.z * fRateFrame;

	// カウンターがフレームを超えたら
	if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
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
	using namespace AnimationConstants;

	// ファイルをロードする
	std::ifstream file(pFilePath);

	// 開けなかったら
	if (!file.is_open())
	{
		MessageBox(NULL, "ファイルが開けません", pFilePath, MB_OK);
		return E_FAIL;
	}

	json loadinfo;

	// 情報を読み込む
	file >> loadinfo;

	// ファイルを閉じる
	file.close();
	file.clear();

	// あらかじめメモリを確保する
	m_aInfo.reserve(RESERVE);

	for (auto& cameraSet : loadinfo[AnimationConstants::KEY_CAMERA_SET])
	{
		CCameraAnimationSystem::Info motionInfo = {};

		// 情報の取得
		motionInfo.bLoop = cameraSet["Loop"];

		int nCntKey = 0;

		// キーの情報分回す
		for (auto& keyInfoJson : cameraSet[AnimationConstants::KEY_KEY_INFO])
		{
			KeyInfo keyinfo;

			keyinfo.nFrame = keyInfoJson["frame"];

			// 視点の取得
			keyinfo.key.posV = JsonManager::LoadVector3(keyInfoJson["posV"]);

			// 注視点の取得
			keyinfo.key.posR = JsonManager::LoadVector3(keyInfoJson["posR"]);

			// 回転の取得
			keyinfo.key.rot = JsonManager::LoadVector3(keyInfoJson["rot"]);

			// 要素の追加
			motionInfo.aKeyInfo.push_back(keyinfo);

			nCntKey++;
		}

		motionInfo.nNumKey = nCntKey;

		m_aInfo.push_back(std::move(motionInfo));
	}

	return S_OK;
}
