//==============================================
//
// ゲームシーン [game.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _GAME_H_
#define _GAME_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "scene.h"
#include "rulebase.h"
#include "functionBase.h"
#include "Audience.h"

//**********************************************
// 前方宣言
//**********************************************
class CPlayer;					// プレイヤー
class CGoal;					// ゴール
class CScore;					// スコア
class CDualUnitTimer;			// タイマー
class CDramcan;					// ドラム缶
class CRotationFloorManager;	// 回転床のマネージャー
class CCollisionMapObjManager;	// 当たり判定するマップオブジェクトのマネージャー
class CRuleBase;
class CGameManager;				// ゲームマネージャー
class CAudienceManager;
class CObjectX;

//**********************************************
// ゲームシーンクラスの定義
//**********************************************
class CGame : public CScene
{
public:
	// シチュエーション列挙
	enum SITUATION
	{
		SITUATION_NONE = 0,				// 何もない
		SITUATION_START,				// 何もない
		SITUATION_PLAY,					// 何もない

		SITUATION_MAX
	};

	CGame();
	~CGame();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void ReadJsonSystemFile(void);
	static void AddFuncTion(std::unique_ptr<CFunctionBase> function);
	static void DestoroyRule(void);
	static void ChangeRule(CRuleBase* pNewRule);
	static void PlayerStateManagement(void);

	// ゲッター
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CGoal* GetGoal(void) { return m_pGoal; }
	static CScore* GetScore(void) { return m_pScore; }
	static CDualUnitTimer* GetTimer(void){ return m_pTimer; }
	static CRotationFloorManager* GetRotationFloorManager(void) { return m_pRotationFloorManager; }
	static CRuleBase* GetRule(void) { return m_pRule; }
	static int GetRuleResult(void) { if (m_pRule != nullptr) { return m_pRule->GetResult(); }; return CRuleBase::RESULT_NONE; }
	static CCollisionMapObjManager* GetMapObjectManager(void) { return m_pCollMapObjManager.get(); }

	// セッター
	static void SetSituation(int nSitu) { m_nSituation = nSitu; }
	static void SetAnimation(CAudience::ANIMETYPE animetype, int nNum = 0);

	template<class T>
	static void DeleteFuncTion(void)
	{
		for (auto it = m_vpFunction.begin(); it != m_vpFunction.end(); )
		{
			// unique_ptr から生ポインタを取り出して dynamic_cast
			if (auto t = dynamic_cast<T*>(it->get()))
			{
				//(*it)->Uninit();        // 中身を破棄
				m_vpFunction.erase(it); // erase は iterator を入れる
				return;                  // 一つ消したら終了
			}
			else
			{
				++it;                   // 進める
			}
		}
	}

private:
	static CPlayer* m_pPlayer;			// プレイヤー
	static CGoal* m_pGoal;				// ゴール
	static CScore* m_pScore;			// スコア
	static CDualUnitTimer* m_pTimer;	// タイマーへのポインタ
	static CRotationFloorManager* m_pRotationFloorManager;			// 回転床のマネージャーへのポインタ
	static CRuleBase* m_pRule;			// ルール基底クラス
	static int m_nSituation;			//状況管理変数

	static std::vector<std::unique_ptr<CFunctionBase>> m_vpFunction;		// 機能ポインタ

	std::unique_ptr<CGameManager> m_pGameManager;					// ゲームマネージャクラスへのポインタ
	static std::unique_ptr<CCollisionMapObjManager> m_pCollMapObjManager;	// 当たり判定するマップオブジェクトのマネージャー
	D3DXVECTOR3 m_rotCamera;	// カメラの角度
	float m_fCameraDistance;	// カメラの距離
	static std::vector<CAudienceManager*> m_vectorAudience;

	CObjectX* m_pBall;
};
#endif