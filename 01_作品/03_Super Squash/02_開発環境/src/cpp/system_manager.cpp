//===================================================
//
// systemのマネージャーの処理 [system_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "system_manager.h"
#include "renderer_system_base.h"
#include "system_base.h"
#include "renderer_system_base.h"
#include "algorithm"
#include "layer_component.hpp"
#include "billboard_renderer.h"
#include "character_renderer.h"
#include "model_renderer.h"
#include "mesh_renderer.h"
#include "mesh_quad_renderer.h"
#include "sprite_renderer.h"
#include "camera.h"
#include "manager.h"
#include "renderer_manager.h"

//***************************************************
// 静的メンバ変数
//***************************************************

//===================================================
// コンストラクタ
//===================================================
CSystemManager::CSystemManager() : 
	m_apRendererSystems(),
	m_apSystems(),
	m_apUnPauseSystem(),
	m_apPostProcessExcludedRendererSystem(),
	m_apRendererMRTSystem()
{
}

//===================================================
// デストラクタ
//===================================================
CSystemManager::~CSystemManager()
{
}

//===================================================
// 終了処理
//===================================================
void CSystemManager::ReleaseAll(void)
{
	// 要素分調べる
	for (auto& list : m_apSystems)
	{
		// 破棄
		list.reset();
	}

	m_apSystems.clear();

	// 要素分調べる
	for (auto& list : m_apUnPauseSystem)
	{
		// 破棄
		list.reset();
	}

	m_apUnPauseSystem.clear();

	// 要素分調べる
	for (auto& list : m_apPostProcessExcludedRendererSystem)
	{
		// 破棄
		list.reset();
	}

	// 要素分調べる
	for (auto& list : m_apRendererSystems)
	{
		// 破棄
		list.reset();
	}

	m_apRendererSystems.clear();
}

//===================================================
// 更新処理
//===================================================
void CSystemManager::UpdateAll(entt::registry& registry)
{
	// 要素分調べる
	for (auto system = m_apSystems.begin(); system != m_apSystems.end(); system++)
	{
		// nullなら処理を飛ばす
		if ((*system) == nullptr) continue;

		// 更新処理
		(*system)->Update(registry);
	}
}

//===================================================
// ポーズの影響を受けない更新処理
//===================================================
void CSystemManager::UnPauseUpdateAll(entt::registry& registry)
{
	// 要素分調べる
	for (auto system = m_apUnPauseSystem.begin(); system != m_apUnPauseSystem.end(); system++)
	{
		// nullなら処理を飛ばす
		if ((*system) == nullptr) continue;

		// ポーズの影響を受けない更新処理
		(*system)->Update(registry);
	}
}

//===================================================
// 描画処理
//===================================================
void CSystemManager::DrawAll(entt::registry& registry)
{
	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラの描画
	if (pCamera != nullptr)
	{
		pCamera->SetCamera();
	}

	// 要素分調べる
	for (auto renderer = m_apRendererSystems.begin(); renderer != m_apRendererSystems.end(); renderer++)
	{
		// nullなら処理を飛ばす
		if ((*renderer) == nullptr) continue;

		// 描画処理
		(*renderer)->Renderer(registry);
	}
}

//===================================================
// MRT描画処理
//===================================================
void CSystemManager::DrawMRTAll(entt::registry& registry)
{
	// MRTの種類分回す
	for (int nCnt = 0; nCnt < CTextureMRTManager::TYPE_MAX; nCnt++)
	{
		if (nCnt == CTextureMRTManager::TYPE_SCENE)
		{
			continue;
		}
		// レンダーターゲットを変更
		RendererManager::ChangeRenderTarget(nCnt);

		// 要素分調べる
		for (auto rendererMRT = m_apRendererMRTSystem[nCnt].begin(); rendererMRT != m_apRendererMRTSystem[nCnt].end(); rendererMRT++)
		{
			// nullなら処理を飛ばす
			if ((*rendererMRT) == nullptr) continue;

			// 描画処理
			(*rendererMRT)->Renderer(registry, nCnt);
		}

		// レンダーターゲットのリセット
		RendererManager::ResetRenderTarget();
	}
}

//===================================================
// ポストプロセスを適応しない描画処理
//===================================================
void CSystemManager::DrawPostProcessExcluded(entt::registry& registry)
{
	// 要素分調べる
	for (auto renderer = m_apPostProcessExcludedRendererSystem.begin(); renderer != m_apPostProcessExcludedRendererSystem.end(); renderer++)
	{
		// nullなら処理を飛ばす
		if ((*renderer) == nullptr) continue;

		// 描画処理
		(*renderer)->Renderer(registry);
	}
}