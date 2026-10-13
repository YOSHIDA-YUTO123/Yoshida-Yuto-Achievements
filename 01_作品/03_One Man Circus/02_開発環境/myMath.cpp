//**********************************************************************************************************************
// 
// 数学
// Author Tetsuji Yamamoto
// 
//**********************************************************************************************************************
#include "myMath.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include <iostream>
#include <fstream>
#include <locale.h>

//**********************************************************************************************************************
// 数学コンストラクタ
//**********************************************************************************************************************
CMyMath::CMyMath()
{
}

//**********************************************************************************************************************
// 数学デストラクタ
//**********************************************************************************************************************
CMyMath::~CMyMath()
{
}

//**********************************************************************************************************************
// 数学メッシュフィールドの縮退ポリゴンの数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekMeshFieldNumPolygonHid(int nDivisionY)
{
	return 4 * (nDivisionY - 1);
}

//**********************************************************************************************************************
// 数学メッシュフィールドの見えるポリゴンの数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumPolygonApp(int nDivisionX, int nDivisionY)
{
	return (nDivisionX * nDivisionY) * 2;
}

//**********************************************************************************************************************
// 数学メッシュフィールドのすべてのポリゴン数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumPolygonAll(int nDivisionX, int nDivisionY)
{
	return ((nDivisionX * nDivisionY) * 2) + (4 * (nDivisionY - 1));
}

//**********************************************************************************************************************
// 数学メッシュフィールドの頂点数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumVertexAll(int nDivisionX, int nDivisionY)
{
	return (nDivisionX + 1) * (nDivisionY + 1);
}

//**********************************************************************************************************************
// 数学メッシュフィールドのインデックス数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFieldNumIndexAll(int nNumPolygonAll)
{
	return nNumPolygonAll + 2;
}

//**********************************************************************************************************************
// 数学ファンのインデックス数を求める処理
//**********************************************************************************************************************
int CMyMath::SeekFanNumIndexAll(int nDivisionX)
{
	return nDivisionX + 3;
}

//************************************************************************************************************************
// ローカル座標から回転行列に変換する処理
//************************************************************************************************************************
D3DXMATRIX CMyMath::ConvertLocalCoordinatesToRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 nor)
{
	D3DXMATRIX mtxResult;	// 計算結果保存用変数
	D3DXVECTOR3 X, Y, Z;	// 計算したベクトル保存用変数

	Z = -(look - pos);										// 位置から見ている方向のベクトルを求める
	D3DXVec3Normalize(&Z, &Z);								// 標準化する
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &nor), &Z);		// x軸を求める
	D3DXVec3Normalize(&X, &X);								// 標準化する
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));		// y軸を求める

	// 求めた値を回転行列に変換する
	mtxResult._11 = X.x; mtxResult._12 = X.y; mtxResult._13 = X.z; mtxResult._14 = 0;
	mtxResult._21 = Y.x; mtxResult._22 = Y.y; mtxResult._23 = Y.z; mtxResult._24 = 0;
	mtxResult._31 = Z.x; mtxResult._32 = Z.y; mtxResult._33 = Z.z; mtxResult._34 = 0;
	mtxResult._41 = 0.0f; mtxResult._42 = 0.0f; mtxResult._43 = 0.0f; mtxResult._44 = 1.0f;

	// 結果を返す
	return mtxResult;
}

//************************************************************************************************************************
// 角度の正規化
//************************************************************************************************************************
void CMyMath::NormalizingAngles(D3DXVECTOR3* pRot)
{
	// 角度の正規化
	if (pRot->x > D3DX_PI)
	{
		pRot->x = -D3DX_PI + (pRot->x - D3DX_PI);
	}
	else if (pRot->x < -D3DX_PI)
	{
		pRot->x = D3DX_PI + (pRot->x + D3DX_PI);
	}

	if (pRot->y > D3DX_PI)
	{
		pRot->y = -D3DX_PI + (pRot->y - D3DX_PI);
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->y = D3DX_PI + (pRot->y + D3DX_PI);
	}

	if (pRot->z > D3DX_PI)
	{
		pRot->z = -D3DX_PI + (pRot->z - D3DX_PI);
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->z = D3DX_PI + (pRot->z + D3DX_PI);
	}
}

