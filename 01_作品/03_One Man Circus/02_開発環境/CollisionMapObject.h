//===================================================
//
// 当たり判定をするマップオブジェクトクラス [CollisionMapObject.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISIONMAPOBJECT_H_
#define _COLLISIONMAPOBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "objectX.h"
#include <memory>

//***************************************************
// 前方宣言
//***************************************************
class CCollisionObject3D;

//***************************************************
// 当たり判定をするマップオブジェクトのクラスの定義
//***************************************************
class CCollisionMapObject : public CObject
{
public:
	CCollisionMapObject();
	~CCollisionMapObject();

	static CCollisionMapObject* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	HRESULT LoadModel(const char* pModelFileName);
	CCollisionObject3D* GetCollision(void) { return m_pCollisionObj.get(); }

	// ゲッター
	std::string& GetXFilePath(void) { return m_Xfilepath; }
	bool GetIsRanding(void) { return m_bRanding; }
	int GetModelID(void) { return m_nModelID; }
	const int GetLightID(void) const { return m_nLightID; }

	// セッター
	void SetIsRanding(bool bRanding) { m_bRanding = bRanding; }
	void SetLightID(const int nID) { m_nLightID = nID; }
private:
	std::unique_ptr<CCollisionObject3D> m_pCollisionObj;	// 当たり判定をするオブジェクトへのポインタ
	std::string m_Xfilepath;								// Xファイルパス
	int m_nLightID;											// ライトのID
	int m_nModelID;											// モデルのID
	bool m_bRanding;										// 乗ったことがあるかどうか
};

#endif
