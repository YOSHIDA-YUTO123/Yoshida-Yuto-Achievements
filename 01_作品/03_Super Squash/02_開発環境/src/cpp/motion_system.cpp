//===================================================
//
// モーション更新処理 [motion_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion_system.h"
#include "motion_component.hpp"
#include "character_model_component.hpp"
#include "parent_component.hpp"
#include "utility_math.h"
#include "transform_component.hpp"
#include "debug_proc.h"
#include "game_controller.h"
#include "hitstop_controller.h"
#include "tag_component.hpp"

//===================================================
// 更新処理
//===================================================
void MotionSystem::Update(entt::registry& registry)
{
	// モーションのコンポーネントをもつentityの取得
	auto view = registry.view<MotionComponent, ChildrenComponent>();

	// entity分調べる
	for (auto [entity, motionComp, childens] : view.each())
	{
		// ゲームを操作するクラスの取得
		auto pGameController = CGameController::GetInstance();

		// ヒットストップしているときにHitStop対象のentityだったら
		if (pGameController != nullptr && registry.any_of<Tag::HitStopTag>(entity))
		{
			// ヒットストップしているなら処理を飛ばす
			if (pGameController->GetHitStop()->IsStop())
			{
				return;
			}
		}

		// 現在のモーションのデータのアドレスの取得
		MotionComponent::CurrentInfo *pCurrent = &motionComp.current;

		// ブレンドのモーションのデータのアドレスの取得
		MotionComponent::BlendInfo* pBlend = &motionComp.blend;

		// キーの総数の取得
		pCurrent->nNumKey = motionComp.aInfo[pCurrent->nType].nNumkey;
		pBlend->nNumKeyBlend = motionComp.aInfo[pBlend->nTypeBlend].nNumkey;

		// ループするかどうかの取得
		pCurrent->bLoopMotion = motionComp.aInfo[pCurrent->nType].bLoop;

		// キーが無いなら処理しない
		if (motionComp.aInfo[pCurrent->nType].nNumkey == 0 || pBlend->nNumKeyBlend == 0) return;

		// 次のキーを増やす
		pCurrent->nNextKey = (pCurrent->nKey + 1) % motionComp.aInfo[pCurrent->nType].nNumkey;

		// 次のブレンドキーを増やす
		pBlend->nNextKeyBlend = (pBlend->nKeyBlend + 1) % pBlend->nNumKeyBlend;

		// モデルの総数分回す
		for (int nCntModel = 0; nCntModel < motionComp.nNumModel; nCntModel++)
		{
			if (pBlend->bFinish == false && pBlend->bFirst == false)
			{
				// 現在のモーションの更新処理
				UpdateCurrent(registry,childens.aChildrenID[nCntModel], nCntModel, motionComp);
			}
			if ((pBlend->bFinish == true || pBlend->bFirst == true) && pBlend->bBlend == true)
			{
				// ブレンドのモーションの更新処理
				UpdateBlend(registry, childens.aChildrenID[nCntModel], nCntModel, motionComp);
			}
		}

		// モーションが終了したら
		if (IsEnd(motionComp))
		{
			// ブレンドのフレームを計算
			int nBlendFrame = motionComp.aInfo[pCurrent->nType].aKeyInfo[pCurrent->nNumKey - 1].nFrame;

			pBlend->nCounterMotionBlend = 0;		// ブレンドモーションのカウンターをリセット
			pBlend->nKeyBlend = 0;				// ブレンドキーをリセット
			pBlend->nCounterBlend = 0;			// ブレンドフレームをリセット
			pBlend->bFinish = true;				// モーションが終わった
			pBlend->nFrameBlend = nBlendFrame;	// ブレンドフレームを設定
			pBlend->nTypeBlend = pBlend->nEndBlendMotion;	// ニュートラルにブレンドする
		}

		// モーションの出だしのブレンドが終了した
		FinishFirstBlend(motionComp);

		// キーが最大かつブレンドのカウントが最大になった
		if (IsEndBlend(motionComp) == true)
		{
			pBlend->bFinish = false;								// もとに戻す
			pBlend->bBlend = false;									// もとに戻す
			pCurrent->nCount = pBlend->nCounterMotionBlend;			// フレームをブレンドした先のフレームに合わせる
			motionComp.nAllCounter = pBlend->nCounterMotionBlend;	// フレームカウンターを設定
			pCurrent->nType = pBlend->nEndBlendMotion;				// モーションタイプをニュートラルにする
			pBlend->nCounterBlend = 0;
			pCurrent->nKey = pBlend->nKeyBlend;
		}

		// フレームを計算
		int nFrame = motionComp.aInfo[pCurrent->nType].aKeyInfo[pCurrent->nKey].nFrame;

		// フレームが最大フレームを超えたら
		if (pCurrent->nCount >= nFrame)
		{
			// キーを増やす
			pCurrent->nKey++;

			// 範囲内にラップする
			pCurrent->nKey = math::Wrap(pCurrent->nKey, 0, motionComp.aInfo[pCurrent->nType].nNumkey - 1);

			// カウンターをリセット
			pCurrent->nCount = 0;
		}

		// ブレンドフレームを計算
		nFrame = motionComp.aInfo[pBlend->nTypeBlend].aKeyInfo[pBlend->nKeyBlend].nFrame;

		// ブレンドキーを進める
		if (pBlend->nCounterMotionBlend >= nFrame && (pBlend->bFinish == true || pBlend->bFirst == true))
		{
			// キーを増やす
			pBlend->nKeyBlend++;

			// 範囲内にラップする
			pBlend->nKeyBlend = math::Wrap(pBlend->nKeyBlend, 0, motionComp.aInfo[pBlend->nTypeBlend].nNumkey - 1);

			pBlend->nCounterMotionBlend = 0;
		}

		// 最初のブレンドじゃないなら
		if (pBlend->bFirst == false)
		{
			motionComp.nAllCounter++;
			pCurrent->nCount++;
		}

		// ブレンドが始まったら
		if (pBlend->bFinish == true || pBlend->bFirst == true)
		{
			pBlend->nCounterBlend++;
			pBlend->nCounterMotionBlend++;
			motionComp.nAllCounter++;
		}

		// 計算用ALLフレーム
		int nAllFrame = 0;

		for (int nCnt = 0; nCnt < motionComp.aInfo[pCurrent->nType].nNumkey; nCnt++)
		{
			nAllFrame += motionComp.aInfo[pCurrent->nType].aKeyInfo[nCnt].nFrame;
		}

		motionComp.nAllFrame = nAllFrame;

		// ブレンドフレームを計算
		nFrame = motionComp.nAllFrame;

		// 最大を超えたら
		if (motionComp.nAllCounter >= nFrame)
		{
			motionComp.nAllCounter = 0;
		}
	}
}

