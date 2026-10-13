//===================================================
//
// 3Dオブジェクトの描画 [object3D.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// オブジェクト3Dクラスの定義
//***************************************************
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = PRIORITY::PRIORITY_3DPOLYGON);
	~CObject3D();

	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const char* pTextureName, const bool bLight_off = false);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetDraw(void);

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotaition(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const D3DXVECTOR3 Size) { m_Size = Size; }

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetSize(void) const { return m_Size; }

	void SetOffsetVtx(const D3DXCOLOR col = Const::WHITE, const int nPosX = 1, const int nPosY = 1);
	void SetTextureID(const char* pTextureName);
	void SetTexture(const float fX1, const float fY1, const float fX2, const float fY2);
	void UpdatePosition(const D3DXVECTOR3 pos, const D3DXVECTOR3 Size);
	void UpdateCol(const D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_Size;						// 大きさ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	int m_nTextureIdx;						// テクスチャのID
	bool m_bLight_off;						// ライトの影響を受けないかどうか							
};
#endif