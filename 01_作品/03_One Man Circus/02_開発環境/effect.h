//================================================
//
// エフェクト処理 [effect.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "billboard.h"

//*************************************************
// エフェクトクラスの定義
//*************************************************
class CEffect : public CObjectBillboard
{
public:
	CEffect();
	~CEffect();

	/// <summary>
	/// 生成処理 -> 使用例 : CEffect *pEffect = CEffect::Create(位置、色、半径、寿命);
	/// pEffect->SetParam(....);
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="色"></param>
	/// <param name="半径"></param>
	/// <param name="寿命"></param>
	/// <returns>自分のインスタンス</returns>
	static CEffect* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fRadius, const int nLife);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// パラメータの設定処理
	/// </summary>
	/// <param name="慣性"></param>
	void SetParam(const float fInertia, const D3DXVECTOR3 move, const float fGravity = 0.0f);
private:
	D3DXVECTOR3 m_move;	// 移動量
	D3DXCOLOR m_col;	// 色
	float m_fDecALv;	// アルファ値の減少スピード
	float m_fDecRadius;	// 半径の減少スピード
	float m_fInertia;	// 慣性
	float m_fGravity;	// 重力量
	int m_nMaxLife;		// 最大の寿命
	int m_nLife;		// 寿命
};
#endif