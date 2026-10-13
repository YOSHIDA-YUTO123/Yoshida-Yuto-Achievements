//==============================================
//
// 回転する床ギミックのクラスの定義 [RotationFloor.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _ROTATIONFLOOR_H_
#define _ROTATIONFLOOR_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "objectX.h"
#include <memory>

//**********************************************
// 前方宣言
//**********************************************
class CCollisionObject3D;	// 当たり判定するモデルのオブジェクト
class CCollider;			// コライダー

//**********************************************
// 回転する床のクラスの定義
//**********************************************
class CRotationFloor : public CObjectX
{
public:
	CRotationFloor();
	~CRotationFloor();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="移動速度"></param>
	/// <param name="回転量"></param>
	/// <returns>インスタンス</returns>
	static CRotationFloor* Create(const D3DXVECTOR3 pos, const float fMove, const float fRot);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 当たり判定の処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="レイの方向(今は真下(y = -1.0f))"></param>
	/// <param name="ブロックのマトリックス"></param>
	/// <param name="衝突点"></param>
	/// <param name="判定をする床とレイの原点の距離"></param>
	/// <returns>判定結果</returns>
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 rayDir, D3DXMATRIX* pOutMatrix, D3DXVECTOR3* HitPos, const float fRayToFloorDistance);
	bool Collision(CCollider* pAABB, D3DXMATRIX* pOutMatrix, int *pFace);

	void SetIsRanding(bool bRanding) { m_bRanding = bRanding; }
	bool GetIsRanding(void) { return m_bRanding; }

	void SetLightID(const int nID) { m_nLightID = nID; }
	const int GetLightID(void) const { return m_nLightID; }

private:
	void CircularMove(void);

	std::unique_ptr<CCollisionObject3D> m_pCollisionObj; // 当たり判定するモデルのクラスへのポインタ
	D3DXVECTOR3 m_move;	// 移動量
	int m_nLightID;		// ライトのID
	float m_fTime;		// 回転時間
	float m_fMoveVaule;	// 移動量
	float m_fRotValue;	// 回転量
	bool m_bRanding;	// 乗られたか否か
};
#endif