//===================================================
// 現在のモーションの更新処理
//===================================================
void MotionSystem::UpdateCurrent(entt::registry& registry, const entt::entity model, const int nCntModel, MotionComponent& motionComp)
{
	// 現在のモーションのデータのアドレスの取得
	MotionComponent::CurrentInfo* pCurrent = &motionComp.current;

	int nType = pCurrent->nType;		// 現在のモーションの種類
	int nKey = pCurrent->nKey;			// 現在のキー
	int nNextKey = pCurrent->nNextKey;	// 次のキー

	// 次のキーのアドレスの取得
	MotionComponent::Key* pKeyNext = &motionComp.aInfo[nType].aKeyInfo[nNextKey].aKey[nCntModel];

	// 現在のキーのアドレスの取得
	MotionComponent::Key* pKey = &motionComp.aInfo[nType].aKeyInfo[nKey].aKey[nCntModel];

	// キー情報のアドレスの取得
	MotionComponent::KeyInfo* pKeyInfo = &motionComp.aInfo[nType].aKeyInfo[nKey];

	// 次のキーまでの距離を求める
	float fDiffPosX = pKeyNext->fPosX - pKey->fPosX;
	float fDiffPosY = pKeyNext->fPosY - pKey->fPosY;
	float fDiffPosZ = pKeyNext->fPosZ - pKey->fPosZ;

	// 次のキーまでの距離を求める
	float fDiffRotX = pKeyNext->fRotX - pKey->fRotX;
	float fDiffRotY = pKeyNext->fRotY - pKey->fRotY;
	float fDiffRotZ = pKeyNext->fRotZ - pKey->fRotZ;
	
	float fFrame = (float)pKeyInfo->nFrame;

	// フレームと最大フレームの割合を求める
	float fRateFrame = pCurrent->nCount / fFrame;

	// 次のキーの位置までフレームに応じた位置を設定する
	float fPosX = math::LerpDiff(pKey->fPosX, fDiffPosX, fRateFrame);
	float fPosY = math::LerpDiff(pKey->fPosY, fDiffPosY, fRateFrame);
	float fPosZ = math::LerpDiff(pKey->fPosZ, fDiffPosZ, fRateFrame);

	// 次のキーの向きまでフレームに応じた向きを設定する
	float fRotX = math::LerpDiff(pKey->fRotX, fDiffRotX, fRateFrame);
	float fRotY = math::LerpDiff(pKey->fRotY, fDiffRotY, fRateFrame);
	float fRotZ = math::LerpDiff(pKey->fRotZ, fDiffRotZ, fRateFrame);

	// モデルのコンポーネントの取得
	auto& transform = registry.get<Transform3DComponent>(model);
	auto& parentComp = registry.get<ParentComponent>(model);

	// 位置の設定
	transform.pos = parentComp.offSetPos + D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// クォータニオンの作成
	D3DXQuaternionRotationYawPitchRoll(&transform.quaternion, fRotY, fRotX, fRotZ);

	// クォータニオンを掛け合わせる
	D3DXQuaternionMultiply(&transform.quaternion, &parentComp.offSetQuaternion, &transform.quaternion);
}

