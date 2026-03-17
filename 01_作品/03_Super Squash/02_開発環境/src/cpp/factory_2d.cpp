//===================================================
//
// 2Dのエンティティのファクトリーの処理 [factory_2d.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "factory_2d.h"
#include "texture_manager.h"
#include "manager.h"
#include "transform_component.hpp"
#include "renderer.h"
#include "size_component.hpp"
#include "layer_component.hpp"
#include "mesh_layer.hpp"
#include "texture_id_component.hpp"
#include "tag_component.hpp"
#include "vertex_buffer_component.hpp"
#include "vertex_build.h"
#include "color_component.hpp"
#include "sheet_component.hpp"
#include "init_2d.h"
#include "timer_component.hpp"
#include "shader_manager.h"
#include "texture_mt_component.hpp"
#include "texture_mrt_manager.h"
#include "menu_component.hpp"
#include "ui_scaling_component.hpp"
#include "renderer_component.hpp"
#include "score_component.hpp"
#include "renderer.h"
#include "ui_fadeout_component.hpp"
#include "tutorial_ui_component.hpp"
#include "ranking_component.hpp"
#include "result_loader.h"
#include "ranking_highlight_component.hpp"
#include "ui_flash_component.hpp"
#include "mrt_target_component.hpp"
#include "color_constants.hpp"

//***************************************************
// 定数宣言
//***************************************************
namespace Factory2DConst
{
	constexpr int NUMBER_SEGMENT_U			= 9;		// 数字textureの横の分割数
	constexpr int NUMBER_SEGMENT_V			= 1;		// 数字textureの縦の分割数
	constexpr float STENCIL_ALPHA			= 0.4f;		// ステンシルポリゴンの透明度
	constexpr float PAUSE_ANIM_SPEED		= 0.08f;	// ポーズのアニメーションの速度
	constexpr float PAUSE_ANIM_SCALE		= 0.1f;		// ポーズのアニメーションの拡大率
	constexpr float PAUSE_ANIM_SCALE_COEF	= 0.1f;		// ポーズのアニメーションの拡大係数
	constexpr float PAUSE_FLASH_SPEED		= 0.3f;		// ポーズの点滅の速さ
	constexpr int PAUSE_FLASH_TIME			= 120;		// ポーズの点滅の時間			
}

//===================================================
// スプライトの生成処理
//===================================================
entt::entity Factory2D::Create::Sprite(entt::registry& registry, const Param& param)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

    // entityの生成
    const entt::entity entity = registry.create();

	registry.emplace<Tag::SpriteTag>(entity);
	registry.emplace<RendererTag::SpriteTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform2DComponent>(entity, param.pos);
	registry.emplace<Size2DComponent>(entity, param.size);
	registry.emplace<LayerComponent>(entity, param.nPriority);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<ColorComponent>(entity);
	

	// スプライトの初期化処理
	if (FAILED(Init2D::Sprite(registry, entity)))
	{
		assert(false);
	}

    return entity;
}

