//===================================================
//
// シーンの基底クラス [scene.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CSceneController; // シーンを制御するクラス
class CStateMachine;	// 状態マシーン
class CSceneState;		// シーンの状態マシーンの親
class CSystemManager;	// システムのマネージャークラス

//***************************************************
// シーンクラスの定義
//***************************************************
class CScene
{
public:

	// モードの種類
	enum class Mode : uint8_t
	{
		Title = 0,
		Game,
		Result,
		Max
	};

	CScene();
	explicit CScene(const Mode mode);
	~CScene();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawMRT(void);
	void UnPauseUpdate(void);
	void DrawPostProcessExcluded(void);

	inline CSystemManager* GetSystemManager(void) { return m_pSystemManager.get(); }
	inline CSceneController* GetController(void) { return m_pController.get(); }
	inline const Mode GetMode(void) const { return m_mode; }

	void ChangeState(std::shared_ptr<CSceneState> pNewState, const int nTime = 0);

	inline void SetFps(const int nFps) { m_nFps = nFps; }
	inline entt::registry& GetRegistry(void) { return m_registry; }

private:
	entt::entity CreateEntity(void);
	void CreateScene(void);
	void SetChangeState(void);
private:
	entt::registry m_registry;		// レジストリー

	std::unique_ptr<CSceneController> m_pController;	// シーンを制御するクラスへのポインタ
	std::unique_ptr<CStateMachine> m_pState;			// 状態マシーン
	std::unique_ptr<CSystemManager> m_pSystemManager;	// 処理を管理するクラスへのポインタ

	std::shared_ptr<CSceneState> m_pNextState;			// 次の状態(保存用)

	Mode m_mode;										// 現在のモード
	int m_nFps;											// フレームレート
	int m_nNextStateTime;								// 次の状態になるまでの時間
};

#endif