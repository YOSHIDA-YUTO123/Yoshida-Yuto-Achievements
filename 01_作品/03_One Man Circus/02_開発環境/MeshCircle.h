//================================================
//
// メッシュインパクト [impact.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _IMPACT_H_
#define _IMPACT_H_

//************************************************
// インクルードファイル
//************************************************
#include "object.h"

//************************************************
// メッシュサークル(平面)クラスの定義
//************************************************
class CMeshCircle : public CObject
{
public:
	CMeshCircle();
	~CMeshCircle();

	/// <summary>
	/// 生成処理(パラメーターの設定はSetParam関数で)
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="色"></param>
	/// <param name="内側の半径"></param>
	/// <param name="外側の半径"></param>
	/// <param name="分割数"></param>
	/// <returns>自分のインスタンス</returns>
	static CMeshCircle* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fInRadius, const float fOutRadius, const int nSegH = 32);

	/// <summary>
	/// パラメータの設定処理
	/// </summary>
	/// <param name="高さ(内側)"></param>
	/// <param name="拡大速度"></param>
	/// <param name="寿命"></param>
	/// <param name="向き"></param>
	void SetParam(const float fHeight, const float fSpeed, const int nLife, const D3DXVECTOR3 rot = Const::VEC3_NULL);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetTextureID(const char* pTexturePath);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXCOLOR m_InCol;						// 内側の色
	D3DXCOLOR m_OutCol;						// 外側の色
	float m_fInRadius;						// 内側の半径
	float m_fOutRadius;						// 外側の半径
	float m_fHeight;						// 高さ
	float m_fDecAlv;						// A値の減少値
	float m_fSpeed;							// 速さ
	int m_nSegH, m_nSegV;					// 横の分割数、縦の分割数
	int m_nNumVtx;							// 頂点の数
	int m_nNumPolygon;						// ポリゴンの数
	int m_nNumIdx;							// インデックス数
	int m_nLife;							// 寿命
	int m_nTextureIdx;						// テクスチャのID
};
#endif