//===================================================
// マルチレンダーターゲットのスプライトの生成
//===================================================
entt::entity Factory2D::Create::SpriteMRT(entt::registry& registry, const Param& param, const int nMRTType)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::SpriteTag>(entity);
	registry.emplace<RendererTag::SpriteMRTTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform2DComponent>(entity, param.pos);
	registry.emplace<Size2DComponent>(entity, param.size);
	registry.emplace<LayerComponent>(entity, param.nPriority);
	registry.emplace<ColorComponent>(entity);
	
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<MRTTargetComponent>(entity, nMRTType);

	// スプライトの初期化処理
	if (FAILED(Init2D::Sprite(registry, entity)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// スプライトシートの生成
//===================================================
entt::entity Factory2D::Create::SpriteSheet(entt::registry& registry, const Param& param, const int nSegmentU, const int nSegmentV)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::SpriteSheetTag>(entity);
	registry.emplace<RendererTag::SpriteTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform2DComponent>(entity, param.pos);
	registry.emplace<Size2DComponent>(entity, param.size);
	registry.emplace<LayerComponent>(entity, param.nPriority);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<ColorComponent>(entity);
	registry.emplace<SheetComponent>(entity, nSegmentU, nSegmentV);

	// スプライトの初期化処理
	if (FAILED(Init2D::Sprite(registry, entity, nSegmentU, nSegmentV)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// マルチレンダーターゲットのスプライトシートの生成
//===================================================
entt::entity Factory2D::Create::SpriteSheetMRT(entt::registry& registry, const Param& param, const int nMRTType, const int nSegmentU, const int nSegmentV)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::SpriteSheetTag>(entity);
	registry.emplace<RendererTag::SpriteMRTTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform2DComponent>(entity, param.pos);
	registry.emplace<Size2DComponent>(entity, param.size);
	registry.emplace<LayerComponent>(entity, param.nPriority);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<ColorComponent>(entity);
	registry.emplace<MRTTargetComponent>(entity, nMRTType);
	registry.emplace<SheetComponent>(entity, nSegmentU, nSegmentV);

	// スプライトの初期化処理
	if (FAILED(Init2D::Sprite(registry, entity, nSegmentU, nSegmentV)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// タイマーの生成
//===================================================
entt::entity Factory2D::Create::Timer(entt::registry& registry, const Param& param, const int nTime)
{
	// entityの生成
	const entt::entity entity = registry.create();

	auto& timerComp = registry.emplace<TimerComponent>(entity, nTime);
	
	int nCount = 0;

	// 横幅の割合を求める
	float fRateWidth = param.size.x / TimerComponent::MAX_DIGIT;

	// シートのパラメータ
	Param sheetParam;

	// 計算用の位置
	D3DXVECTOR2 posWk = param.pos;

	// 秒数分回す
	for (auto& second : timerComp.aSecondID)
	{
		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCount;

		// 位置を計算
		posWk.x = param.pos.x - fOffsetX;

		// パラメータの設定
		sheetParam.pos = posWk;
		sheetParam.size = { fRateWidth ,param.size.y };
		sheetParam.texturePath = param.texturePath;
		sheetParam.nPriority = param.nPriority;

		// スプライトシートの生成
		second = SpriteSheetMRT(registry, sheetParam, CTextureMRTManager::TYPE_TIMER, Factory2DConst::NUMBER_SEGMENT_U, Factory2DConst::NUMBER_SEGMENT_V);

		nCount++;
	}

	// 秒数分回す
	for (auto& minute : timerComp.aMinuteID)
	{
		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCount;

		// 位置を計算
		posWk.x = param.pos.x - fOffsetX;

		// パラメータの設定
		sheetParam.pos = posWk;
		sheetParam.size = { fRateWidth ,param.size.y };
		sheetParam.texturePath = param.texturePath;
		sheetParam.nPriority = param.nPriority;

		// スプライトシートの生成
		minute = SpriteSheetMRT(registry, sheetParam, CTextureMRTManager::TYPE_TIMER, Factory2DConst::NUMBER_SEGMENT_U, Factory2DConst::NUMBER_SEGMENT_V);

		nCount++;
	}

	return entity;
}

//===================================================
// タイトルのメニュー
//===================================================
entt::entity Factory2D::Create::Menu(entt::registry& registry, const Param& param, const entt::entity selectMenu, const int nMenuID)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::SpriteTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Transform2DComponent>(entity, param.pos);
	registry.emplace<Size2DComponent>(entity, param.size);
	registry.emplace<LayerComponent>(entity, param.nPriority);
	registry.emplace<TextureIDComponent>(entity, nTextureID);
	registry.emplace<ColorComponent>(entity);
	registry.emplace<RendererTag::SpriteTag>(entity);
	registry.emplace<MenuComponent>(entity, selectMenu, nMenuID);

	// スプライトの初期化処理
	if (FAILED(Init2D::Sprite(registry, entity)))
	{
		assert(false);
	}

	return entity;
}

//===================================================
// スコアの生成
//===================================================
entt::entity Factory2D::Create::Score(entt::registry& registry, const Param& param, const int nScore)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();
	

	// スコアの生成
	auto& scoreComp = registry.emplace<ScoreComponent>(entity, nScore);

	// 横幅の割合を求める
	float fRateWidth = param.size.x / ScoreComponent::MAX_DIGIT;

	int nCnt = 0;

	// 計算用の位置
	D3DXVECTOR2 posWk = param.pos;

	// シートのパラメータ
	Param sheetParam;

	// シートの数分回す
	for (auto& sheetID : scoreComp.aNumberID)
	{
		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCnt;

		// 位置を計算
		posWk.x = param.pos.x - fOffsetX;

		// パラメータの設定
		sheetParam.pos = posWk;
		sheetParam.size = { fRateWidth ,param.size.y };
		sheetParam.texturePath = param.texturePath;
		sheetParam.nPriority = param.nPriority;

		// シートの生成
		sheetID = Factory2D::Create::SpriteSheet(registry, sheetParam, Factory2DConst::NUMBER_SEGMENT_U, Factory2DConst::NUMBER_SEGMENT_V);
		
		nCnt++;
	}
	return entity;
}

//===================================================
// リザルトのMRTのスコア
//===================================================
entt::entity Factory2D::Create::ScoreMRTResult(entt::registry& registry, const int nScore)
{
	// entityの生成
	const entt::entity entity = registry.create();

	// スコアの生成
	auto& scoreComp = registry.emplace<ScoreComponent>(entity, nScore);
	registry.emplace<Tag::ResultScoreTag>(entity);

	int nScoreWk = nScore;
	int nDigit = 0;

	// 桁数を求める
	do
	{
		nScoreWk /= 10;
		nDigit++;

	} while (nScoreWk != 0);

	// 桁数を求める
	scoreComp.nDigit = nDigit;

	return entity;
}

//===================================================
// ランキングのスコアの生成
//===================================================
entt::entity Factory2D::Create::ScoreMRTRanking(entt::registry& registry, const Param& param, const int nScore)
{
	// テクスチャのマネージャークラスの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	int nTextureID = -1;

	if (pTextureManager != nullptr)
	{
		// テクスチャのIDの設定
		nTextureID = pTextureManager->Register(param.texturePath.c_str());
	}

	// entityの生成
	const entt::entity entity = registry.create();
	
	// スコアの生成
	auto& scoreComp = registry.emplace<ScoreComponent>(entity, nScore);

	// 横幅の割合を求める
	float fRateWidth = param.size.x / ScoreComponent::MAX_DIGIT;

	int nCnt = 0;

	// シートのパラメータ
	Param sheetParam;

	// 計算用の位置
	D3DXVECTOR2 posWk = param.pos;

	// シートの数分回す
	for (auto& sheetID : scoreComp.aNumberID)
	{
		// 位置のオフセット
		float fOffsetX = fRateWidth * 2.0f * nCnt;

		// 位置を計算
		posWk.x = param.pos.x - fOffsetX;

		// パラメータの設定
		sheetParam.pos = posWk;
		sheetParam.size = { fRateWidth ,param.size.y };
		sheetParam.texturePath = param.texturePath;
		sheetParam.nPriority = param.nPriority;

		// シートの生成
		sheetID = Factory2D::Create::SpriteSheetMRT(registry, sheetParam , CTextureMRTManager::TYPE_RANKING, Factory2DConst::NUMBER_SEGMENT_U, Factory2DConst::NUMBER_SEGMENT_V);

		nCnt++;
	}
	return entity;
}

//===================================================
// ランキングの生成処理
//===================================================
entt::entity Factory2D::Create::RankingMRT(entt::registry& registry, const Param& param, const int nScore)
{
	// entityの生成
	const entt::entity entity = registry.create();

	auto& rankingComp = registry.emplace<RankingComponent>(entity);

	std::array<int, RankingComponent::MAX_RANK> ranking;

	// ランキングの取得
	Loader::Result::MinigameRanking(ranking);

	// 順位数の取得
	int nMaxRank = RankingComponent::MAX_RANK;

	// 順位数分回す
	for (int nCnt = 0; nCnt < nMaxRank; nCnt++)
	{
		float fHeight = (param.size.y * 2.0f);
		
		// シートのパラメータ
		Param sheetParam;

		// パラメータの設定
		sheetParam.pos = { param.pos.x,param.pos.y + (nCnt * fHeight) };
		sheetParam.size = param.size;
		sheetParam.texturePath = param.texturePath;

		 // スコアの生成
		rankingComp.aScoreID[nCnt] = Factory2D::Create::ScoreMRTRanking(registry, sheetParam, ranking[nCnt]);

	}

	// 順位数分回す
	for (int nCnt = 0; nCnt < nMaxRank; nCnt++)
	{
		// ランクインしていたら
		if (ranking[nCnt] == nScore)
		{
			// ハイライト表示コンポーネント
			registry.emplace<RankingHighLightComponent>(rankingComp.aScoreID[nCnt]);
			break;
		}
	}
	return entity;
}

//===================================================
// ステンシル用のポリゴンの描画処理
//===================================================
entt::entity Factory2D::Create::StencilPolygon(entt::registry& registry)
{
	// entityの生成
	const entt::entity entity = registry.create();

	registry.emplace<Tag::StencilPolygonTag>(entity);
	registry.emplace<VertexBufferComponent>(entity);
	registry.emplace<Size2DComponent>(entity, D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	registry.emplace<Transform2DComponent>(entity,Const::CENTER_POS_2D);

	// 色の設定
	D3DXCOLOR col = { Color::BLACK.r ,Color::BLACK.g, Color::BLACK.b,Factory2DConst::STENCIL_ALPHA };

	registry.emplace<ColorComponent>(entity, col);

	// 初期化処理
	if (FAILED(Init2D::Sprite(registry, entity)))
	{
		assert(false);
	}
	return entity;
}

//===================================================
// ポーズメニューの構築処理
//===================================================
void Factory2D::Build::PauseMenu(entt::registry& registry, const entt::entity entity)
{
	// 名前空間の使用
	using namespace Factory2DConst;

	auto& sizeComp = registry.get<Size2DComponent>(entity);

	registry.emplace<Tag::PauseMenuTag>(entity);
	registry.emplace<RendererComponent>(entity, RendererComponent::TYPE_NO_DISP);
	registry.emplace<UIScalingComponent>(entity, sizeComp.size, PAUSE_ANIM_SPEED, PAUSE_ANIM_SCALE, PAUSE_ANIM_SCALE_COEF);
	registry.emplace<UIFlashComponent>(entity, PAUSE_FLASH_SPEED, PAUSE_FLASH_TIME);
}