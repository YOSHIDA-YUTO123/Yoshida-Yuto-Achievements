//**************************************************************************
// 
// フォントマネージャー
// Author Tetsuji Yamamoto
// 
//**************************************************************************
#include "fontManager.h"
#include "renderer.h"
#include "manager.h"
#include <fstream>

// 静的メンバー変数宣言
std::map<std::string, CFontManager::FONTINFO> CFontManager::m_mFontInfo;

//**************************************************************************
// フォントマネージャー
//**************************************************************************
CFontManager::CFontManager()
{

}

//**************************************************************************
// フォントマネージャー
//**************************************************************************
CFontManager::~CFontManager()
{
}

//**************************************************************************
// フォントマネージャー
//**************************************************************************
void CFontManager::Init(void)
{

}

//**************************************************************************
// フォントマネージャー破棄
//**************************************************************************
void CFontManager::Uninit(void)
{
	// すべてのゴンとを破棄
	for (auto& iterFontInfo : m_mFontInfo)
	{
		//デバッグ表示用フォントの破棄
		if (iterFontInfo.second.pFont != nullptr)
		{
			iterFontInfo.second.pFont->Release();
			iterFontInfo.second.pFont = nullptr;
		}

		// 読み込んだフォントの破棄
		RemoveFontMemResourceEx(iterFontInfo.second.hFontResource);
	}

	// 自身の破棄
	delete this;
}

//**************************************************************************
// フォントマネージャー全フォントの破棄
//**************************************************************************
LPD3DXFONT CFontManager::UnLord(const char* pFontName)
{
	// 名前が一致したフォントがあれば削除
	auto iterFontInfo = m_mFontInfo.find(pFontName);
	if (iterFontInfo != m_mFontInfo.end())
	{
		//デバッグ表示用フォントの破棄
		if ((*iterFontInfo).second.pFont != nullptr)
		{
			(*iterFontInfo).second.pFont->Release();
			(*iterFontInfo).second.pFont = nullptr;
		}

		// 読み込んだフォントの破棄
		RemoveFontMemResourceEx((*iterFontInfo).second.hFontResource);
	}

	return (*iterFontInfo).second.pFont;
}

//**************************************************************************
// フォントマネージャーフォントの追加
//**************************************************************************
const char* CFontManager::Regiter(int nFontID, const char* pFontName)
{
	if (pFontName == NULL)
	{
		return "";
	}

	// 同じ名前の物があればそれを返す
	if (m_mFontInfo.find(pFontName) != m_mFontInfo.end())
	{
		return pFontName;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォント情報
	FONTINFO fontInfo;

	// モージュールハンドルを取得
	HMODULE hModule = GetModuleHandle(NULL);

	// リソースIDを文字列型に変換
	LPCTSTR strtest = MAKEINTRESOURCE(nFontID);

	// リソースの場所を決定
	fontInfo.hFontResource = FindResource(hModule, strtest, "BINARY");

	// リソースが見つからなかった場合エラーメッセージを出す
	if (fontInfo.hFontResource == nullptr) {
		MessageBox(NULL, "リソースが見つかりません", "エラー", MB_OK);
		return "";
	}

	// リソースのサイズを取得
	DWORD dwFontImageSize = SizeofResource(NULL, fontInfo.hFontResource);

	// リソースのポインターを取得するためのハンドルを取得
	HGLOBAL Address = LoadResource(hModule, fontInfo.hFontResource);

	// リソースのポインターを取得
	LPVOID lpFontImage = LockResource(Address);

	// フォントのカウンター
	DWORD dwFontCount = 0;

	// フォントの追加
	HANDLE _hFontResource = AddFontMemResourceEx(lpFontImage, dwFontImageSize, NULL, &dwFontCount);

	if (_hFontResource == 0)
	{
		MessageBox(NULL, "フォントを追加できません", "Error", MB_OK);
		return "";
	}

	// フォントの生成
	HRESULT hr = D3DXCreateFont(pDevice,
		100,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		pFontName,
		&fontInfo.pFont);

	// フォントの生成に失敗した
	if (FAILED(hr))
	{
		MessageBox(NULL, "フォントを生成できません", "Error", MB_OK);
		return "";
	}

	// 情報を追加
	m_mFontInfo.insert(std::make_pair(pFontName, fontInfo));

	return pFontName;
}

//**************************************************************************
// フォントマネージャーフォントの取得
//**************************************************************************
LPD3DXFONT CFontManager::GetFont(const char* pFontName)
{
	// 名前が一致したフォントがあればそれを返す
	auto iterFontInfo = m_mFontInfo.find(pFontName);
	if (iterFontInfo != m_mFontInfo.end())
	{
		return (*iterFontInfo).second.pFont;
	}

	return nullptr;
}

//**************************************************************************
// フォントマネージャー生成
//**************************************************************************
CFontManager* CFontManager::Create(void)
{
	CFontManager* pFontMane = nullptr;

	// メモリ確保
	pFontMane = new CFontManager;

	if (pFontMane != nullptr)
	{
		// 初期化処理
		pFontMane->Init();	// 初期化処理
	}
	else
	{
		MessageBox(NULL, " CFontManager* Create() > if ( pFont != nullptr) メモリを確保できませんでした", "Error", MB_OK);
	}

	return  pFontMane;
}