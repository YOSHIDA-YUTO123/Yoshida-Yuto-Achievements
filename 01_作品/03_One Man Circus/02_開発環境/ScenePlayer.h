//================================================
//
// シーンのプレイヤーの基底クラス [ScenePlayer.h]
// Author:YUTO YOSHIDA
// 
// << 生成はScenePlayerFactoryのCreate関数で生成 >>
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _SCENE_PLAYER_H_
#define _SCENE_PLAYER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "object.h"
#include <memory>
#include <functional>

//*************************************************
// 前方宣言
//*************************************************
class CCharacter3D; // キャラクター

//*************************************************
// シーンのプレイヤーのクラスの定義
//*************************************************
class CScenePlayer : public CObject
{
public:

	// 種類
	enum class Type : uint8_t
	{
		Title = 0,	// タイトル
		Opening,	// オープニング
		GameEnd,	// ゲーム終了
		ResultWin,	// クリア
		ResultLose,	// 失敗
		Max
	};

	CScenePlayer();
	virtual ~CScenePlayer();

	void Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void RegisterEvent(std::function<void(void)> event) { m_Event = event; }

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

protected:
	// キャラクタークラスの取得 : 例 >> CCharacter3D *p = GetCharacter();
	CCharacter3D* GetCharacter(void) { return m_pCharacter.get(); }

	void BeginEvent(void) { if (m_Event != nullptr) m_Event(); }
private:
	std::function<void(void)> m_Event;			// イベント
	std::unique_ptr<CCharacter3D> m_pCharacter; // キャラクタークラスへのポインタ
};
#endif