//===================================================
// ブレンドモーションの更新処理
//===================================================
void MotionSystem::UpdateBlend(entt::registry& registry, const entt::entity model, const int nCntModel, MotionComponent& motionComp)
{
	// 現在のモーションのデータのアドレスの取得
	MotionComponent::CurrentInfo* pCurrent = &motionComp.current;

	// ブレンドのモーションのデータのアドレスの取得
	MotionComponent::BlendInfo* pBlend = &motionComp.blend;

	// モーションの種類の取得
	int nType = pCurrent->nType;
	int nBlendType = pBlend->nTypeBlend;

	// キーの取得
	int nKey = pCurrent->nKey;
	int nNextKey = pCurrent->nNextKey;
	int nBlendKey = pBlend->nKeyBlend;
	int nBlendNextKey = pBlend->nNextKeyBlend;

	// 現在のキーのアドレスを取得
	MotionComponent::Key* pCurrentKey = &motionComp.aInfo[nType].aKeyInfo[nKey].aKey[nCntModel];

	// 次のキーのアドレスを取得
	MotionComponent::Key* pNextKey = &motionComp.aInfo[nType].aKeyInfo[nNextKey].aKey[nCntModel];

	// キー情報のアドレスの取得
	MotionComponent::KeyInfo* pKeyInfo = &motionComp.aInfo[nType].aKeyInfo[nKey];

	// ブレンドのキーのアドレスを取得
	MotionComponent::Key* pCurrentBlendKey = &motionComp.aInfo[nBlendType].aKeyInfo[nBlendKey].aKey[nCntModel];

	// ブレンドの次のキーのアドレスを取得
	MotionComponent::Key* pNextBlendKey = &motionComp.aInfo[nBlendType].aKeyInfo[nBlendNextKey].aKey[nCntModel];

	// 現在のフレームを計算
	float fCurrentFrame = (float)pKeyInfo->nFrame;

	// ブレンドフレームを計算
	float fBlendFrame = (float)motionComp.aInfo[nBlendType].aKeyInfo[nBlendNextKey].nFrame;

	float fRateMotion = pCurrent->nCount / fCurrentFrame; // 相対値

	float fRateMotionBlend = pBlend->nCounterMotionBlend / fBlendFrame;
	float fRateBlend = pBlend->nCounterBlend / (float)pBlend->nFrameBlend;

	// 現在のモーションの角度の差分
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// 現在のモーションのフレームに応じた角度を設定
	float fRotXCurrent = math::LerpDiff(pCurrentKey->fRotX, fDiffMotionRX, fRateMotion);
	float fRotYCurrent = math::LerpDiff(pCurrentKey->fRotY, fDiffMotionRY, fRateMotion);
	float fRotZCurrent = math::LerpDiff(pCurrentKey->fRotZ, fDiffMotionRZ, fRateMotion);

	//ブレンドモーションの角度の差分を求める
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	// ブレンドモーションのフレームに応じた角度の設定
	float fRotXBlend = math::LerpDiff(pCurrentBlendKey->fRotX, fDiffMotionBlendRX, fRateMotionBlend);
	float fRotYBlend = math::LerpDiff(pCurrentBlendKey->fRotY, fDiffMotionBlendRY, fRateMotionBlend);
	float fRotZBlend = math::LerpDiff(pCurrentBlendKey->fRotZ, fDiffMotionBlendRZ, fRateMotionBlend);

	//現在のモーションの位置の差分を求める
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// 現在のモーションのフレームに応じた位置の設定
	float fPosXCurrent = math::LerpDiff(pCurrentKey->fPosX, fDiffMotionPX, fRateMotion);
	float fPosYCurrent = math::LerpDiff(pCurrentKey->fPosY, fDiffMotionPY, fRateMotion);
	float fPosZCurrent = math::LerpDiff(pCurrentKey->fPosZ, fDiffMotionPZ, fRateMotion);

	// ブレンドモーションの位置の差分を求める
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	// ブレンドモーションのフレームに応じた位置の設定
	float fPosXBlend = math::LerpDiff(pCurrentBlendKey->fPosX, fDiffMotionBlendPX, fRateMotionBlend);
	float fPosYBlend = math::LerpDiff(pCurrentBlendKey->fPosY, fDiffMotionBlendPY, fRateMotionBlend);
	float fPosZBlend = math::LerpDiff(pCurrentBlendKey->fPosZ, fDiffMotionBlendPZ, fRateMotionBlend);

	// 現在の角度とブレンドする角度の差分を求める
	float fDiffBlendRX = fRotXBlend - fRotXCurrent;
	float fDiffBlendRY = fRotYBlend - fRotYCurrent;
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent;

	// 角度の正規化
	math::NormalizeRot(&fDiffBlendRX);
	math::NormalizeRot(&fDiffBlendRY);
	math::NormalizeRot(&fDiffBlendRZ);

	// ブレンドのフレームに応じた向きの設定
	float fRotX = math::LerpDiff(fRotXCurrent, fDiffBlendRX, fRateBlend);
	float fRotY = math::LerpDiff(fRotYCurrent, fDiffBlendRY, fRateBlend);
	float fRotZ = math::LerpDiff(fRotZCurrent, fDiffBlendRZ, fRateBlend);

	// 現在の位置とブレンドする位置の差分を求める
	float fDiffBlendPX = fPosXBlend - fPosXCurrent;
	float fDiffBlendPY = fPosYBlend - fPosYCurrent;
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent;

	// ブレンドのフレームに応じた位置の設定
	float fPosX = math::LerpDiff(fPosXCurrent, fDiffBlendPX, fRateBlend);
	float fPosY = math::LerpDiff(fPosYCurrent, fDiffBlendPY, fRateBlend);
	float fPosZ = math::LerpDiff(fPosZCurrent, fDiffBlendPZ, fRateBlend);

	// モデルのコンポーネントの取得
	auto& transform = registry.get<Transform3DComponent>(model);
	auto& parentComp = registry.get<ParentComponent>(model);

	// 位置の設定
	transform.pos = parentComp.offSetPos + D3DXVECTOR3(fPosX, fPosY, fPosZ);

	// クォータニオンの作成
	D3DXQuaternionRotationYawPitchRoll(&transform.quaternion, fRotY, fRotX, fRotZ);

	// クォータニオンを掛け合わせる
	D3DXQuaternionMultiply(&transform.quaternion, &parentComp.offSetQuaternion, &transform.quaternion);
}

