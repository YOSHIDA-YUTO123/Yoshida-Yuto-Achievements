//===================================================
//
// リザルトの更新処理 [result_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "update_ball_system.h"
#include "result_ball_component.hpp"
#include "mesh_wall_collision_component.hpp"
#include "factory_2d.h"
#include "score_component.hpp"
#include "result_system_component.hpp"
#include "factory_mesh.h"
#include "entity_name_component.hpp"
#include "wall_constants.hpp"
#include "input.h"
#include "manager.h"
#include "ui_wall_controller.h"
#include "ui_slide_component.hpp"
#include "scene_controller.h"
#include "scene.h"
#include "title_state.h"
#include "camera_animation_system.h"
#include "ball_constants.hpp"
#include "tag_component.hpp"
#include "manager.h"
#include "sound.h"
#include "result_system.h"
#include "texture_mrt_manager.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ResultSystem_Const
{
	const D3DXVECTOR2 SCORE_POS				= { 1000.0f,380.0f };			// スコアの位置
	const D3DXVECTOR2 SCORE_SIZE			= { 400.0f,100.0f };			// スコアの大きさ
	const D3DXVECTOR3 BALL_POS				= { 0.0f,200.0f,0.0f };			// リザルトのボールの位置
	const D3DXVECTOR3 BALL_MOVE				= { 0.0f,0.0f,20.0f };			// リザルトのボールの移動量
	const D3DXVECTOR2 SCORE_DEST_POSOUT		= { 2000.0f,360.0f };			// スコアのスライドアウトの目的の位置
	constexpr float BALL_RADIUS				= 20.0f;						// リザルトのボールの半径
	constexpr int BALL_SEGMENT				= 16;							// リザルトのボールの分割数				
	constexpr int BALL_LIFE					= 180;							// リザルトのボールの使用時間			
	constexpr int SCORE_SEGMENT_U			= 9;							// スコアのテクスチャの横の分割数
	constexpr int SCORE_SEGMENT_V			= 1;							// スコアのテクスチャの縦の分割数
	constexpr int SCORE_SLIDE_TIME			= 180;							// スコアのスライド時間
	constexpr int CREATE_BALL_TIME			= 50;							// ボールの生成時間
	constexpr const char* SCORE_TEXTURE		= "data/TEXTURE/number001.png";	// テクスチャのパス
	constexpr int SCENE_CHANGE_TIME			= 120;							// シーンの変更の時間
}

//===================================================
// 更新処理
//===================================================
void ResultSystem::Update(entt::registry& registry)
{
	// 定数の名前空間の使用
	using namespace ResultSystem_Const;

	// リザルトの処理のIDの取得
	auto resultSystemID = registry.view<ResultSystemComponent>().front();

	// 使用できないなら処理しない
	if (!registry.valid(resultSystemID))
	{
		return;
	}
	// スコアの取得
	auto score_ID = registry.view<Tag::ResultScoreTag>().front();

	// 使用できないなら処理しない
	if (!registry.valid(score_ID))
	{
		return;
	}

	// リザルトの処理の取得
	auto& resultSystemComp = registry.get<ResultSystemComponent>(resultSystemID);

	// スコアの取得
	auto& scoreComp = registry.get<ScoreComponent>(score_ID);

	// リザルトのボールの取得
	auto ball_view = registry.view<ResultBallComponent>();

	// スコアの桁数が最大まで行ったら
	if (resultSystemComp.nDigit >= scoreComp.MAX_DIGIT)
	{
		// スコアの生成が終わった
		resultSystemComp.bFinishCreateScore = true;
	}

	// リザルトの状態が次に進むときの処理
	AdvanceResult(registry, resultSystemComp, scoreComp);

	// ボールの発射
	ShotBall(registry, resultSystemComp, scoreComp);

	// 取得した分回す
	for (auto ball : ball_view)
	{
		// スコアの生成処理
		CreateScore(registry, ball, scoreComp, resultSystemComp);
	}
}

//===================================================
// スコアの生成
//===================================================
void ResultSystem::CreateScore(entt::registry& registry, const entt::entity ballID, ScoreComponent& scoreComp, ResultSystemComponent& resultSystemComp)
{
	// 定数の名前空間の使用
	using namespace ResultSystem_Const;

	// コンポーネントの取得
	auto& wallCollisionComp = registry.get<MeshWallCollisionComponent>(ballID);
	auto& resultBallComp = registry.get<ResultBallComponent>(ballID);

	// 使用できないなら処理しない
	if (!registry.valid(wallCollisionComp.meshID))
	{
		return;
	}

	// 名前のコンポーネントの取得
	auto& entityNameComp = registry.get<EntityNameComponent>(wallCollisionComp.meshID);
	
	// 正面の壁じゃないなら
	if (entityNameComp.name != WallConst::FRONT_WALL)
	{
		return;
	}

	// ボールが壁に当たったら
	if (wallCollisionComp.bCollision && !resultBallComp.bWallHit)
	{
		// スプライトの生成処理
		CreateSpriteSheet(registry, scoreComp, resultSystemComp);

		resultBallComp.bWallHit = true;
	}
}