//************************************************************************************************************************
// 角度の正規化
//************************************************************************************************************************
float CMyMath::NormalizingAngles(float fAngle)
{
	// 角度の正規化
	if (fAngle > D3DX_PI)
	{
		fAngle = -D3DX_PI + (fAngle - D3DX_PI);
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle = D3DX_PI + (fAngle + D3DX_PI);
	}

	return fAngle;
}

//**************************************************
// 最短の角度に変換する処理
//**************************************************
float CMyMath::ShortestAngle(float fAngleDiff)
{
	// 差分が角度一周分を超えていたら差分から一周を引いて次に足せばいい角度を求める
	if (fAngleDiff > D3DX_PI)
	{
		fAngleDiff = fAngleDiff - D3DX_PI * 2;
	}
	else if (fAngleDiff < -D3DX_PI)
	{
		fAngleDiff = fAngleDiff + D3DX_PI * 2;
	}

	// 結果を返す
	return fAngleDiff;
}

//**************************************************
// 最短の角度に変換する処理
//**************************************************
float CMyMath::ShortestAngle(float AngleDest, float Angle)
{
	float fAngleDiff = AngleDest - Angle;

	// 差分が角度一周分を超えていたら差分から一周を引いて次に足して角度を求める
	if (fAngleDiff > D3DX_PI)
	{
		fAngleDiff = fAngleDiff - D3DX_PI * 2;
	}
	else if (fAngleDiff < -D3DX_PI)
	{
		fAngleDiff = fAngleDiff + D3DX_PI * 2;
	}

	// 結果を返す
	return fAngleDiff;
}

//**************************************************
// 3D座標から2D座標に変換処理
//**************************************************
D3DXVECTOR3 CMyMath::Posision3Dto2D(D3DXVECTOR3* pOutPos2D, D3DXVECTOR3 pos3D)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 PosScreen = {};		// 出力先
	D3DVIEWPORT9 viewport = {};		// ビューポート
	D3DXMATRIX projectionMtx = {};	// プロジェクションマトリックス
	D3DXMATRIX viewMtx = {};		// ビューマトリックス
	D3DXMATRIX worldMtx = {};		// ワールドマトリックス

	pDevice->GetViewport(&viewport);							// ビューポート取得
	pDevice->GetTransform(D3DTS_PROJECTION, &projectionMtx);	// プロジェクションマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &viewMtx);				// ビューマトリックス取得
	D3DXMatrixIdentity(&worldMtx);								// ワールドマトリックスの初期化

	// 変換する
	D3DXVec3Project(
		&PosScreen,		// 出力先のスクリーン座標
		&pos3D,			// 変換したいワールド座標
		&viewport,		// ビューポート情報
		&projectionMtx,	// プロジェクションマトリックス
		&viewMtx,		// ビューマトリックス
		&worldMtx		// ワールドマトリックス
	);

	// 結果を代入
	*pOutPos2D = PosScreen;

	// 結果を返す
	return PosScreen;
}

//**************************************************
// スクリーン座標から3D座標に変換処理
//**************************************************
D3DXVECTOR3 CMyMath::Posision2Dto3D(D3DXVECTOR3* pOutPos3D, D3DXVECTOR3 pos2D)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 out;
	D3DVIEWPORT9 vp;
	D3DXMATRIX view;
	D3DXMATRIX proj;

	// ビューポート行列を作成
	D3DXMATRIX vpMat;
	D3DXMatrixIdentity(&vpMat);

	pDevice->GetViewport(&vp);						// ビューポート取得
	pDevice->GetTransform(D3DTS_VIEW, &view);		// ビュー行列取得
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);	// プロジェクションマトリックス取得

	vpMat._11 = (float)vp.Width / 2;
	vpMat._22 = -1.0f * (float)(vp.Height / 2);
	vpMat._33 = (float)vp.MaxZ - vp.MinZ;
	vpMat._41 = (float)(vp.X + vp.Width / 2);
	vpMat._42 = (float)(vp.Y + vp.Height / 2);
	vpMat._43 = vp.MinZ;

	// スクリーン位置をワールドへ
	out.x = pos2D.x;
	out.y = pos2D.y;
	out.z = pos2D.z;

	D3DXMATRIX invMat, inv_proj, inv_view;
	D3DXMatrixInverse(&invMat, 0, &vpMat);
	D3DXMatrixInverse(&inv_proj, 0, &proj);
	D3DXMatrixInverse(&inv_view, 0, &view);

	invMat *= inv_proj * inv_view;

	D3DXVec3TransformCoord(&out, &out, &invMat);

	*pOutPos3D = out;
	return out;
}

