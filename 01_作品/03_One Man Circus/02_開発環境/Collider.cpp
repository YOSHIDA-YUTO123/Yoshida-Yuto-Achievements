//================================================
//
// コライダー [collider.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "Collider.h"
#include "object.h"
#include "math.h"
#include "transform.h"
#include "line.h"
#include "debugproc.h"
#include "lineRenderer.h"

//================================================
// コンストラクタ
//================================================
CCollider::CCollider() :
	m_col(Const::WHITE),
	m_pTransform(nullptr),
	m_type(TYPE_BASE),
	m_tag()
{
}

//================================================
// コンストラクタ
//================================================
CCollider::CCollider(const TYPE type) : 
	m_pTransform(nullptr),
	m_tag(),
	m_type(type),
	m_col(Const::WHITE)
{
}

//================================================
// デストラクタ
//================================================
CCollider::~CCollider()
{
	m_tag.clear();
	m_pTransform = nullptr;
}

//================================================
// 更新処理
//================================================
void CCollider::Update(void)
{
	m_col = m_bHit ? D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) : Const::WHITE;

#ifdef _DEBUG
	DrawLine();
#endif // _DEBUG
}

//================================================
// 終了処理(自分で破棄)
//================================================
void CCollider::DeleteTransform(void)
{
	// 自分自身の破棄
	if (m_pTransform != nullptr)
	{
		delete m_pTransform;
		m_pTransform = nullptr;
	}
}

//================================================
// コンストラクタ
//================================================
CColliderAABB::CColliderAABB() :
	CCollider(TYPE_AABB),
	m_Data()
{

}

//================================================
// デストラクタ
//================================================
CColliderAABB::~CColliderAABB()
{
}

//================================================
// 更新処理
//================================================
void CColliderAABB::Update(void)
{
	// データの更新処理
	UpdateData();

	// 更新処理
	CCollider::Update();
}

//================================================
// データの更新処理
//================================================
void CColliderAABB::UpdateData(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 空間情報の取得
	CTransform::Info transform = pTransform->GetInfo();

	// ワールド座標に変換
	D3DXVec3TransformCoord(&m_Data.WorldCenterPos, &m_Data.LocalCenterPos, &transform.mtxWorld);
}

//================================================
// コライダーの作成処理
//================================================
std::unique_ptr<CColliderAABB> CColliderAABB::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABBの生成
	std::unique_ptr<CColliderAABB> pAABB = std::make_unique<CColliderAABB>();

	// 空間情報の取得
	CTransform *pTransform = pAABB->GetTransform();

	// 取得できないなら処理しない
	if (pTransform == nullptr) return nullptr;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// 情報の設定
	pTransform->SetInfo(info);

	return pAABB;
}

//================================================
// コライダーの生成処理
//================================================
CColliderAABB CColliderAABB::CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB
	CColliderAABB aabb;

	// 空間情報の取得
	CTransform* pTransform = aabb.GetTransform();

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// 情報の設定
	pTransform->SetInfo(info);

	return aabb;
}

//================================================
// 生成処理
//================================================
CColliderAABB* CColliderAABB::Create(const D3DXVECTOR3 CenterPos)
{
	CColliderAABB* pinstance = new CColliderAABB;

	pinstance->m_Data.LocalCenterPos = CenterPos;

	return pinstance;
}