//===================================================
// ボールの生成処理
//===================================================
void ResultSystem::ShotBall(entt::registry& registry, ResultSystemComponent& resultSystemComp, ScoreComponent& scoreComp)
{
	// 定数の名前空間の使用
	using namespace ResultSystem_Const;

	// タイマーを減らす
	resultSystemComp.nBallCreateTime--;

	// 桁数がオーバーしていなくて生成時間になったら
	if (resultSystemComp.nBallCreateTime <= 0 && resultSystemComp.nDigit < scoreComp.MAX_DIGIT)
	{
		// ボールを生成
		auto ballID = FactoryMesh::Create::ResultBall(registry, BALL_POS, BALL_MOVE, BALL_RADIUS, BallConstants::TEXTURE_PATH, BALL_SEGMENT, BALL_LIFE);
		FactoryMesh::Build::Ball(registry, ballID);

		// タイマーを再セット
		resultSystemComp.nBallCreateTime = CREATE_BALL_TIME;
	}
}

//===================================================
// スプライトの生成処理
//===================================================
void ResultSystem::CreateSpriteSheet(entt::registry& registry, ScoreComponent& scoreComp, ResultSystemComponent& resultSystemComp)
{
	// 定数の名前空間の使用
	using namespace ResultSystem_Const;

	// 現在の桁数
	int nDigit = resultSystemComp.nDigit;

	// 範囲オーバーだったら
	if (nDigit >= scoreComp.MAX_DIGIT)
	{
		return;
	}

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// 音の取得
	CSound* pSound = pManager->GetSound();

	if (pSound != nullptr)
	{
		// 音の再生
		pSound->Play(CSound::LABEL_SE_RESULT);
	}

	// 計算用の位置
	D3DXVECTOR2 posWk = SCORE_POS;

	// 横幅の割合を求める
	float fRateWidth = SCORE_SIZE.x / ScoreComponent::MAX_DIGIT;

	// 位置のオフセット
	float fOffsetX = fRateWidth * 2.0f * nDigit;

	// 位置を計算
	posWk.x = SCORE_POS.x - fOffsetX;

	Factory2D::Param param;
	param.pos = posWk;
	param.size = { fRateWidth,SCORE_SIZE.y };
	param.texturePath = SCORE_TEXTURE;

	// スプライトの生成
	scoreComp.aNumberID[nDigit] = Factory2D::Create::SpriteSheetMRT(registry, param, CTextureMRTManager::TYPE_RESULT, SCORE_SEGMENT_U, SCORE_SEGMENT_V);
	registry.emplace<UISlideComponent>(scoreComp.aNumberID[nDigit], posWk, SCORE_DEST_POSOUT, SCORE_SLIDE_TIME, posWk);

	resultSystemComp.nDigit++;
}

//===================================================
// スコアのスライドアウト開始処理
//===================================================
void ResultSystem::BeginScoreSlideOut(entt::registry& registry, ScoreComponent& scoreComp)
{
	// スコア分回す
	for (auto scoreID : scoreComp.aNumberID)
	{
		// 使用できないなら処理を飛ばす
		if (!registry.valid(scoreID))
		{
			continue;
		}

		auto& slideComp = registry.get<UISlideComponent>(scoreID);

		// スライドアウトオン
		slideComp.bOut = true;
	}
}

//===================================================
// ランキングの設定処理
//===================================================
void ResultSystem::SetRanking(entt::registry& registry, ResultSystemComponent& resultSystemComp)
{
	// ランキングの設定が終わっていないなら
	if (!resultSystemComp.bFinishCreateRanking)
	{
		// ランキングの設定
		UIWallController::SetRanking(registry);

		// ランキングを設定した。
		resultSystemComp.bFinishCreateRanking = true;

		return;
	}

	// 定数の名前空間の使用
	using namespace ResultSystem_Const;

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	CScene *pScene = pManager->GetScene();
	CSceneController* pSceneController = pScene->GetController();

	pSceneController->ChangeScene(std::make_shared<CTitleState>(), CCameraAnimationSystem::TYPE_REULT_TO_TITLE, SCENE_CHANGE_TIME);
}

//===================================================
// リザルトの状態が次に進むときの処理
//===================================================
void ResultSystem::AdvanceResult(entt::registry& registry, ResultSystemComponent& resultSystemComp, ScoreComponent& scoreComp)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pJoyPad = pManager->GetInputJoypad();

	// 取得できないなら処理しない
	if (pKeyboard == nullptr || pJoyPad == nullptr)
	{
		return;
	}

	// ボタンを押していなかったら
	if (!pKeyboard->GetTrigger(DIK_RETURN) && !pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
	{
		return;
	}

	// スコアの生成が終わってないなら
	if (!resultSystemComp.bFinishCreateScore)
	{
		// スコアを一気に生成する
		for (int nCnt = resultSystemComp.nDigit; nCnt < scoreComp.MAX_DIGIT; nCnt++)
		{
			// スプライトの生成処理
			CreateSpriteSheet(registry, scoreComp, resultSystemComp);
		}

		// スコアの生成が終わった
		resultSystemComp.bFinishCreateScore = true;
	}
	else
	{
		// ランキングの設定処理
		SetRanking(registry, resultSystemComp);
	}
}