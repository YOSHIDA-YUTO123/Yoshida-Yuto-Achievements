//**************************************************
// 
// テンプレート
// Author Tetsuji Yamamoto
// 
//**************************************************
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

// テンプレートクラス
class CTemplate
{
public:
	CTemplate();
	~CTemplate();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTemplate* Create(void);

private:
};

#endif