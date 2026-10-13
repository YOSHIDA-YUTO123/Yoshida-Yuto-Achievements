//===================================================
//
// 近いギミックにスポットライトを当てる処理 [functionGimickSpotLight.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "functionGimickSpotLight.h"
#include "CollisionMapObjManager.h"
#include "CollisionMapObject.h"
#include "player.h"
#include "game.h"
#include "light.h"
#include "manager.h"
#include "RotationFloorManager.h"
#include "RotationFloor.h"

//***************************************************
// 定数宣言
//***************************************************
namespace GimickToLight_Const
{
	constexpr float LIGHT_HEIGHT = 100.0f;			// ライトの高さ
	constexpr float LIGHTING_DISTANCE = 400.0f;		// ライトを当てることができる距離
	constexpr float LIGHT_RANGE = 120.0f;			// ライトの範囲
	constexpr int NUM_NEAR_GIMICK = 5;				// 近くのギミックの数
}

//===================================================
// コンストラクタ
//===================================================
CFunctionGimickSpotLight::CFunctionGimickSpotLight(CCollisionMapObjManager* pMapObjectManager) : 
	m_pMapObjectManagerRef(pMapObjectManager)
{
}

//===================================================
// デストラクタ
//===================================================
CFunctionGimickSpotLight::~CFunctionGimickSpotLight()
{

}

//===================================================
// 更新処理
//===================================================
void CFunctionGimickSpotLight::Update(void)
{
	// 名前空間の使用
	using namespace GimickToLight_Const;

	// プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// 取得できなかったら処理しない
	if (pPlayer == nullptr) return;

	// リストの取得
	auto& vMapObj = m_pMapObjectManagerRef->GetList();

	// ライトの取得
	CLight* pLight = CManager::GetLight();

	// 近いオブジェクトの情報
	struct nearObj
	{
		CCollisionMapObject* pMapObj = nullptr;
		float fMinDistance = FLT_MAX;
	};

	// 近いオブジェクトを格納する配列
	std::array<nearObj, NUM_NEAR_GIMICK> aNearObjects;

	// マップオブジェクト分回す
	for (auto& mapObj : vMapObj)
	{
		nearObj nearObject;

		// nullだったら処理を飛ばす
		if (mapObj == nullptr) continue;

		// ギミックじゃないなら処理を飛ばす
		if (!CheckGimickObj(mapObj)) continue;

		// ライトのIDの取得
		int nLightID = mapObj->GetLightID();

		if (nLightID != -1)
		{
			// ライトをリセット
			pLight->DeleteLight(nLightID);
		}

		// もうすでに乗っていたら
		if (mapObj->GetIsRanding()) continue;

		// 位置の取得
		D3DXVECTOR3 mapObjPos = mapObj->GetCollision()->GetTransform()->GetInfo().pos;

		// 距離の取得
		float fDistance = pPlayer->GetDistance(mapObjPos);

		// 距離を保存
		nearObject.fMinDistance = fDistance;
		nearObject.pMapObj = mapObj;
		
		// 配列の最後よりも近い距離だったら
		if (nearObject.fMinDistance < aNearObjects.back().fMinDistance)
		{
			// 更新
			aNearObjects.back().fMinDistance = nearObject.fMinDistance;
			aNearObjects.back().pMapObj = nearObject.pMapObj;

			// 配列を近い順に並び替える
			std::sort(aNearObjects.begin(), aNearObjects.end(),
				[](const nearObj& objA, const nearObj& objB)
				{
					// 近いなら
					return objA.fMinDistance < objB.fMinDistance;
				});
		}
	}

	// 回る床のライトの設定処理
	SetRotationFloorLighting(pPlayer, pLight);

	// 近いオブジェクト分回す
	for (auto& itr : aNearObjects)
	{
		if (itr.pMapObj == nullptr) continue;

		// ライトを当てられる距離より遠くだったら
		if (itr.fMinDistance >= LIGHTING_DISTANCE) continue;

		// 位置の取得
		D3DXVECTOR3 pos = itr.pMapObj->GetCollision()->GetTransform()->GetInfo().pos;

		// ライトのIDの取得
		int nLightID = pLight->SetPoint(D3DXVECTOR3(pos.x, pos.y + LIGHT_HEIGHT, pos.z), LIGHT_RANGE, Const::WHITE, Const::WHITE);

		// ライトのIDの設定
		itr.pMapObj->SetLightID(nLightID);
	}
}

//===================================================
// ギミックのオブジェクトかどうか判定
//===================================================
bool CFunctionGimickSpotLight::CheckGimickObj(CCollisionMapObject* pMapObj)
{
	// ドラム缶かどうか判定
	const bool bCheckDrumCan = pMapObj->GetXFilePath() == CPlayer::FILEPATH_DRUMMCAN;

	// ボールかどうか判定
	const bool bCheckBall = pMapObj->GetXFilePath() == CPlayer::FILEPATH_BALL;

	// ロープかどうか判定
	const bool bCheckRope = pMapObj->GetXFilePath() == CPlayer::FILEPATH_ROPE;

	// 平均台かどうか判定
	const bool bCheckBalanceBeam = pMapObj->GetXFilePath() == CPlayer::FILEPATH_BALANCEBEAM;

	// 棒かどうか判定
	const bool bCheckRod = pMapObj->GetXFilePath() == CPlayer::FILEPATH_ROD;

	// 棒かどうか判定
	const bool bCheckGoal = pMapObj->GetXFilePath() == CPlayer::FILEPATH_GOAL;

	// ギミックのオブジェクトか判定する
	const bool bCheckGimickObj = (bCheckDrumCan || bCheckBall || bCheckRope || bCheckBalanceBeam || bCheckRod || bCheckGoal);

	// ギミックなら
	if (bCheckGimickObj)
	{
		return true;
	}

	return false;
}

//===================================================
// 回る床のライトの設定処理
//===================================================
void CFunctionGimickSpotLight::SetRotationFloorLighting(CPlayer* pPlayer, CLight* pLight)
{
	// 名前空間の使用
	using namespace GimickToLight_Const;

	// 最小の距離
	float fDistanceMin = FLT_MAX;

	// 近い回る床
	CRotationFloor* pRotationFloor = nullptr;

	// 回る床
	for (auto& floor : CGame::GetRotationFloorManager()->GetList())
	{
		// nullなら処理しない
		if (floor == nullptr) continue;

		// ライトのIDの取得
		int nLightID = floor->GetLightID();

		if (nLightID != -1)
		{
			// ライトをリセット
			pLight->DeleteLight(nLightID);
		}

		// すでに乗っているなら処理を飛ばす
		if (floor->GetIsRanding()) continue;

		// 位置の取得
		D3DXVECTOR3 floorPos = floor->GetPosition();

		// 距離の取得
		float fDistance = pPlayer->GetDistance(floorPos);

		// 最小の距離よりも近いなら
		if (fDistance < fDistanceMin)
		{
			fDistanceMin = fDistance;
			pRotationFloor = floor;
		}
	}

	if (pRotationFloor != nullptr)
	{
		// 位置の取得
		D3DXVECTOR3 pos = pRotationFloor->GetPosition();

		// ライトを当てられる距離より遠くだったら
		if (fDistanceMin >= LIGHTING_DISTANCE) return;

		// ライトのIDの取得
		int nLightID = pLight->SetPoint(D3DXVECTOR3(pos.x, pos.y + LIGHT_HEIGHT, pos.z), LIGHT_RANGE, Const::WHITE, Const::WHITE);

		// ライトのIDの設定
		pRotationFloor->SetLightID(nLightID);
	}
}
