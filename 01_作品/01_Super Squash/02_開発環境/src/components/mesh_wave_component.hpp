//===================================================
//
// メッシュの波のコンポーネント [mesh_wave_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_WAVE_COMPONENT_HPP_
#define _MESH_WAVE_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// メッシュの波のデータの定義
//***************************************************
struct MeshWaveComponent
{
	// 波の種類
	enum class Type : uint8_t
	{
		XZ = 0,		// XZの平面の波
		XY,			// XYの平面の波
		MULT_XZ,	// XYの平面の波マルチテクスチャ
		MULT_XY,	// XYの平面の波マルチテクスチャ
		Max
	};

	// 波のパラメータ
	struct Param
	{
		D3DXVECTOR3 epicenter;			// 波の発生地点
		float fInRadius;				// 波の内半径
		float fOutRadius;				// 波の外半径
		float fHeight;					// 波の高さ
		float fSpeed;					// 速さ
		float fCoef;					// 係数
		int nTime;						// 波の発生時間
	};

	MeshWaveComponent(const Type _type) :
		type(_type), param(), fStartHeight(param.fHeight),
		fTime(0.0f), nCounter(0), meshID(entt::null),
		col(Const::WHITE)
	{}
	
	Type type;						// 種類
	Param param;					// パラメータ
	entt::entity meshID;			// メッシュのID
	D3DXCOLOR col;					// 波の影響を受けた頂点の色
	float fStartHeight;				// 最初の高さ
	float fTime;					// 高さ
	int nCounter;					// 波のカウンター
};

#endif