//==============================================
//
// 回転する床ギミックのクラスの定義 [RotationFloor.cpp]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "RotationFloor.h"
#include "math.h"
#include "transform.h"
#include "Collider.h"
#include "Collision.h"
#include "CollisionObject3D.h"

//**********************************************
// 定数宣言
//**********************************************
namespace Floor_Info
{
	constexpr float INERTIA_COEF = 0.25f;		// 減衰係数
	constexpr float CIRCULARMOVE_TIME = 0.05f;	// 回転運動の時間
	const char* MODEL_NAME = "Model()/move_floor.x"; // 読み込むパス
};

//==============================================
// コンストラクタ
//==============================================
CRotationFloor::CRotationFloor() : 
	m_fMoveVaule(NULL),
	m_fTime(NULL),
	m_move(Const::VEC3_NULL),
	m_fRotValue(NULL),
	m_pCollisionObj(nullptr),
	m_bRanding(false),
	m_nLightID(-1)
{
}

//==============================================
// デストラクタ
//==============================================
CRotationFloor::~CRotationFloor()
{
}

//==============================================
// 生成処理
//==============================================
CRotationFloor* CRotationFloor::Create(const D3DXVECTOR3 pos, const float fMove,const float fRot)
{
	CRotationFloor* pInstance = new CRotationFloor;

	pInstance->m_fMoveVaule = fMove;
	pInstance->m_fRotValue = fRot;

	// 初期化に失敗したら
	if (FAILED(pInstance->Init()))
	{
		// 破棄
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	// 空間情報の取得
	CTransform* pTransform = pInstance->m_pCollisionObj->GetTransform();

	if (pTransform != nullptr)
	{
		// 情報の取得
		CTransform::Info transform = pTransform->GetInfo();

		transform.pos = pos;

		pTransform->SetInfo(transform);
	}

	return pInstance;
}

//==============================================
// 初期化処理
//==============================================
HRESULT CRotationFloor::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}

	// 初期化処理
	if (FAILED(CObjectX::LoadModel(Floor_Info::MODEL_NAME)))
	{
		return E_FAIL;
	}

	if (m_pCollisionObj == nullptr)
	{
		// 生成
		m_pCollisionObj = CCollisionObject3D::Create();
	}

	// 生成できなかったら処理しない
	if (m_pCollisionObj == nullptr) return E_FAIL;

	// 空間情報の取得
	CTransform* pTransform = m_pCollisionObj->GetTransform();

	// 情報の取得
	CTransform::Info transform = pTransform->GetInfo();

	transform.rot.y = D3DX_PI * 0.25f;

	// 大きさの取得
	transform.Size = CObjectX::GetSize();

	m_pCollisionObj->AddCollider(CColliderOBB::Create());

	// 情報の設定
	pTransform->SetInfo(transform);

	return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CRotationFloor::Uninit(void)
{
	if (m_pCollisionObj != nullptr)
	{
		m_pCollisionObj->Uninit();
		m_pCollisionObj.reset();
	}

	// 終了処理
	CObjectX::Uninit();
}

//==============================================
// 更新処理
//==============================================
void CRotationFloor::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = m_pCollisionObj->GetTransform();

	// 情報の取得
	CTransform::Info transform = pTransform->GetInfo();

	// 回転移動
	CircularMove();

	// 移動量の減衰
	m_move.x += (0.0f - m_move.x) * Floor_Info::INERTIA_COEF;
	m_move.z += (0.0f - m_move.z) * Floor_Info::INERTIA_COEF;

	////// 回転させる
	transform.rot.y += m_fRotValue;

	// 移動量の更新
	transform.pos += m_move;

	CObjectX::SetPosition(transform.pos);
	CObjectX::SetRotation(transform.rot);

	// 情報の設定
	pTransform->SetInfo(transform);

	if (m_pCollisionObj != nullptr)
	{
		pTransform->CalcuMatrix();

		// 更新処理
		m_pCollisionObj->Update();
	}
}

//==============================================
// 描画処理
//==============================================
void CRotationFloor::Draw(void)
{
	if (m_pCollisionObj != nullptr)
	{
		// 描画処理
		m_pCollisionObj->SetMatrix();
	}

	// 描画の設定
	CObjectX::SetUpDraw();
}

