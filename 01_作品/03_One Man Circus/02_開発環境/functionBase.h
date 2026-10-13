//**************************************************
// 
// 機能基底クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _FANCTIONBASE_H_
#define _FANCTIONBASE_H_

// 機能基底クラス
class CFunctionBase
{
public:
	CFunctionBase() {}
	virtual ~CFunctionBase() {}

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

private:
};

#endif