//================================================
// 線の描画
//================================================
void CColliderAABB::DrawLine(void)
{
	// 線の描画関数の取得
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// 取得できなかったら処理しない
	if (pLine == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 色の設定
	D3DXCOLOR col = CCollider::GetLineColor();

	D3DXVECTOR3 Size = info.Size * 0.5f;
	D3DXVECTOR3 pos = m_Data.WorldCenterPos;

	D3DXVECTOR3 vtx[8] =
	{
		{ pos.x - Size.x, pos.y - Size.y,pos.z - Size.z },
		{ pos.x + Size.x, pos.y - Size.y,pos.z - Size.z },
		{ pos.x + Size.x, pos.y + Size.y,pos.z - Size.z },
		{ pos.x - Size.x, pos.y + Size.y,pos.z - Size.z },

		{ pos.x - Size.x, pos.y - Size.y,pos.z + Size.z },
		{ pos.x + Size.x, pos.y - Size.y,pos.z + Size.z },
		{ pos.x + Size.x, pos.y + Size.y,pos.z + Size.z },
		{ pos.x - Size.x, pos.y + Size.y,pos.z + Size.z },
	};

	pLine->AddLine(vtx[0], vtx[1], col);
	pLine->AddLine(vtx[1], vtx[2], col);
	pLine->AddLine(vtx[2], vtx[3], col);
	pLine->AddLine(vtx[3], vtx[0], col);

	pLine->AddLine(vtx[4], vtx[5], col);
	pLine->AddLine(vtx[5], vtx[6], col);
	pLine->AddLine(vtx[6], vtx[7], col);
	pLine->AddLine(vtx[7], vtx[4], col);

	pLine->AddLine(vtx[0], vtx[4], col);
	pLine->AddLine(vtx[1], vtx[5], col);
	pLine->AddLine(vtx[2], vtx[6], col);
	pLine->AddLine(vtx[3], vtx[7], col);
}

//================================================
// コンストラクタ
//================================================
CColliderSphere::CColliderSphere() : CCollider(TYPE_SPHERE)
{
}

//================================================
// デストラクタ
//================================================
CColliderSphere::~CColliderSphere()
{
}

//================================================
// 更新処理
//================================================
void CColliderSphere::Update(void)
{
	// 更新処理
	CCollider::Update();
}

//================================================
// 円のコライダーの作成
//================================================
std::unique_ptr<CColliderSphere> CColliderSphere::Create(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphereの生成
	std::unique_ptr<CColliderSphere> pSphere = std::make_unique<CColliderSphere>();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return nullptr;

	// 空間情報の取得
	pSphere->SetTransform(pTransform);

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// 生ポインタで生成する
//================================================
CColliderSphere* CColliderSphere::CreateRawPtr(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphereの生成
	CColliderSphere* pSphere = new CColliderSphere;

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return nullptr;

	// 空間情報の取得
	pSphere->SetTransform(pTransform);
	
	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// コライダーの作成処理
//================================================
CColliderSphere CColliderSphere::CreateCollider(const D3DXVECTOR3 pos, const float fRadius)
{
	// コライダー
	CColliderSphere sphere;

	// 空間情報の取得
	CTransform* pTransform = CTransform::Create();

	// 空間情報の取得
	sphere.SetTransform(pTransform);

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return sphere;
}

//================================================
// 線の描画
//================================================
void CColliderSphere::DrawLine(void)
{
	// 線の描画関数の取得
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// 取得できなかったら処理しない
	if (pLine == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	const int nSegX = 8;
	const int nSegZ = 8;
	const float fRadius = info.fRadius;

	// 位置
	D3DXVECTOR3 pos = info.pos;

	// Zの分割分回す
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		// Xの分割分回す
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			// 角度の割合を計算
			float fRateAngleX = (D3DX_PI * 2.0f) / nSegX;
			float fRateAngleZ = D3DX_PI / nSegZ;

			// 始点の角度の計算
			float fStartAngleX = fRateAngleX * nCntX;
			float fStartAngleZ = fRateAngleZ * nCntZ;

			// 終点の角度の計算
			float fEndAngleX = fRateAngleX * (nCntX + 1);

			D3DXVECTOR3 start,end;

			// 始点の計算
			start.x = pos.x + sinf(fStartAngleZ) * sinf(fStartAngleX) * fRadius;
			start.y = pos.y + cosf(fStartAngleZ) * fRadius;
			start.z = pos.z + sinf(fStartAngleZ) * cosf(fStartAngleX) * fRadius;

			// 終点の計算
			end.x = pos.x + sinf(fStartAngleZ) * sinf(fEndAngleX) * fRadius;
			end.y = pos.y + cosf(fStartAngleZ) * fRadius;
			end.z = pos.z + sinf(fStartAngleZ) * cosf(fEndAngleX) * fRadius;

			// 線の描画
			pLine->AddLine(start, end, CCollider::GetLineColor());
		}
	}
}

//================================================
// コンストラクタ
//================================================
CColliderFOV::CColliderFOV() : CCollider(TYPE_FOV)
{
	// メモリのクリア
	ZeroMemory(&m_Data, sizeof(m_Data));
}

//================================================
// デストラクタ
//================================================
CColliderFOV::~CColliderFOV()
{
}

//================================================
// 生成処理
//================================================
std::unique_ptr<CColliderFOV> CColliderFOV::Create(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 視界の生成
	std::unique_ptr<CColliderFOV> pFOV = std::make_unique<CColliderFOV>();

	// 設定処理
	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	return pFOV;
}

//================================================
// 生成処理(生ポインタ)
//================================================
CColliderFOV* CColliderFOV::CreateRawPtr(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 視界の生成
	CColliderFOV* pFOV = new CColliderFOV;

	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	return pFOV;
}

//================================================
// コライダーの生成
//================================================
CColliderFOV CColliderFOV::CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 視界の
	CColliderFOV FOV = {};

	// 空間情報の取得
	CTransform* pTransform = FOV.GetTransform();

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	FOV.m_Data.fAngleLeft = fAngleLeft;
	FOV.m_Data.fAngleRight = fAngleRight;
	FOV.m_Data.fLength = fLength;
	FOV.m_Data.fNowAngle = fAngle;

	// 情報の設定
	pTransform->SetInfo(info);

	return FOV;
}

//================================================
// 更新処理
//================================================
void CColliderFOV::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// 空間情報の取得
		CTransform::Info info = pTransform->GetInfo();

		// 現在の自分の向き
		m_Data.fNowAngle = info.rot.y;
	}

	// 更新処理
	CCollider::Update();
}