//**************************************************
// 三角関数で点と点の長さを図る処理2D
//**************************************************
float CMyMath::LengthPositions2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	return sqrtf((fX * fX) + (fY * fY));
}

//**************************************************
// 点と点の長さを図る処理3D
//**************************************************
float CMyMath::LengthPositions3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	float fX = posA.x - posB.x;
	float fY = posA.y - posB.y;
	float fZ = posA.z - posB.z;
	return sqrtf((fX * fX) + (fY * fY) + (fZ * fZ));
}

//**************************************************
// 円と円の当たり判定2D
//**************************************************
bool CMyMath::IsColiisionRectTopoint2D(D3DXVECTOR2 posPoint, D3DXVECTOR2 posRect, float fWidth, float fHeight)
{
	/*
	自身の位置が相手の上辺より大きく
	自身の位置が相手の底辺より小さく
	自身の位置が相手の左辺より大きく
	自身の位置が相手の右辺より小さく
	 */
	if (posRect.y - fHeight < posPoint.y &&
		posRect.y + fHeight > posPoint.y &&
		posRect.x - fWidth < posPoint.x &&
		posRect.x + fWidth > posPoint.x
		)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 円と円の当たり判定2D
//**************************************************
bool CMyMath::IsColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions2D(posA, posB);

	// 限界の長さより短ければ
	if (fLength <= fLimitLength)
	{
		// 当たっている判定
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 球と球の当たり判定
//**************************************************
bool CMyMath::IsColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB)
{
	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions3D(posA, posB);

	// 限界の長さより短ければ
	if (fLength <= fLimitLength)
	{
		// 当たっている判定
		return true;
	}

	// 当たっていない
	return false;
}

//**************************************************
// 円柱と円柱の当たり判定
//**************************************************
bool CMyMath::IsColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB)
{
	/*
			 |---|		|---|
			 |   |		|   |
		|---||   |		|   ||---|
		|   ||---|		|---||   |
		|   |				 |   |
		|---|				 |---|
		my   par		 my   par
	*/

	// 自身の底辺が相手の底辺より小さい、かつ自身の上辺が相手の底辺より高い
	if (posA.y <= posB.y && posA.y + fHeightA >= posB.y)
	{

	}
	// 自身の底辺が相手の底辺より大きい、かつ自身の底辺が相手の上辺より低い
	else if (posA.y >= posB.y && posA.y <= posB.y + fHeightB)
	{

	}
	// どれにも当てはまらない
	else
	{
		return false;
	}

	// D3DXVECTOR3からD3DXVECTOR2へ移し替える
	D3DXVECTOR2 pos2D, pos2DPar;
	pos2D.x = posA.x;
	pos2D.y = posA.z;
	pos2DPar.x = posB.x;
	pos2DPar.y = posB.z;

	// 限界の長さ
	float fLimitLength = fRadiusA + fRadiusB;

	// 点と点の長さを図る
	float fLength = CMyMath::LengthPositions2D(pos2D, pos2DPar);

	// 限界の長さより短ければ当たっている判定
	if (fLength <= fLimitLength)
	{
		return true;
	}

	// 当たっていない判定
	return false;
}

