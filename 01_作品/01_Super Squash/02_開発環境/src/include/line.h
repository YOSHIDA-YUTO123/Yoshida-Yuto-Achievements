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
#include "vector"

//**********************************************
// 線の描画クラスの定義
//**********************************************
class CLineRenderer
{
public:

	// 頂点構造体
	struct Vertex
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXCOLOR col;		// 色
	};

	~CLineRenderer();

	static CLineRenderer* GetInstance(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void) {};
	void Draw(void);

	void AddLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& col);
private:
	CLineRenderer();

	static constexpr int NUM_VERTEX = 4096;	// 頂点の総数

	static std::unique_ptr<CLineRenderer> m_pInstance; // 自分のインスタンス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	std::vector<Vertex> m_aLine;			// 線の情報
};
#endif
