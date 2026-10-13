//===================================================
//
// キャラクターの構成 [character.h]
// Author:YUTO YOSHIDA
// 
// << 3Dキャラクターの見た目、モーションを作る基底クラス >>
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER3D_H_
#define _CHARACTER3D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include <memory>
#include <vector>

//***************************************************
// 前方宣言
//***************************************************
class CShadowS;
class CRotation;
class CMotion;
class CModel;

//***************************************************
// キャラクター3Dクラスの定義
//***************************************************
class CCharacter3D
{
public:

	// 状態の種類
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE,
		STATE_ACTION,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	// キャラクターの種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_MAX
	}TYPE;

	CCharacter3D();
	CCharacter3D(const TYPE type);
	virtual ~CCharacter3D();

	/// <summary>
	/// 生成処理(パラメータの設定はSetCharacter関数でできます)
	/// </summary>
	/// <param name="モーションのTXTファイルパス"></param>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションの総数"></param>
	/// <returns>インスタンス(unique_ptr型で受け取る)</returns>
	static std::unique_ptr<CCharacter3D> Create(const char* pMotionFileName,const D3DXVECTOR3 pos,const D3DXVECTOR3 rot, const int nNumMotion);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	/// <summary>
	/// 計算後のマトリックスを設定できる描画処理
	/// </summary>
	/// <param name="計算語のマトリックス"></param>
	void Draw(const D3DXMATRIX mtxWorld);
	void Draw(const float fAvl);

	TYPE GetType(void) const { return m_type; }
	CMotion* LoadMotion(const char *pFileName, const int nNumMotion);

	// ゲッター
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetSize(void) const { return m_Size; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetModelPos(const int nIdx);
	D3DXVECTOR3 GetModelRot(const int nIdx);
	D3DXVECTOR3 GetModelSize(const int nIdx);

	STATE GetState(void) { return m_state; }
	CMotion* GetMotion(void) { return m_pMotion.get(); } // モーションの取得
	float GetSpeed(void) { return m_fSpeed; }
	int GetLife(void) const { return m_nLife; }
	D3DXVECTOR3 GetRotDest(void) const { return m_rotDest; }

	// セッター
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 ShadowScal, const D3DXVECTOR3 Size);
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }

	bool GetAlive(void);		// 生きているか

	void SetState(const STATE state,const int nTime);
	void DeleteShadow(void);	// 影の消去
	bool Hit(int nDamage);		// ヒット時の処理
	bool HitStop(void);			// ヒットストップしてるかどうか
	void SetHitStop(const int nTime) { m_nHitStopTime = nTime; } // ヒットストップの設定
	void UpdateMotion(void);
private:

	std::unique_ptr<CMotion> m_pMotion;		// モーションのクラスへのポインタ
	std::vector<CModel*> m_apModel;			// モデルクラスのポインタ
	CShadowS* m_pShadowS;					// 影(ステンシル)
	STATE m_state;							// 状態
	TYPE m_type;							// 種類
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_ShadowScal;				// 影の大きさ
	D3DXVECTOR3 m_Size;						// 大きさ
	D3DXVECTOR3 m_rotDest;					// 目的の角度
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	float m_fSpeed;							// 足の速さ
	int m_nNumModel;						// モデルの最大数
	int m_nCounterState;					// 状態のカウンター
	int m_nLife;							// 寿命
	int m_nHitStopTime;						// ヒットストップの時間
};
#endif