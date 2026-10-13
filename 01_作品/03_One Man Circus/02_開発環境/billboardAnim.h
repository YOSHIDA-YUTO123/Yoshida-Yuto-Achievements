//===================================================
//
// アニメーションビルボードの描画をするクラス [billboardAnim.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _BILLBOARDANIM_H_
#define _BILLBOARDANIM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// ビルボード(アニメーション)クラスの定義
//***************************************************
class CBillboardAnimation : public CObject
{
public:
	CBillboardAnimation(int nPriority = PRIORITY::PRIORITY_BILLBOARD);
	virtual ~CBillboardAnimation();

	/// <summary>
	/// 生成処理(SetParam関数でアニメーションの設定をしてください)
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="大きさ"></param>
	/// <param name="横の分割数"></param>
	/// <param name="縦の分割数"></param>
	/// <returns>自分のインスタンス</returns>
	static CBillboardAnimation* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size,const int nDivU,const int nDivV);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR2 GetSize(void) const { return m_Size; }

	void SetParam(const int nSpeed, const bool bLoop);
	void SetDivUV(const int nDivU, const int nDivV);

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(const D3DXVECTOR2 size) { m_Size = size; }

	void SetTextureVtx(const D3DXVECTOR2 OffPos, const D3DXVECTOR2 RateUV);
	void SetColor(const D3DXCOLOR col);
	void SetTextureID(const char* pTextureName);
	void UpdateVertexPos(const D3DXVECTOR3 pos);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXVECTOR3 m_pos;						// 位置のクラスのポインタ
	D3DXVECTOR2 m_Size;						// 大きさ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	float m_fPosTexU;						// テクスチャ座標UV
	float m_fPosTexH;						// テクスチャ座標HV
	int m_nSpeed;							// アニメーションの速さ
	int m_nPattern;							// アニメーションのパターン
	int m_nCounter;							// アニメーションのカウンター
	int m_nDivU;							// テクスチャ座標の分割数横
	int m_nDivV;							// テクスチャ座標の分割数縦
	int m_nTextureIdx;						// テクスチャのID
	bool m_bLoop;							// ループするかどうか
};

#endif