//**************************************************
// 一文字抜き取る処理
//**************************************************
int CMyMath::PickOutALetter(char* pOut, int nCharNum, const char* pStr)
{
	// マルチバイト文字系関数を使用するための設定
	setlocale(LC_ALL, "");

	// 文字列の長さを調べる
	int nLength = strlen(pStr);

	// 計算用変数
	int nData = 0;
	char cDataA = 0;

	// 文字を生成
	for (int nCntLen = 0; nCntLen < nLength; nCntLen++)
	{
		cDataA = pStr[nCntLen];

		// 目的の文字に達していなければ処理しない
		if (nCntLen < nCharNum)
		{
			continue;
		}

		// データがマイナス、1バイト目ならばもう一度検査
		if (cDataA < 0 && nData <= 0)
		{
			pOut[nData] = cDataA;
			nData++;
			continue;
		}
		// 2バイト目ならば処理を終わる
		else
		{
			pOut[nData] = cDataA;
			return nCntLen;
		}
	}

	return 0;
}

//**************************************************b
// フラグを立てる処理
//**************************************************
char CMyMath::BitFlagRaiseAFlag(char cVariable, char cFlagValue)
{
	return cVariable | cFlagValue;
}

//**************************************************
// フラグを下げる処理
//**************************************************
char CMyMath::BitFlagLowerFlag(char cVariable, char cFlagValue)
{
	return (cVariable & cFlagValue) ? cVariable ^ cFlagValue : cVariable;
}

//**************************************************
// フラグが立っているか確認する
//**************************************************
bool CMyMath::BitFlagIsSet(char cVariable, char cFlagCheck)
{
	return (cVariable & cFlagCheck) ? true : false;
}

//**************************************************
// ベクトルのなす角を求める処理(ラジアン角)
//**************************************************
float CMyMath::VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
{
	// ベクトルを正規化
	D3DXVec3Normalize(&vecA, &vecA);
	D3DXVec3Normalize(&vecB, &vecB);

	// 内積を求めて
	float fDot = D3DXVec3Dot(&vecA, &vecB);

	// ラジアン角に変換
	return (float)acos(fDot);
}

//**************************************************
// ベクトルのなす角を求める処理(ラジアン角)
//**************************************************
float CMyMath::VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB)
{
	D3DXVECTOR2 vecAB = vecA - vecB;
	return atan2f(vecAB.x, vecAB.y);
}

//**************************************************
// 位置と角度からワールドマトリックスを作る処理
//**************************************************
D3DXMATRIX CMyMath::D3DXMatrixMultiplyYawPitchRollTranslation(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxWorld;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを追加
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を追加
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	return mtxWorld;
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSort(int* SortData, int nNum)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < nNum; nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < nNum - nCntRankA; nCntRankB++)
		{
			// 数字を比較大きければ入れ替える
			int nNumA = SortData[nCntRankB];
			int nNumB = SortData[nCntRankB + 1];
			if (nNumA < nNumB)
			{
				SortData[nCntRankB + 1] = nNumA;
				SortData[nCntRankB] = nNumB;
			}
		}
	}
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSort(std::vector<int>& vrTarget)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < (int)vrTarget.size(); nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < (int)vrTarget.size() - nCntRankA - 1; nCntRankB++)
		{
			// 数字を比較大きければ入れ替える
			auto iterNumA = vrTarget.begin() + nCntRankB;
			auto iterNumB = vrTarget.begin() + nCntRankB + 1;
			if ((*iterNumA) < (*iterNumB))
			{
				int nData = (*iterNumB);
				(*iterNumB) = (*iterNumA);
				(*iterNumA) = nData;
			}
		}
	}
}

//**********************************************************************************************************************
// バブルソート処理
//**********************************************************************************************************************
void CMyMath::BubbleSortMin(int* SortData, int nNum)
{
	// バブルソートで並べなおす
	for (int nCntRankA = 0; nCntRankA < nNum; nCntRankA++)
	{
		for (int nCntRankB = 0; nCntRankB < nNum - nCntRankA; nCntRankB++)
		{
			// 数字を比較小さければ入れ替える
			int nNumA = SortData[nCntRankB];
			int nNumB = SortData[nCntRankB + 1];
			if (nNumA > nNumB)
			{
				SortData[nCntRankB + 1] = nNumA;
				SortData[nCntRankB] = nNumB;
			}
		}
	}
}

