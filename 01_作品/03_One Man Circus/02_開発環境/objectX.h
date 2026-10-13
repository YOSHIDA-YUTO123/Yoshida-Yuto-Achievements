//===================================================
//
// オブジェクトXファイル [objectX.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// オブジェクトXファイルのクラスの定義
//***************************************************
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = PRIORITY::PRIORITY_OBJECTX);
	virtual ~CObjectX();

	static CObjectX* Create(const D3DXVECTOR3 pos, const char* pModelName,const D3DXVECTOR3 rot);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	
	void Draw(const float Diffuse); // 透明度を変更できる描画処理
	void DrawShadow(void);
	void SetUpMatrix(const D3DXVECTOR3 Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void SetUpMatrix(const D3DXMATRIX mtxRot, const D3DXMATRIX mtxTrans,const D3DXVECTOR3 Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	void SetMatrix(const D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetUpDraw(const float Diffuse = 1.0f);

	HRESULT LoadModel(const char* pXFileName);
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }

	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetSize(void);
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetMax(void) { return m_max; }
	D3DXVECTOR3 GetMin(void) { return m_min; }
	D3DXMATRIX GetMatrix(void) const { return m_mtxWorld; }
	D3DXMATRIX* GetPointerMatrix(void) { return &m_mtxWorld; }
	void CreateMaxMin(void);
private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_max;			// 最大値
	D3DXVECTOR3 m_min;			// 最小値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	int m_nModelIdx;			// モデルのID
};
#endif
