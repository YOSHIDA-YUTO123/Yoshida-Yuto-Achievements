//================================================
//
// タイマーを表示する処理(基底クラス) [timer.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "object.h"
#include <memory>

//*************************************************
// 前方宣言
//*************************************************
class CNumber;

//*************************************************
// タイマークラスの定義
//*************************************************
class CTimer : public CObject
{
public:
	static constexpr int MAX_DIGIT = 2;				// 最大の桁数

	CTimer(int nPriority = CObject::PRIORITY_UI_FRONT);
	~CTimer();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	void SetTexture(const char* pTexturePath);

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR2 GetSize(void) const { return m_Size; }
	void SetTime(const int nTime) { m_nTime = nTime; }

	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(const D3DXVECTOR2 Size) { m_Size = Size; }
	int GetTime(void) const { return m_nTime; }

	static int Load(const char* pFilename);

	CNumber* GetNumber(const int nIdx);
private:

	std::unique_ptr<CNumber> m_apNumber[MAX_DIGIT]; // ナンバークラスの定義
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR2 m_Size;								// 大きさ
	int m_nTime;									// 時間
};

#endif