//**************************************************
// クォータニオンからオイラー角への変換
//**************************************************
D3DXVECTOR3 CMyMath::QuaternionToEuler(const D3DXQUATERNION& quat)
{
	// オイラー角
	D3DXVECTOR3 euler;

	// X軸
	float sinr_cosp = 2.0f * (quat.w * quat.x + quat.y * quat.z);
	float cosr_cosp = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);
	euler.x = (float)atan2(sinr_cosp, cosr_cosp);

	// Y軸
	float sinp = 2.0f * (quat.w * quat.y - quat.z * quat.x);
	if (fabs(sinp) >= 1)
	{
		euler.y = (float)copysign(D3DX_PI / 2.0f, sinp); // 90度にクランプ
	}
	else
	{
		euler.y = (float)asin(sinp);
	}

	// Z軸
	float siny_cosp = 2.0f * (quat.w * quat.z + quat.x * quat.y);
	float cosy_cosp = 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z);
	euler.z = (float)atan2(siny_cosp, cosy_cosp);

	return euler;
}

//**************************************************
// 位置に回転マトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MulRotationMatrix(D3DXVECTOR3 pos,D3DXMATRIX mtxRot)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtxRot._11) + (pos.y * mtxRot._21) + (pos.z * mtxRot._31);
	posOut.y = (pos.x * mtxRot._12) + (pos.y * mtxRot._22) + (pos.z * mtxRot._32);
	posOut.z = (pos.x * mtxRot._13) + (pos.y * mtxRot._23) + (pos.z * mtxRot._33);
	return posOut;
}

//**************************************************
// 位置に回転マトリックスを掛ける処理
//**************************************************
D3DXVECTOR3 CMyMath::MulWorldMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxWorld)
{
	D3DXVECTOR3 posOut;
	posOut.x = (pos.x * mtxWorld._11) + (pos.y * mtxWorld._21) + (pos.z * mtxWorld._31) + mtxWorld._41;
	posOut.y = (pos.x * mtxWorld._12) + (pos.y * mtxWorld._22) + (pos.z * mtxWorld._32) + mtxWorld._42;
	posOut.z = (pos.x * mtxWorld._13) + (pos.y * mtxWorld._23) + (pos.z * mtxWorld._33) + mtxWorld._43;
	return posOut;
}

//**************************************************
// クォータニオンで回転行列を求める処理
//**************************************************
D3DXMATRIX CMyMath::QuatnionToMatrix(D3DXVECTOR3 centralAxis, D3DXVECTOR3 clossAxis, D3DXVECTOR3 angleAxis)
{
	D3DXMATRIX mtxRot;
	D3DXQUATERNION quat;
	D3DXVECTOR3 vecRotationAxis;
	float fAngle;

	fAngle = CMyMath::VectorAngleToRadian(centralAxis, angleAxis);
	D3DXVec3Cross(&vecRotationAxis,&centralAxis, &clossAxis);
	D3DXQuaternionRotationAxis(&quat, &vecRotationAxis,fAngle);
	D3DXMatrixRotationQuaternion(&mtxRot ,&quat);
	return mtxRot;
}

//**************************************************
// ランダムで小数点を出す処理
//**************************************************
float CMyMath::RandamDecimal(float fMax)
{
	int nMax = (int)fMax;
	return (float)(rand() % nMax);
}

//**************************************************
// ランダムで小数点を出す処理
//**************************************************
float CMyMath::RandamDecimal(float fMax, float fMin)
{
	int nMax = (int)(fMax * 1000.0f);
	int nMin = (int)(fMin * 1000.0f);
	return (float)(rand() % nMax + nMin) * 0.001f;
}

//**************************************************
// ランダムで角度を出す処理
//**************************************************
float CMyMath::RandamAngle()
{
	return (float)(rand() % 628 - 314) * 0.01f;
}

//**************************************************
// sinfで値を求める関数
//**************************************************
float CMyMath::Sinf(float fAngle,float fLength)
{
	return sinf(fAngle) * fLength;
}