//==============================================
// 判定処理
//==============================================
bool CRotationFloor::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 rayDir, D3DXMATRIX* pOutMatrix, D3DXVECTOR3* HitPos, const float fRayToFloorDistance)
{
#if 1
	pPos = nullptr;
	rayDir = {};
	pOutMatrix = nullptr;
	HitPos = nullptr;
	
	if (fRayToFloorDistance <= 0)
	{

	}
#else
	// 大きさの取得
	D3DXVECTOR3 Size = CObjectX::GetSize() * 0.5f;

	// 位置の取得
	D3DXVECTOR3 pos = CObjectX::GetPosition();
	D3DXMATRIX mtxWorld = CObjectX::GetMatrix();

	// 頂点数(AABB)
	const int NUM_AABB_VERTEX = 8;

	// 三角形の数
	const int NUM_TRAIANGLE = 12;

	// 矩形のローカル頂点の設定
	D3DXVECTOR3 VtxPos[NUM_AABB_VERTEX] =
	{
		{-Size.x,Size.y,Size.z},
		{Size.x,Size.y,Size.z},
		{-Size.x,Size.y,-Size.z},
		{Size.x,Size.y,-Size.z},

		{-Size.x,-Size.y,Size.z},
		{Size.x,-Size.y,Size.z},
		{-Size.x,-Size.y,-Size.z},
		{Size.x,-Size.y,-Size.z}
	};

	// ワールド座標
	D3DXVECTOR3 worldposMin = VtxPos[0], worldposMax = VtxPos[0];

	for (auto& vtx : VtxPos)
	{
		// ワールド座標に変換
		D3DXVec3TransformCoord(&vtx, &vtx, &mtxWorld);
	}

	// 三角形の構造体
	struct Traiangle
	{
		D3DXVECTOR3 vtx0;
		D3DXVECTOR3 vtx1;
		D3DXVECTOR3 vtx2;
	};

	// 三角形を設定
	Traiangle traiangle[NUM_TRAIANGLE] =
	{
		// 上の面
		{ VtxPos[0],VtxPos[1],VtxPos[2] },
		{ VtxPos[1],VtxPos[2],VtxPos[3] },

		// 下の面
		{ VtxPos[4],VtxPos[5],VtxPos[6] },
		{ VtxPos[5],VtxPos[6],VtxPos[7] },

		// 左の面
		{ VtxPos[0],VtxPos[2],VtxPos[4] },
		{ VtxPos[2],VtxPos[4],VtxPos[6] },

		// 右の面
		{ VtxPos[1],VtxPos[3],VtxPos[5] },
		{ VtxPos[3],VtxPos[5],VtxPos[7] },

		// 奥の面
		{ VtxPos[0],VtxPos[1],VtxPos[4] },
		{ VtxPos[1],VtxPos[4],VtxPos[5] },

		// 手前の面
		{ VtxPos[2],VtxPos[3],VtxPos[6] },
		{ VtxPos[3],VtxPos[6],VtxPos[7] },

	};

	// 三角形の数分調べる
	for (auto& trianglepos : traiangle)
	{
		// 距離
		float fDistance = 0.0f;

		// 衝突点
		float posU, posV;

		// 三角形とレイの当たり判定
		if (D3DXIntersectTri(&trianglepos.vtx0, &trianglepos.vtx1, &trianglepos.vtx2,
			pPos, &rayDir, &posU, &posV, &fDistance))
		{
			// 衝突点
			D3DXVECTOR3 hitPoint = (1.0f - posU - posV) * trianglepos.vtx0 + posU * trianglepos.vtx1 + posV * trianglepos.vtx2;

			// 衝突点とレイの距離が範囲内か判定
			const bool CheckRayDistance = math::GetDistance(hitPoint - *pPos) <= fRayToFloorDistance;

			// レイと床の距離が指定範囲外だったら
			if (!CheckRayDistance)
			{
				return false;
			}

			if (HitPos != nullptr)
			{
				*HitPos = hitPoint;
			}

			if (pOutMatrix != nullptr)
			{
				*pOutMatrix = GetMatrix();
			}

			return true;
		}
	}
#endif // _DEBUG

	return false;
}

//==============================================
// 当たり判定処理
//==============================================
bool CRotationFloor::Collision(CCollider* pAABB, D3DXMATRIX* pOutMatrix, int* pFace)
{
	CCollider* pObb = nullptr;

	// コライダーを調べる
	for (auto& collider : m_pCollisionObj->GetCollider())
	{
		// OBBのコライダーがあるなら
		if (collider->GetType() == CCollider::TYPE_OBB)
		{
			// コライダーの取得
			pObb = collider;
		}
	}

	// 取得できなかったら処理しない
	if (pObb == nullptr) return false;

	D3DXVECTOR3 push;

	// 当たり判定処理
	if (CCollisionOBB::Collision(pObb, pAABB, &push, pFace))
	{
		if (pOutMatrix != nullptr)
		{
			pObb->GetTransform()->CalcuMatrix();

			// ワールドマトリックスの取得
			*pOutMatrix = pObb->GetTransform()->GetInfo().mtxWorld;
		}

		return true;
	}
	return false;
}

//==============================================
// 回転移動
//==============================================
void CRotationFloor::CircularMove(void)
{
	m_fTime += Floor_Info::CIRCULARMOVE_TIME;

	// 円運動の速度ベクトルを計算
	m_move.x = -sinf(m_fTime) * m_fMoveVaule;
	m_move.z = cosf(m_fTime) * m_fMoveVaule;
}