//================================================
// 線の描画
//================================================
void CColliderFOV::DrawLine(void)
{
	// 線の描画関数の取得
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// 取得できなかったら処理しない
	if (pLine == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	D3DXVECTOR3 pos = info.pos;
	D3DXVECTOR3 posLeft,posRight;

	// 視界の左方向を求める
	posLeft.x = pos.x + sinf(info.rot.y + m_Data.fAngleLeft) * m_Data.fLength;
	posLeft.y = pos.y;
	posLeft.z = pos.z + cosf(info.rot.y + m_Data.fAngleLeft) * m_Data.fLength;

	// 線の描画
	pLine->AddLine(pos, posLeft, { 1.0f,1.0f,1.0f,1.0f });

	// 視界の右方向を求める
	posRight.x = pos.x + sinf(info.rot.y + m_Data.fAngleRight) * m_Data.fLength;
	posRight.y = pos.y;
	posRight.z = pos.z + cosf(info.rot.y + m_Data.fAngleRight) * m_Data.fLength;

	// 線の描画
	pLine->AddLine(pos, posRight, { 1.0f,1.0f,1.0f,1.0f });

}

//================================================
// コンストラクタ
//================================================
CColliderCapsule::CColliderCapsule() : CCollider(TYPE_CAPSULE)
{
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_LocalEnd = Const::VEC3_NULL;
	m_LocalStart = Const::VEC3_NULL;
}

//================================================
// デストラクタ
//================================================
CColliderCapsule::~CColliderCapsule()
{
}

//================================================
// 生成処理
//================================================
std::unique_ptr<CColliderCapsule> CColliderCapsule::Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	// カプセルの生成
	auto pCapsule = std::make_unique<CColliderCapsule>();

	// 設定処理
	pCapsule->m_LocalStart = StartPos;
	pCapsule->m_LocalEnd = EndPos;
	pCapsule->m_Data.fRadius = fRadius;
	return pCapsule;
}

//================================================
// 生成処理(生ポインタ)
//================================================
CColliderCapsule* CColliderCapsule::CreateRawPtr(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos)
{
	// カプセルの生成
	auto pCapsule = new CColliderCapsule;

	// 設定処理
	pCapsule->m_LocalStart = StartPos;
	pCapsule->m_LocalEnd = EndPos;

	return pCapsule;
}

//================================================
// コライダーの作成処理
//================================================
CColliderCapsule CColliderCapsule::CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	CColliderCapsule capsule = {};

	// 設定処理
	capsule.m_Data.StartPos = StartPos;
	capsule.m_Data.EndPos = EndPos;
	capsule.m_Data.fRadius = fRadius;

	return capsule;
}

//================================================
// 更新処理
//================================================
void CColliderCapsule::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// 空間情報の取得
		CTransform::Info info = pTransform->GetInfo();

		// 終点と始点の空間の位置を求める
		D3DXVec3TransformCoord(&m_Data.StartPos, &m_LocalStart, &info.mtxWorld);
		D3DXVec3TransformCoord(&m_Data.EndPos, &m_LocalEnd, &info.mtxWorld);
	}

	// 更新処理
	CCollider::Update();
}

