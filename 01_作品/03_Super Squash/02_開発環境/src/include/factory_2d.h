//===================================================
//
// 2Dのエンティティのファクトリーの処理 [factory_2d.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FACTORY_2D_H_
#define _FACTORY_2D_H_

//***************************************************
// 2Dファクトリー名前空間の定義
//***************************************************
namespace Factory2D
{
	// パラメータ
	struct Param
	{
		D3DXVECTOR2 pos;			// 位置
		D3DXVECTOR2 size;			// 大きさ
		std::string texturePath;	// テクスチャのパス
		int nPriority = 0;			// 描画の優先順位
	};

	// 生成
	namespace Create
	{
		entt::entity Sprite				(entt::registry& registry, const Param& param);
		entt::entity SpriteMRT			(entt::registry& registry, const Param& param,const int nMRTType);
		entt::entity SpriteSheet		(entt::registry& registry, const Param& param, const int nSegmentU, const int nSegmentV);
		entt::entity SpriteSheetMRT		(entt::registry& registry, const Param& param, const int nMRTType, const int nSegmentU, const int nSegmentV);
		entt::entity Timer				(entt::registry& registry, const Param& param, const int nTime);
		entt::entity Menu				(entt::registry& registry, const Param& param, const entt::entity selectMenu, const int nMenuID);
		entt::entity Score				(entt::registry& registry, const Param& param, const int nScore);
		entt::entity ScoreMRTResult		(entt::registry& registry, const int nScore);
		entt::entity ScoreMRTRanking	(entt::registry& registry, const Param& param, const int nScore);
		entt::entity RankingMRT			(entt::registry& registry, const Param& param, const int nScore);
		entt::entity StencilPolygon		(entt::registry& registry);
	}

	namespace Build
	{
		void PauseMenu(entt::registry& registry, const entt::entity entity);
	}
}

#endif