//**************************************************
// cosfで値を求める関数
//**************************************************
float CMyMath::Cosf(float fAngle, float fLength)
{
	return cosf(fAngle) * fLength;
}

//**************************************************
// 球座標で位置を求める関数 
//**************************************************
D3DXVECTOR3 CMyMath::SphericalCoordinates(D3DXVECTOR3 posCenter,D3DXVECTOR3 rot,float fDistance)
{
	D3DXVECTOR3 pos;
	pos.x = posCenter.x + sinf(rot.x) * sinf(rot.y) * fDistance;
	pos.y = posCenter.y + cosf(rot.x) * fDistance;
	pos.z = posCenter.z + sinf(rot.x) * cosf(rot.y) * fDistance;

	return pos;
}

//**************************************************
// カウントダウン処理 
//**************************************************
bool CMyMath::CountDown(int* pData, int nMin)
{
	// 値を減らす
	*pData -= 1;

	// 最小値の値になったら止める
	if (*pData <= nMin)
	{
		*pData = nMin;
		return true;
	}

	return false;
}

//**************************************************
// 文字列を空白まで読み取る処理
//**************************************************
void CMyMath::ReadToSpace(std::ifstream& ifs, std::string& str)
{
	// 一文字保存用
	char cData;

	// 最初に読み取った空白
	bool bFarstSpace = false;

	// 文字列を削除
	str.clear();

	// 一文字ずつ読み取る
	while (ifs.get(cData))
	{
		// 空白だったら
		if (cData == ' ')
		{
			if (bFarstSpace == false)
			{
				bFarstSpace = true;
			}
			else
			{
				return;
			}
		}
		// 改行だったら
		else if (cData == '\n')
		{
			return;
		}
		// タブ文字だったら
		else if (cData == '\t')
		{
			return;
		}
		// キャリッジリターンだったら
		else if (cData == '\r')
		{
			return;
		}
		// 当てはまらないので文字列追加
		else
		{
			// 文字列追加
			str += cData;
		}

		// スペースの読み取りを不可にする
		bFarstSpace = true;
	}
}

//**************************************************
// クォータニオンの球面線形補間処理
//**************************************************
void CMyMath::SlerpQuaternion(D3DXQUATERNION* pOut, D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric)
{
	float fDotQuat = D3DXQuaternionDot(&quatNext, &quatNow);
	float fCosTheta = acosf(fDotQuat);

	// 近いほうから回るようにする
	if (fCosTheta < 0.0f)
	{
		fCosTheta = -fCosTheta;
	}

	float fSin = sinf(fCosTheta);

	// ゼロ以下だった場合そのまま返す
	if (fSin <= FLT_EPSILON)
	{
		*pOut = quatNow;
		return;
	}

	float fSinA = sinf(fCosTheta * (1.0f - fParametric));
	float fSinB = sinf(fCosTheta * fParametric);

	float fSinMugA = fSinA / fSin;
	float fSinMugB = fSinB / fSin;

	pOut->x = quatNow.x * fSinMugA + quatNext.x * fSinMugB;
	pOut->y = quatNow.y * fSinMugA + quatNext.y * fSinMugB;
	pOut->z = quatNow.z * fSinMugA + quatNext.z * fSinMugB;
	pOut->w = quatNow.w * fSinMugA + quatNext.w * fSinMugB;
}

