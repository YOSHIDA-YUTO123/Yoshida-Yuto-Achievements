//===================================================
//
// タイトルUI [TitleUI.h]
// Author:TAKUTO SAEKI
//
//===================================================

//===================================================
//インクルード防止
//===================================================
#ifndef _TITLELUI_H_
#define _TITLELUI_H_

//===================================================
// インクルードファイル
//===================================================
#include "object2D.h"

//===================================================
//クラスの定義
//===================================================
class CTitleUI : public CObject2D
{
public:

	// メニューの種類
	enum UI
	{
		UI_START = 0,
		UI_QUIT,
		UI_SELECT,
		UI_MAX
	};

	CTitleUI(const UI menu);
	virtual ~CTitleUI();

	static CTitleUI* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 Size, const UI menu);

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
protected:
	UI GetUI(void) const { return m_UI; }
	D3DXVECTOR2 GetBaseSize(void) const { return m_BaseSize; }
private:
	UI m_UI; // タイトルのメニュー
	D3DXVECTOR2 m_BaseSize; // 基準の大きさ
	D3DXCOLOR m_col;		// 色
	float m_fCounter;		// 色のカウンター
};

//***************************************************
// タイトルメニュー(スタート)のクラスの定義
//***************************************************
class CTitleStart : public CTitleUI
{
public:
	CTitleStart();
	~CTitleStart();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	float m_fCounterStart;//スタート後用カウンター

};

//***************************************************
// タイトルメニュー(やめる)のクラスの定義
//***************************************************
class CTitleQuit : public CTitleUI
{
public:
	CTitleQuit();
	~CTitleQuit();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};
//***************************************************
// タイトルメニュー(やめる)のクラスの定義
//***************************************************
class CTitleSelect: public CTitleUI
{
public:
	CTitleSelect();
	~CTitleSelect();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
};

//***************************************************
// タイトルメニューのマネージャークラスの定義
//***************************************************
class CTitleUIManager : public CObject2D
{
public:
	~CTitleUIManager();

	static void Create(void);
	static CTitleUIManager* GetInstance(void) { return m_pInstance; }

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	CTitleUI::UI GetUI(void) const { return m_UI; }
	bool CheckStart(void) const { return m_bStart; }
	void SetStart(bool bStart) { m_bStart = bStart; }
private:
	CTitleUIManager();
	static CTitleUIManager* m_pInstance; // 自分のインスタンス
	bool m_bStart; // スタートを押したかどうか
	CTitleUI::UI m_UI; // タイトルのメニュー
};

#endif