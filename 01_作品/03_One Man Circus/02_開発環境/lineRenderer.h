//==============================================
//
// 線の描画をするクラス [line.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _LINE_H_
#define _LINE_H_

//**********************************************
// インクルードファイル
//**********************************************
#include <vector>
#include "object.h"

//**********************************************
// 線の描画クラスの定義
//**********************************************
class CLineRenderer : public CObject
{
public:

	// 頂点構造体
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	};

	~CLineRenderer();
	static CLineRenderer* GetInstance(void);
	static void SetShow(const bool bEnable) { m_bShow = bEnable; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override {};
	void Draw(void) override;

	void AddLine(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXCOLOR col);
private:
	CLineRenderer();

	static constexpr int NUM_VERTEX = 25600;	// 頂点の総数

	static CLineRenderer* m_pInstance;		// 自分のインスタンス
	static bool m_bShow;					// 表示するかどうか
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	std::vector<Vertex> m_aLine;			// 線の情報
};
#endif
