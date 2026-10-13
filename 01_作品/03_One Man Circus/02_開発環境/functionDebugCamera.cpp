//**************************************************
// 
// 機能デバッグ時のカメラの処理
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionDebugCamera.h"
#include "manager.h"
#include "camera.h"

//**************************************************
// 機能デバッグ時のカメラ更新
//**************************************************
void  CFuncDebCamera::Update(void)
{
#ifdef _DEBUG

    // カメラ取得
    CCamera* pCamera = CManager::GetCamera();

    pCamera->MouseView();
    pCamera->MouseWheel();
#endif // _DEBUG
}
