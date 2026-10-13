//*********************************************
// 
// 数学
// Author Tetsuji Yamamoto
// 
//*********************************************
#ifndef _MYMATH_H_
#define _MYMATH_H_

#include "main.h"

// 数学クラス
class CMyMath
{
public:
	CMyMath();
	~CMyMath();

	static int SeekMeshFieldNumPolygonHid(int nDivisionY);
	static int SeekFieldNumPolygonApp(int nDivisionX, int nDivisionY);
	static int SeekFieldNumPolygonAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumVertexAll(int nDivisionX, int nDivisionY);
	static int SeekFieldNumIndexAll(int nNumPolygonAll);
	static int SeekFanNumIndexAll(int nDivisionX);
	static D3DXMATRIX ConvertLocalCoordinatesToRotMtx(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 nor);
	static void NormalizingAngles(D3DXVECTOR3* pRot);
	static float NormalizingAngles(float fAngle);
	static float ShortestAngle(float fAngleDiff);
	static float ShortestAngle(float AngleDest, float Angle);
	static D3DXVECTOR3 Posision3Dto2D(D3DXVECTOR3* pOutPos2D, D3DXVECTOR3 pos3D);
	static D3DXVECTOR3 Posision2Dto3D(D3DXVECTOR3* pOutPos3D, D3DXVECTOR3 pos2D);
	static float LengthPositions2D(D3DXVECTOR2 posA, D3DXVECTOR2 posB);
	static float LengthPositions3D(D3DXVECTOR3 posA, D3DXVECTOR3 posB);
	static bool IsColiisionRectTopoint2D(D3DXVECTOR2 posPoint, D3DXVECTOR2 posRect, float fWidth, float fHeight);
	static bool IsColiisionCircleAndCircle2D(D3DXVECTOR2 posA, float fRadiusA, D3DXVECTOR2 posB, float fRadiusB);
	static bool IsColiisionSpherAndSpher(D3DXVECTOR3 posA, float fRadiusA, D3DXVECTOR3 posB, float fRadiusB);
	static bool IsColiisionCylinderAndCylinder(D3DXVECTOR3 posA, float fRadiusA, float fHeightA, D3DXVECTOR3 posB, float fRadiusB, float fHeightB);
	static int PickOutALetter(char* pOut, int nCharNum, const char* pStr);
	static char BitFlagRaiseAFlag(char cVariable, char cFlagValue);
	static char BitFlagLowerFlag(char cVariable, char cFlagValue);
	static bool BitFlagIsSet(char cVariable, char cFlagCheck);
	static float VectorAngleToRadian(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);
	static float VectorsAngle2D(D3DXVECTOR2 vecA, D3DXVECTOR2 vecB);
	static D3DXMATRIX D3DXMatrixMultiplyYawPitchRollTranslation(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void BubbleSort(int* SortData, int nNum);
	static void BubbleSort(std::vector<int>& vrTarget);
	static void BubbleSortMin(int* SortData, int nNum);
	static D3DXVECTOR3 QuaternionToEuler(const D3DXQUATERNION& quat);
	static D3DXVECTOR3 MulRotationMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxRot);
	static D3DXVECTOR3 MulWorldMatrix(D3DXVECTOR3 pos, D3DXMATRIX mtxWorld);
	static D3DXMATRIX QuatnionToMatrix(D3DXVECTOR3 centralAxis, D3DXVECTOR3 clossAxis,D3DXVECTOR3 angleAxis);
	static float RandamDecimal(float fMax);
	static float RandamAngle(void);
	static float RandamDecimal(float fMax, float fMin);
	static float Sinf(float fAngle, float fLength);
	static float Cosf(float fAngle, float fLength);
	static D3DXVECTOR3 SphericalCoordinates(D3DXVECTOR3 posCenter, D3DXVECTOR3 rot, float fDistance);
	static bool CountDown(int* pData,int nMin);
	static void ReadToSpace(std::ifstream& ifs, std::string& str);
	static void SlerpQuaternion(D3DXQUATERNION* pOut, D3DXQUATERNION quatNow, D3DXQUATERNION quatNext, float fParametric);
	static bool CollisionMesh(LPD3DXMESH pMesh, D3DXMATRIX mtxMesh, D3DXVECTOR3* pOut,D3DXVECTOR3 pos, D3DXVECTOR3 posOld);
	static float DegreeToRadian(float fDegree);
	static float RadianToDegree(float radian);

private:
};

//**********************************************************************************************************************
// クランプ せき止める処理
//**********************************************************************************************************************
template <class T> T Clamp(const T t, const T min, const T max)
{
	if (t > max) return max;
	if (t < min) return min;
	return t;
}

//**********************************************************************************************************************
// ラップ 繰り返す処理
//**********************************************************************************************************************
template <class T> T Wrap(const T t, const T min, const T max)
{
	if (t > max) return min;
	if (t < min) return max;
	return t;
}
#endif // !_MATH_H_
