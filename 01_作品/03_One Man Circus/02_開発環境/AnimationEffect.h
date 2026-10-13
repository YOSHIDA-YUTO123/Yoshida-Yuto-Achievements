//===================================================
//
// アニメーションするエフェクト [AnimationEffect.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _ANIMATION_EFFECT_H_
#define _ANIMATION_EFFECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "billboardAnim.h"

//***************************************************
// アニメーションエフェクトのクラスの定義
//***************************************************
class CAnimationEffect : public CBillboardAnimation
{
public:
	CAnimationEffect();
	~CAnimationEffect();

	static CAnimationEffect* Create(const D3DXVECTOR3& pos, const D3DXCOLOR& col, const float fRadius, const int nDiviU, const int nDiviV, const int nLife);

	void SetParam(const float fInertia, const D3DXVECTOR3 move, const int nAnimSpeed, const bool bLoop, const float fGravity = 0.0f);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
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