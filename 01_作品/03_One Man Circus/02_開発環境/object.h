//===================================================
//
// オブジェクトの描画処理 [object.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"main.h"

//***************************************************
// オブジェクトクラスの定義
//***************************************************
class CObject
{
public:

	// 優先順位
	typedef enum
	{
		PRIORITY_NONE = 0,	// 特に無し
		PRIORITY_MESH,		// メッシュ
		PRIORITY_OBJECTX,   // モデル
		PRIORITY_CHARACTER,	// キャラクター
		PRIORITY_SHADOW,	// 影
		PRIORTTY_COLLISION, // コライダー
		PRIORITY_3DPOLYGON, // 3Dポリゴン
		PRIORITY_BILLBOARD,	// ビルボード
		PRIORITY_UI_BACK,	// UI後ろ
		PRIORITY_UI_FRONT,	// UI手前
		PRIORITY_MAX
	}PRIORITY;

	// オブジェクトの種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_OBSTACLE,
		TYPE_PAUSE,
		TYPE_MAX
	}TYPE;

	CObject(int nPriority = PRIORITY_NONE);
	virtual ~CObject();

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumObject(const int nPriority) { return m_nNumAll[nPriority]; }
	static void DestroyAll(void);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	int GetPriority(void) const { return m_nPriority; }
	void SetType(const TYPE type) { m_type = type; }
	TYPE GetType(void) const { return m_type; }
	bool GetDeath(void) const { return m_bDeath; }	// 死亡フラグの取得

protected:
	void Release(void);								// 死亡フラグを立てる
private:
	void Destroy(CObject* pObject);
	static int m_nNumAll[PRIORITY_MAX];				// オブジェクトの総数
	static CObject* m_pTop[PRIORITY_MAX];			// 先頭オブジェクトへのポインタ
	static CObject* m_pCur[PRIORITY_MAX];			// 最後尾オブジェクトへのポインタ
	CObject* m_pPrev;								// 前のオブジェクトへのポインタ
	CObject* m_pNext;								// 次のオブジェクトへのポインタ
	TYPE m_type;									// オブジェクトの種類
	int m_nPriority;								// 優先順位
	bool m_bDeath;									// 死亡フラグ
};

#endif