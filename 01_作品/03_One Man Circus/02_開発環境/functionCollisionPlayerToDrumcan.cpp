//**************************************************
// 
// 機能プレイヤーとドラム缶の当たり判定クラス
// Author Tetsuji Yamamoto
// 
//**************************************************
#include "functionCollisionPlayerToDrumcan.h"
#include "game.h"
#include "player.h"
#include "DrumCan.h"
#include "transform.h"
#include "statePlayer.h"

//**************************************************
// 機能プレイヤーとドラム缶の当たり判定更新
//**************************************************
void CFuncCollPlayerToDrumcan::Update(void)
{
	//// プレイヤーとドラム缶取得
	//CPlayer* pPlayer = CGame::GetPlayer();
	//CDramcan* pDramcan = CGame::GetDramcan();

	//// 当たり判定の取得
	//CCollisionObject3D* pCollision = pPlayer->GetCollision();

	//// 空間情報の取得
	//CTransform* pTransform = pCollision->GetTransform();

	//// 情報の取得
	//CTransform::Info transform = pTransform->GetInfo();

	//// 立て直しであれば処理しない
	//if (pPlayer->GetStateBaranceID() == CStateBasePlayer::ID_BALANCE_REBUILD)
	//{
	//	return;
	//}

	////***********************************************************
	//// 変更部分(_吉田_)Collision -> CheckPathに変更した
	//// (Updateで判定して押し出ししていたのをもう一回判定していたためelseが通っていた)
	////***********************************************************
	//if (pPlayer->CheckLandingMapObj(CPlayer::FILEPATH_DRUMMCAN))
	//{
	//	// ローカル位置作成
	//	pPlayer->CreatePositionLocal(pDramcan->GetPointerMatrix(), transform.pos);

	//	// 移動量を消す
	//	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	//	movePlayer.y = 0.0f;
	//	pPlayer->SetMove(movePlayer);

	//	// 乗ってる状態にする
	//	pPlayer->SetIsRanding(true);

	//	// 状態を変更
	//	pPlayer->ChangeState(std::make_shared<CStatePlayerDramcan>());
	//	pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalance>());

	//	// ドラム缶移動
	//	D3DXVECTOR3 vecMovePlayer = pPlayer->GetVecMove();
	//	D3DXVECTOR3 rot = pPlayer->GetRotationBarance();
	//	rot.y = pPlayer->GetRotation().y;
	//	pDramcan->SetMove(rot, 0.4f);
	//}
	//else
	//{
	//	// バランスとっていれば解除
	//	if (pPlayer->GetStateBaranceID() == CStateBasePlayer::ID_BALANCE_BALANCE &&
	//		pPlayer->GetStateID() == CStateBasePlayer::ID_DRAMCAN)
	//	{
	//		pPlayer->ResetBarance();
	//		pPlayer->ChangeState(std::make_shared<CStatePlayerNewtral>());
	//		pPlayer->ChangeStateBalance(std::make_shared<CStatePlayerBalanceNone>());
	//		return;
	//	}

	//	pDramcan->SetIsMove(false);
	//}

	//// 位置設定
	//pTransform->SetInfo(transform);

}