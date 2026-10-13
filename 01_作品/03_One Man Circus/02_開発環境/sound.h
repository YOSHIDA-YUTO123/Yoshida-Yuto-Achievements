//================================================
//
// サウンド処理 [sound.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************
// インクルードファイル
//*************************************************
#include"main.h"

//*************************************************
// サウンドクラスの定義
//*************************************************
class CSound
{
public:

	// サウンドの種類
	typedef enum
	{
		SOUND_LABEL_TITLEBGM = 0,	// タイトルBGM
		SOUND_LABEL_GAMEBGM,		// ゲームBGM
		SOUND_LABEL_TIMEOVER,		// タイムオーバー
		SOUND_LABEL_SE_START,		// スタート
		SOUND_LABEL_SE_MISS,		// 失敗
		SOUND_LABEL_SE_NORMAL,		// 
		SOUND_LABEL_SE_Appluse,		
		SOUND_LABEL_SE_Cheers,		
		SOUND_LABEL_SE_Success,
		SOUND_LABEL_RANKING,
		SOUND_LABEL_SE_TITLE_SELECT,	// タイトル選択
		SOUND_LABEL_SE_JUGGLING_SUCCESS,	// ジャグリング選択成功
		SOUND_LABEL_SE_JUGGLING_FAIL,		// ジャグリング選択失敗
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	HRESULT Play(SOUND_LABEL label,const float fVolume);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// サウンド情報構造体の定義
	struct SOUND_INFO
	{
		const char* pFilename;  // ファイル名
		int nCntLoop;			// ループカウント
	};

	// サウンドの情報
	SOUND_INFO m_aSoundInfo[SOUND_LABEL_MAX] = 
	{
		{"data/BGM/Title_BGM.wav", -1},//TITLEBGM
		{"data/BGM/GAME_BGM.wav",-1},
		{"data/BGM/TimeOver.wav",-1},
		{"data/SE/Title_Start.wav",0},
		{"data/SE/Audience_Miss.wav",0},
		{"data/SE/Audience_Normal.wav",-1},
		{"data/SE/Audience_Appluse.wav",-1},
		{"data/SE/Audience_Cheers.wav",0},
		{"data/SE/Audience_Success.wav",0},
		{"data/BGM/Ranking.wav",-1},
		{"data/SE/Title_Select.wav",0},
		{"data/SE/juggling_Select_success.wav",0},
		{"data/SE/juggling_Select_fail.wav",0},
		


	};				
};
#endif
