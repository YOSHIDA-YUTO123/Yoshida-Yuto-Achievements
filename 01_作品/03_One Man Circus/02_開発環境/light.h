//===================================================
//
// ライト [light.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include <array>

//***************************************************
// ライトのクラスの定義
//***************************************************
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetDirectional(const D3DXCOLOR& Diffuse, const D3DXVECTOR3& dir, const D3DXVECTOR3& pos);						// ディレクショナルライトの設定処理
	int SetPoint(const D3DXVECTOR3& pos,const float fRange,const D3DXCOLOR& Diffuse,const D3DXCOLOR& Ambient);			// ポイントライトの設定処理
	void SetLight(void);
	void DeleteLight(const int nID);

	static constexpr int MAX_LIGHT = 8; // ライトの最大数

	// ライトの情報
	struct LightInfo
	{
		LightInfo() : 
			nID(-1), aLight(),bUse(false) 
		{}

		D3DLIGHT9 aLight;	// ライトの構造体
		int nID;			// ライトのID
		bool bUse;			// 使用状態
	};

	// ライトの取得
	LightInfo& GetInfo(const int nID) { return m_aLightInfo[nID]; }
private:

	std::array<LightInfo , MAX_LIGHT> m_aLightInfo; // ライトの構造体
};
#endif