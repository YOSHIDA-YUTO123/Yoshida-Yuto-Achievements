//===================================================
//
// シーンを操作するクラス [scene_controller.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCENE_CONTROLLER_H_
#define _SCENE_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CCameraAnimationSystem;	// カメラのアニメーション
class CScene;					// シーン
class CSceneState;				// シーンの状態

//***************************************************
// シーンのコントローラーのクラスの定義
//***************************************************
class CSceneController
{
public:
	// シーンの状態
	enum class State
	{
		Normal = 0,		// 通常状態
		ChangeScene,	// シーン変更状態
		Max
	};

	CSceneController(CScene* pScene);
	~CSceneController();

	static std::unique_ptr<CSceneController> Create(CScene* pScene);

	void ChangeScene(std::shared_ptr<CSceneState> pNewScene, const int nAnimType, const int nTime = 0);
	void FInishChangeScene(void) { m_state = State::Normal; }

	State GetState(void) const { return m_state; }
	bool GetPauseState(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
private:
	CScene* m_pScene;	// シーンクラスへのポインタ
	State m_state;		// 状態
};
#endif