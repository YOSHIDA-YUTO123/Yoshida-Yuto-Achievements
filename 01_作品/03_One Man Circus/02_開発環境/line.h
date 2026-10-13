//***********************************************
// 
// 線
// Author Tetsuji Yamamoto
// 
//***********************************************
#include "main.h"
#include "object.h"

// 線クラス
class CLine : CObject
{
public:
	CLine();
	~CLine();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 posA, D3DXVECTOR3 posB,D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLine* Create(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	int m_NumLine;						// 線の数
};