//===================================================
//
// メニューの見た目の処理 [MenuVisual.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MenuVisual.h"

//===================================================
// ハイライトの更新処理
//===================================================
void CMenuHighLightVisual::Update(const bool bSelect, const D3DXVECTOR3& pos, const D3DXVECTOR2& BaseSize, D3DXCOLOR& col, float& fScal)
{
	// 使用してないので警告を消す
	(void)fScal;
	(void)pos;
	(void)BaseSize;

	col.a = bSelect ? 1.0f : 0.5f;
}

//===================================================
// ハイライト点滅のコンストラクタ
//===================================================
CMenuHighLightFlashVisual::CMenuHighLightFlashVisual() : 
	m_fCounter(NULL)
{
}

//===================================================
// ハイライト点滅の更新処理
//===================================================
void CMenuHighLightFlashVisual::Update(const bool bSelect, const D3DXVECTOR3& pos, const D3DXVECTOR2& BaseSize, D3DXCOLOR& col, float& fScal)
{
	// 使用してないので警告を消す
	(void)fScal;
	(void)pos;
	(void)BaseSize;

	// カウンターを加算
	m_fCounter += 0.03f;

	float fOffset = 0.9f;

	if (bSelect)
	{
		// 点滅させる
		col.a = (1.0f - fabsf(sinf(m_fCounter)) * fOffset);
	}
	else
	{
		m_fCounter = 0.0f;

		col.a = 0.7f;
	}
}