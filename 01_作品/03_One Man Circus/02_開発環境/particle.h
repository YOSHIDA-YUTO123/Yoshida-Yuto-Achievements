//================================================
//
// パーティクル処理 [particle.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "object.h"

//*************************************************
// パーティクルのクラスの定義
//*************************************************
class CParticle : public CObject
{
public:

	// 種類
	enum TYPE
	{
		TYPE_NORMAL = 0,	// 通常(effect000.jpg)
		TYPE_RESULT,		// リザルト
		TYPE_SMOKE,			// 煙
		TYPE_MAX
	};

	CParticle();
	~CParticle();

	/// <summary>
	/// パーティクルの生成処理 >> 例 CParticle *p = CParticle::Create(....); 細かい設定は >>
	/// p->SetParam(......)
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="col"></param>
	/// <param name="fRadius"></param>
	/// <param name="nNum"></param>
	/// <returns></returns>
	static CParticle* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius, const int nNum, const TYPE type);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// パラメータの設定処理
	/// </summary>
	/// <param name="移動量"></param>
	/// <param name="最大の放射角度"></param>
	/// <param name="最小の放射角度"></param>
	/// <param name="発生時間"></param>
	/// <param name="寿命"></param>
	void SetParam(const D3DXVECTOR3 move, const int nSpawnTime, const int nLife, const int nAngleMax = 314, const int nAngleMin = -314);

	/// <summary>
	/// パラメータの設定処理
	/// </summary>
	/// <param name="慣性"></param>
	/// <param name="重力量"></param>
	void SetParam(const float fInertia, const float fGravity = 0.0f, const float fPosRange = 0.0f);

	void SetAnim(const int nDiviU, const int nDiviV, const int nAnimSpeed, const bool bLoop);
private:

	struct Anim
	{
		int nAnimSpeed;		// アニメーションスピード
		int nDiviU;			// 分割数横
		int nDiviV;			// 分割数縦
		bool bLoop;			// ループするかどうか
	};

	Anim m_AnimParam;	// アニメーションのパラメータ 
	TYPE m_type;		// 種類
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXCOLOR m_col;	// 色
	float m_fPosRange;	// 位置の範囲
	float m_fRadius;	// 半径
	float m_fInertia;	// 慣性
	float m_fGravity;	// 重力量
	int m_nAngleMax;	// 最大の放射角度
	int m_nAngleMin;	// 最小の放射角度
	int m_nNum;			// 発生数
	int m_nSpawnTime;	// 発生時間
	int m_nLife;		// エフェクトの寿命
	bool m_bAnim;		// アニメーションするかどうか
};
#endif