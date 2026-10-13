//==================================================
// 
// 観客マネージャー [AudienceManager.h]
// Author：Aoto Kaichi
// 
//==================================================

#ifndef _AUDIENCEMANAGER_H_
#define _AUDIENCEMANAGER_H_

// インクルード
#include "Audience.h"

// 観客マネージャークラス
class CAudienceManager : public CObject
{
public:
	CAudienceManager();
	~CAudienceManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAnimation(CAudience::ANIMETYPE animetype);

	static CAudienceManager* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, float fJump, float fSpeed, const bool bLight_Off = false);

private:
	static constexpr int MAX_AUDIENCE = 25;	// 両サイドを除く最大数
	static constexpr float AUDIENCE_SPACE = 45.0f;	// 両サイドを除く最大数

	// メンバ変数
	CAudience* m_pAudiences[MAX_AUDIENCE];	// 観客たちへのポインタ
};

#endif