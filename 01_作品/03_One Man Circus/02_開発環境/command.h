//==================================================
// 
// コマンド [command.h]
// Author：Rio Ohno
// 
//==================================================

#ifndef _COMMAND_H_
#define _COMMAND_H_

// インクルード
#include "main.h"
#include "object2D.h"

// マクロ定義
#define COM_SECONDS (10)
#define NUM_BOTTON (3)

// コマンドクラス
class CCommand
{
public:

	// コマンドの種類
	typedef enum
	{
		COMMANDTYPE_NONE,
		COMMANDTYPE_JUG,	// ジャグリング
		COMMANDTYPE_HULA,	// フラフープ
		COMMANDTYPE_SKIP,	// 縄跳び
		COMMANDTYPE_MAX
	}COMMANDTYPE;

	// 操作
	typedef enum
	{
		CONTROL_Y,
		CONTROL_X,
		CONTROL_A,
		CONTROL_B,
		CONTROL_MAX
	}CONTROL;

	CCommand();
	 ~CCommand();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetCommamnd(COMMANDTYPE type);

private:
	void isJug(void);		// ジャグリング
	void isHula(void);		// フラフープ
	void isSkip(void);		// 縄跳び

	const int m_nFream = COM_SECONDS * 60;		// フレーム数
	COMMANDTYPE m_type;							// 種類
	int m_anCommandList[NUM_BOTTON];			// コマンドリスト
	int m_nCntFream;
};
#endif