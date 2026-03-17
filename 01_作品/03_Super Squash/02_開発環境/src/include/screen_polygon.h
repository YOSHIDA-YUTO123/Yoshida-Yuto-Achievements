//===================================================
//
// 画面のポリゴンの描画 [screen_polygon.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCREEN_POLYGON_H_
#define _SCREEN_POLYGON_H_

//***************************************************
// 画面のポリゴンの描画クラスの定義
//***************************************************
class CScreenPolygon
{
public:
	static constexpr int MAX_TEXTURE = 2;	// テクスチャの数

	CScreenPolygon();
	~CScreenPolygon();

	static std::unique_ptr<CScreenPolygon> Create(const int nMRTType1, const int nMRTType2,const int nShaderType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(const int nPass = 0);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	int m_nMRTType[MAX_TEXTURE];			// MRTの種類
	int m_nShaderType;						// シェーダーの種類
};
#endif