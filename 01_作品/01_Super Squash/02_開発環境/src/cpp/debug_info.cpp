//===================================================
//
// デバッグ情報を表示するクラス [debug_info.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "debug_info.h"
#include "debug_proc.h"
#include "scene.h"
#include "manager.h"
#include "tag_component.hpp"
#include "transform_component.hpp"
#include "input.h"
#include "score_component.hpp"

//===================================================
// コンストラクタ
//===================================================
CDebugInfo::CDebugInfo()
{
}

//===================================================
// デストラクタ
//===================================================
CDebugInfo::~CDebugInfo()
{
}

//===================================================
// デバッグ情報の表示
//===================================================
void CDebugInfo::Debug(entt::registry& registry, const int nFps)
{
#ifdef _DEBUG

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();
	CInputMouse* pMouse = pManager->GetInputMouse();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pMouse == nullptr) return;

	// シーンの取得
	CScene* pScene = pManager->GetScene();

	// モードの取得
	CScene::Mode mode = pScene->GetMode();

	CDebugProc::Print("*******************************************************\n");
	CDebugProc::Print("*                    デバッグ情報                     *\n");
	CDebugProc::Print("*******************************************************\n");

	CDebugProc::Print("現在のシーン		: [ %d ]\n", mode);
	CDebugProc::Print("FPS				: [ %d ]\n", nFps);

	static bool bShowCollider = true;

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		bShowCollider = bShowCollider ? false : true;
	}

	const char* pShowColliderMessage = bShowCollider ? "ON" : "OFF";

	CDebugProc::Print("コライダー表示   : [ %s ]\n", pShowColliderMessage);

	if (mode == CScene::Mode::Game)
	{
		CDebugProc::Print("リロード     : [ F8 ]\n");
	}

	//// カメラのコンポーネントの取得
	//auto CameraView = registry.view<CameraComponent>().each();

	//for (auto [entity, CameraComp] : CameraView)
	//{
	//	// カメラのデバッグ情報
	//	CDebugProc::Print("視点の位置   : [ %.2f : %.2f : %.2f ]\n", CameraComp.posV.x, CameraComp.posV.y, CameraComp.posV.z);
	//	CDebugProc::Print("注視点の位置 : [ %.2f : %.2f : %.2f ]\n", CameraComp.posR.x, CameraComp.posR.y, CameraComp.posR.z);
	//	CDebugProc::Print("カメラの回転 : [ %.2f : %.2f : %.2f ]\n", CameraComp.rot.x, CameraComp.rot.y, CameraComp.rot.z);
	//}

	for (auto player : registry.view<Tag::PlayerTag>())
	{
		auto& transform = registry.get<Transform3DComponent>(player);

		CDebugProc::Print("プレイヤーの位置 : X [ %.3f] Y [ %.3f] Z [ %.3f]\n", transform.pos.x, transform.pos.y, transform.pos.z);
	}

	for (auto enemy : registry.view<Tag::EnemyTag>())
	{
		auto& transform = registry.get<Transform3DComponent>(enemy);

		CDebugProc::Print("敵の位置 : X [ %.3f] Y [ %.3f] Z [ %.3f]\n", transform.pos.x, transform.pos.y, transform.pos.z);
	}

	for (auto score : registry.view<ScoreComponent>())
	{
		auto& scoreComp = registry.get<ScoreComponent>(score);

		CDebugProc::Print("スコア = [ %d ]\n", scoreComp.nScore);
	}
#else
	// 使用していない
	(void)registry;
	(void)nFps;
#endif // _DEBUG
}