//===================================================
// モーションが終わったかどうか
//===================================================
bool MotionSystem::IsEnd(MotionComponent& motionComp)
{
	// 現在のモーションのデータのアドレスの取得
	MotionComponent::CurrentInfo* pCurrent = &motionComp.current;

	// ブレンドのモーションのデータのアドレスの取得
	MotionComponent::BlendInfo* pBlend = &motionComp.blend;

	// モーションが終了したら
	if (pBlend->bFinish == false &&
		pCurrent->nKey >= pCurrent->nNumKey - 1 &&
		pCurrent->bLoopMotion == false &&
		pBlend->bFirst == false)
	{
		pBlend->bBlend = true;
		return true;
	}
	return false;
}

//===================================================
// ブレンドモーションが終了したらどうか
//===================================================
bool MotionSystem::IsEndBlend(MotionComponent& motionComp)
{
	// ブレンドのモーションのデータのアドレスの取得
	MotionComponent::BlendInfo* pBlend = &motionComp.blend;

	// ブレンドが終わったら
	if (pBlend->bFinish == true && pBlend->nFrameBlend <= pBlend->nCounterBlend && pBlend->bFirst == false)
	{
		return true;
	}

	return false;
}

//===================================================
// 最初のブレンドの処理
//===================================================
void MotionSystem::FinishFirstBlend(MotionComponent& motionComp)
{
	// 現在のモーションのデータのアドレスの取得
	MotionComponent::CurrentInfo* pCurrent = &motionComp.current;

	// ブレンドのモーションのデータのアドレスの取得
	MotionComponent::BlendInfo* pBlend = &motionComp.blend;

	// モーションの出だしのブレンドが終了した
	if (pBlend->bFirst == true && pBlend->nCounterBlend >= pBlend->nFrameBlend && pBlend->bFinish == false)
	{
		// もとに戻す
		pBlend->bFirst = false;

		pCurrent->nKey = pBlend->nKeyBlend;

		pBlend->nKeyBlend = 0;

		// モーションをブレンドしたモーションにする
		pCurrent->nType = pBlend->nTypeBlend;

		pCurrent->nCount = pBlend->nCounterMotionBlend;
		pBlend->nCounterBlend = 0;
	}
}
