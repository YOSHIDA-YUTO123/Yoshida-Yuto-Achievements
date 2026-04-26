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

//*************************************************
// サウンドクラスの定義
//*************************************************
class CSound
{
public:

	// サウンドの種類
	typedef enum
	{
		LABEL_SE_BOUND = 0,			// 球のバウンド
		LABEL_SE_NORMAL_SHOT,		// 通常のショットの音
		LABEL_BGM_MINIGAME,			// ミニゲームのBGM
		LABEL_SE_MENU_SELECT000,	// メニュー選択音
		LABEL_SE_MENU_DECISION,		// メニュー決定音
		LABEL_SE_TITLE_DECISION,	// タイトルメニュー決定音
		LABEL_SE_WORK,				// 足音
		LABEL_SE_SHOT_MEDIUM,		// ボールのショット時の威力が強いときの音
		LABEL_SE_SHOT,				// ショット時の音
		LABEL_SE_CHARGE,			// チャージ時の音
		LABEL_BGM_TUTORIAL,			// BGMチュートリアル
		LABEL_SE_TASK_OK,			// チュートリアルタスク成功の音
		LABEL_SE_JET_PACK,			// ジェットパックの音
		LABEL_BGM_TITLE,			// タイトルのBGM
		LABEL_BGM_RESULT,			// リザルトのBGM
		LABEL_SE_SMALL_SHOT,		// 速度小の音
		LABEL_SE_MEDIUM_SHOT_MOVE,	// 速度中以上で打ったときの音
		LABEL_SE_SCORE_AREA_HIT,	// スコアのエリアに当たった音
		LABEL_SE_CREATE_WALL,		// 壁が生成される音
		LABEL_BGM_CREATE_WALL,		// 壁が生成されるBGM
		LABEL_SE_RESULT,			// リザルトの音
		LABEL_MAX
	}LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	HRESULT Play(LABEL label,const float fVolume);
	void StopSound(LABEL label);
	void StopSound(void);
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];					// オーディオデータサイズ

	// サウンド情報構造体の定義
	struct SOUND_INFO
	{
		const char* pFilename;  // ファイル名
		int nCntLoop;			// ループカウント
	};

	// サウンドの情報
	SOUND_INFO m_aSoundInfo[LABEL_MAX] =
	{
		{"data/SE/ball_jump.wav", 0},			// 球を打つ音
		{"data/SE/ball_shot.wav", 0},			// 球を打つ音
		{"data/BGM/bgm_minigame.wav", -1},		// ミニゲームのBGM
		{"data/SE/menu_select000.wav", 0},		// メニュー選択音
		{"data/SE/menu_dicision.wav", 0},		// メニュー決定音
		{"data/SE/title_decision.wav", 0},		// タイトルメニュー決定音
		{"data/SE/work.wav", 0},				// 足音
		{"data/SE/ball_shot_medium.wav", 0},	// ボールのショット時の威力が強いときの音
		{"data/SE/shot.wav", 0},				// ショット時の音
		{"data/SE/charge.wav", 0},				// チャージ時の音
		{"data/BGM/bgm_Tutorial.wav", -1},		// チュートリアルのBGM
		{"data/SE/task_ok.wav", 0},				// チャージ時の音
		{"data/SE/jetpack.wav", 0},				// ジェットパックの音
		{"data/BGM/bgm_title.wav", -1},			// タイトルのBGM
		{"data/BGM/bgm_result.wav", -1},		// リザルトのBGM
		{"data/SE/shot_small.wav", 0},			// 速度小の音
		{"data/SE/medium_shot_move.wav", 0},	// 速度中以上で打ったときの音
		{"data/SE/scoreAreaHit.wav", 0},		// スコアのエリアに当たった音
		{"data/SE/CreateWall.wav", 0},			// スコアのエリアに当たった音
		{"data/BGM/bgm_CreateWalll.wav", -1},	// スコアのエリアに当たった音
		{"data/SE/result.wav", 0},				// リザルトの音
	};				
};
#endif
