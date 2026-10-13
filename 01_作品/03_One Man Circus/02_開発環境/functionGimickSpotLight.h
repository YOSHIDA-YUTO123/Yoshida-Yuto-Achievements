//===================================================
//
// 近いギミックにスポットライトを当てる処理 [functionGimickSpotLight.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FUNCTION_GIMICK_SPOT_LIGHT_H_
#define _FUNCTION_GIMICK_SPOT_LIGHT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "functionBase.h"

//***************************************************
// 前方宣言
//***************************************************
class CCollisionMapObjManager;	// マップオブジェクトのマネージャー
class CCollisionMapObject;		// マップオブジェクト
class CPlayer;					// プレイヤー
class CLight;					// ライト

//***************************************************
// ギミックにスポットライトを当てる処理
//***************************************************
class CFunctionGimickSpotLight : public CFunctionBase
{
public:
	CFunctionGimickSpotLight() = default;
	CFunctionGimickSpotLight(CCollisionMapObjManager *pMapObjectManager);
	~CFunctionGimickSpotLight();

	virtual void Init(void) override {};
	virtual void Uninit(void) override{};
	virtual void Update(void) override;
private:
	bool CheckGimickObj(CCollisionMapObject* pMapObj);
	void SetRotationFloorLighting(CPlayer* pPlayer, CLight* pLight);
private:
	CCollisionMapObjManager* m_pMapObjectManagerRef; // 参照用マップオブジェクトのマネージャー
};
#endif