//================================================
// 線の描画処理
//================================================
void CColliderCapsule::DrawLine(void)
{
	// 線の描画関数の取得
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// 取得できなかったら処理しない
	if (pLine == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	const int nSegX = 8;
	const int nSegZ = 8;
	const float fRadius = info.fRadius;

	// 位置
	D3DXVECTOR3 pos = info.pos;

	// Zの分割分回す
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		// Xの分割分回す
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			float fRateAngleX = (D3DX_PI * 2.0f) / nSegX;

			float fAngleDestX = fRateAngleX * (nCntX + 1);
			float fAngleX = fRateAngleX * nCntX;

			D3DXVECTOR3 start, end;

			start.x = info.pos.x + sinf(fAngleX) * fRadius;
			start.y = info.pos.y + m_Data.EndPos.y - (m_Data.EndPos.y / nSegZ) * nCntZ;
			start.z = info.pos.z + cosf(fAngleX) * fRadius;

			end.x = info.pos.x + sinf(fAngleDestX) * fRadius;
			end.y = info.pos.y + m_Data.EndPos.y - (m_Data.EndPos.y / nSegZ) * nCntZ;
			end.z = info.pos.z + cosf(fAngleDestX) * fRadius;

			// 線の描画
			pLine->AddLine(start, end, Const::WHITE);
		}
	}
}

//================================================
// コンストラクタ
//================================================
CColliderOBB::CColliderOBB() : 
	CCollider(TYPE_OBB),
	m_Data()
{
}

//================================================
// デストラクタ
//================================================
CColliderOBB::~CColliderOBB()
{
}

//================================================
// 生成処理
//================================================
CColliderOBB* CColliderOBB::Create(void)
{
	CColliderOBB* pInstance = new CColliderOBB;

	//// ローカル中心座標の設定
	//pInstance->m_Data.LocalCenterPos = CenterPos;

	return pInstance;
}

//================================================
// 更新処理
//================================================
void CColliderOBB::Update(void)
{
	// データの更新処理
	UpdateData();

	// 更新処理
	CCollider::Update();
}

//================================================
// 線の描画
//================================================
void CColliderOBB::DrawLine(void)
{
	// 線の描画関数の取得
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// 取得できなかったら処理しない
	if (pLine == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 色の設定
	D3DXCOLOR col = CCollider::GetLineColor();

	D3DXVECTOR3 Size = info.Size * 0.5f;
	D3DXVECTOR3 pos = info.pos;

	D3DXVECTOR3 vtx[8] =
	{
		{ - Size.x,  - Size.y, - Size.z },
		{ + Size.x,  - Size.y, - Size.z },
		{ + Size.x,  + Size.y, - Size.z },
		{ - Size.x,  + Size.y, - Size.z },

		{ - Size.x,  - Size.y, + Size.z },
		{ + Size.x,  - Size.y, + Size.z },
		{ + Size.x,  + Size.y, + Size.z },
		{ - Size.x,  + Size.y, + Size.z },
	};

	for (auto& v : vtx)
	{
		D3DXVec3TransformCoord(&v, &v, &info.mtxWorld);
	}

	pLine->AddLine(vtx[0], vtx[1], col);
	pLine->AddLine(vtx[1], vtx[2], col);
	pLine->AddLine(vtx[2], vtx[3], col);
	pLine->AddLine(vtx[3], vtx[0], col);

	pLine->AddLine(vtx[4], vtx[5], col);
	pLine->AddLine(vtx[5], vtx[6], col);
	pLine->AddLine(vtx[6], vtx[7], col);
	pLine->AddLine(vtx[7], vtx[4], col);

	pLine->AddLine(vtx[0], vtx[4], col);
	pLine->AddLine(vtx[1], vtx[5], col);
	pLine->AddLine(vtx[2], vtx[6], col);
	pLine->AddLine(vtx[3], vtx[7], col);
}
//================================================
// データの更新処理
//================================================
void CColliderOBB::UpdateData(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return;

	// 空間情報の取得
	CTransform::Info transform = pTransform->GetInfo();

	// ワールド座標に変換
	D3DXVec3TransformCoord(&m_Data.WorldCenterPos, &m_Data.LocalCenterPos, &transform.mtxWorld);
}
