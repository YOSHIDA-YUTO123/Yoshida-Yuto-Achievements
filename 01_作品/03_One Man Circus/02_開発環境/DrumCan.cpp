//==================================================
// 
// ドラム缶 [DramCan.cpp]
// Author：Rio Ohno
// 
//==================================================

// インクルード
#include "DrumCan.h"
#include "myMath.h"

//==================================================
// コンストラクタ
//==================================================
CDramcan::CDramcan()
{
	m_posLocal = Const::VEC3_NULL;			// ローカル位置
	m_posLocalToWorld = Const::VEC3_NULL;	// ローカル位置をワールド座標に変換した値
	m_move = Const::VEC3_NULL;				// 移動量
	m_fRadius = 0.0f;						// 半径
	m_bMove = false;						// 移動の有無
}

//==================================================
// デストラクタ
//==================================================
CDramcan::~CDramcan()
{
	// なし
}

//==================================================
// 生成処理
//==================================================
CDramcan* CDramcan::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CDramcan* pDramcan = nullptr;

	// メモリの確保
	pDramcan = new CDramcan;

	// モデルのロードに失敗したら
	if (FAILED(pDramcan->LoadModel(DRAMCAN_MODEL_PASS)))
	{
		// 破棄
		pDramcan->Uninit();
		pDramcan = nullptr;
		return nullptr;
	}

	// 要素の設定
	pDramcan->SetPosition(pos);
	pDramcan->SetRotation(rot);

	// 初期化処理
	pDramcan->Init();

	return pDramcan;
}

//==================================================
// 初期化処理
//==================================================
HRESULT CDramcan::Init(void)
{
	// 半径の取得
	m_fRadius = CObjectX::GetSize().y * 0.5f;

	return S_OK;
}

//==================================================
// 終了処理
//==================================================
void CDramcan::Uninit(void)
{
	// 破棄
	CObjectX::Uninit();
}

//==================================================
// 更新処理
//==================================================
void CDramcan::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 posMy = CObjectX::GetPosition();

	// 慣性をつける
	m_move.x -= m_move.x * 0.1f;
	m_move.z -= m_move.z * 0.1f;

	// 位置に移動量追加
	posMy += m_move;

	// 位置の更新
	CObjectX::SetPosition(posMy);
}

//==================================================
// 描画処理
//==================================================
void CDramcan::Draw(void)
{
	// 破棄
	CObjectX::Draw();
}

//==================================================
// 移動
//==================================================
void CDramcan::isMove(D3DXVECTOR3 move)
{
	D3DXVECTOR3 rotation = move / m_fRadius;
	m_move = move;

	// 現在位置の取得
	D3DXVECTOR3 L_pos = CObjectX::GetPosition();

	// 現在の向きの取得
	D3DXVECTOR3 L_rot = CObjectX::GetRotation();

	// 位置の更新
	CObjectX::SetPosition(L_pos + m_move);

	// 向きの更新
	L_rot.x += rotation.z;
	L_rot.y += rotation.y;
	L_rot.z += rotation.x;

	// 向きの設定
	CObjectX::SetRotation(L_rot);
}

//==================================================
// 当たり判定処理
//==================================================
bool CDramcan::Collison(D3DXVECTOR3* pOutPos, D3DXVECTOR3 pos)
{
	// 位置取得,最大値,最小値取得
	D3DXVECTOR3 posMy = CObjectX::GetPosition();
	D3DXVECTOR3 Size = CObjectX::GetSize();

	D3DXVECTOR2 posMy2D = { posMy.x, posMy.y };
	D3DXVECTOR2 pos2D = { pos.x, pos.y };

	// 半径計算
	float fRadius = Size.y * 0.5f;
	float fDips = Size.z * 0.5f;

	// 奥と横の位置を計算
	float fZBack = posMy.z + fDips;
	float fZFront = posMy.z - fDips;
	float fXRight = posMy.x + fRadius;
	float fXLeft = posMy.x - fRadius;

	// 奥行きの範囲内にいるか調べる
	if (!(pos.z <= fZBack && pos.z >= fZFront))
	{
		// 当たってない
		return false;
	}

	// 幅の範囲内にいるか調べる
	if (!(pos.x >= fXLeft && pos.x <= fXRight))
	{
		// 当たってない
		return false;
	}

	// XY平面で判定する
	if (CMyMath::IsColiisionCircleAndCircle2D(posMy2D, fRadius, pos2D, 0.0f) == false)
	{
		// 当たってない
		return false;
	}

	// 位置と一の距離を図る
	D3DXVECTOR2 vecPos;
	vecPos.x = pos.x - posMy.x;
	vecPos.y = pos.y - posMy.y;
	float fLength = D3DXVec2Length(&vecPos);

	// 距離係数を求める
	float fPalamLength = 1.0f - fLength / fRadius;

	// 押し出す
	pOutPos->x = pos.x;
	pOutPos->z = pos.z;
	pOutPos->y = posMy.y + fRadius * fPalamLength;

	// 当たった
	return true;
}

//==================================================
// 当たり判定処理
//==================================================
void CDramcan::SetMove(D3DXVECTOR3 rot, float fMoveMent)
{
	// 角度係数
	float fPlamRot = rot.x / D3DX_PI * 0.5f;

	float fLotY = rot.y;

	if (rot.y != 0.0f)
	{
		// 角度を横だけにする
		fLotY = (D3DX_PI * 0.5f) * (rot.y / fabsf(rot.y));
	}

	// 移動量追加
	m_move.x += sinf(fLotY) * fMoveMent + (fMoveMent * -fPlamRot);
	m_move.z += cosf(fLotY) * fMoveMent + (fMoveMent * -fPlamRot);
	m_move.y = 0.0f;

	// 動く状態にする
	m_bMove = true;
}