//**************************************************
// メッシュでの当たり判定処理
//**************************************************
bool CMyMath::CollisionMesh(LPD3DXMESH pMesh,D3DXMATRIX mtxMesh, D3DXVECTOR3* pOut, D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
	// 頂点フォーマットのサイズを取得
	WORD sizeFVF = (WORD)D3DXGetFVFVertexSize(pMesh->GetFVF());
	
	// インデックスバッファ取得
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;
	pMesh->GetIndexBuffer(&pIdxBuff);
	
	// インデックスバッファの情報取得
	D3DINDEXBUFFER_DESC descIdx;
	pIdxBuff->GetDesc(&descIdx);

	// インデックス数を割り出す
	int nIdxNum = 0;
	if (descIdx.Format == D3DFMT_INDEX16) {
		nIdxNum = (descIdx.Size / sizeof(WORD)); // 16ビットインデックスの場合
	}
	else if (descIdx.Format == D3DFMT_INDEX32) {
		nIdxNum = (descIdx.Size / sizeof(DWORD)); // 32ビットインデックスの場合
	}

	// 頂点バッファのロック
	BYTE* pVtxBuff = NULL;
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// インデックスバッファのロック
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdxBuff);
	WORD* pIdx = (WORD*)pIdxBuff;

	// 出力先を今の位置に設定
	*pOut = pos;

	// モデルの頂点数分回す
	for (int nCntIdx = 0; nCntIdx < nIdxNum;)
	{
		D3DXVECTOR3 aPos[3];			// ポリゴンの頂点座標
		D3DXVECTOR3 nor;				// ポリゴンの法線
		D3DXVECTOR3 aVecPolyLine[3];	// ポリゴンの一辺のベクトル
		D3DXVECTOR3 aVecLinePos[3];		// posとのベクトル
		D3DXVECTOR3 aVecLinePosOld[3];	// posOldとのベクトル
		D3DXVECTOR3 aVecHorizontal[3];	// ポリゴンと水平のベクトル

		int aIdx[3];

		// 三回回す
		for (int nCntTriAngle = 0; nCntTriAngle < 3; nCntTriAngle++, nCntIdx++)
		{
			// インデックス取得
			int nIdx = pIdx[nCntIdx];
			aIdx[nCntTriAngle] = nIdx;

			// 頂点座標の代入
			D3DXVECTOR3* pPosVtx = (D3DXVECTOR3*)(pVtxBuff + sizeFVF * nIdx);
			
			// 座標にマトリックスを掛けて代入
			aPos[nCntTriAngle] = MulWorldMatrix(*pPosVtx,mtxMesh);
		}

		// ポリゴンの一辺のベクトルを求める
		aVecPolyLine[0] = aPos[1] - aPos[0];
		aVecPolyLine[1] = aPos[2] - aPos[1];
		aVecPolyLine[2] = aPos[0] - aPos[2];

		// 外積で法線を求める
		D3DXVec3Cross(&nor, &aVecPolyLine[0], &aVecPolyLine[3]);
		D3DXVec3Normalize(&nor, &nor);

		// 三回回す
		for (int nCntTriAngle = 0; nCntTriAngle < 3; nCntTriAngle++)
		{
			// pos,posOldベクトルを求める
			aVecLinePos[nCntTriAngle] = pos - aPos[nCntTriAngle];
			aVecLinePosOld[nCntTriAngle] = posOld - aPos[nCntTriAngle];
		}

		D3DXVECTOR3 vecMove = pos - posOld;
		float fLengthVecMove = D3DXVec3Length(&vecMove);

		// 位置と古い位置のポリゴンまでの距離を求める
		float fDotLengthPos = D3DXVec3Dot(&nor,&aVecLinePos[0]);
		float fDotLengthPosOld = D3DXVec3Dot(&nor, &aVecLinePosOld[0]);
		float fDotLengthOll = fDotLengthPos + fDotLengthPosOld;

		if (fDotLengthPosOld >= fLengthVecMove)
		{
			continue;
		}

		D3DXVECTOR3 vecMoveSecret = -vecMove * (fDotLengthPos / fDotLengthOll);

		*pOut = pos + vecMoveSecret;

		// 頂点バッファのアンロック
		pMesh->UnlockVertexBuffer();

		// インデックスバッファのアンロック
		pMesh->UnlockIndexBuffer();

		return true;
	}

	// 頂点バッファのアンロック
	pMesh->UnlockVertexBuffer();

	// インデックスバッファのアンロック
	pMesh->UnlockIndexBuffer();

	return false;
}

//**************************************************
// 弧度法を度数法になおす処理
//**************************************************
float CMyMath::RadianToDegree(float radian)
{
	return radian * (180.0f / D3DX_PI);
}

//**************************************************
// 度数法を弧度法になおす処理
//**************************************************
float CMyMath::DegreeToRadian(float fDegree)
{
	return fDegree * (D3DX_PI / 180.0f);
}