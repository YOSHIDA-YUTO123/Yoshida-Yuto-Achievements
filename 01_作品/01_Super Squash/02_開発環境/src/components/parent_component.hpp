//===================================================
//
// 親子関係のデータ処理 [parent_component.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PARENT_COMPONENT_HPP_
#define _PARENT_COMPONENT_HPP_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 親のコンポーネントのデータの定義
//***************************************************
struct ParentComponent
{
	ParentComponent(entt::entity _parentID, const D3DXVECTOR3& _offSetPos = Const::VEC3_NULL, const D3DXVECTOR3& _offSetRot = Const::VEC3_NULL) :
		parentID(_parentID), offSetPos(_offSetPos)
	{
		// 生成順設定用変数
		static int nCreateID = 0;

		// 生成順を取得
		nCreateIndex = nCreateID;

		// 次へ
		nCreateID++;

		// クォータニオンの作成
		D3DXQuaternionRotationYawPitchRoll(&offSetQuaternion, _offSetRot.y, _offSetRot.x, _offSetRot.z);
	}

	entt::entity parentID;				// 親のentity
	D3DXVECTOR3 offSetPos;				// オフセットの位置
	D3DXQUATERNION offSetQuaternion;	// オフセットのクォータニオン
	int nCreateIndex;					// 生成された順番
};

//***************************************************
// 子供のコンポーネントのデータの定義
//***************************************************
struct ChildrenComponent
{
	ChildrenComponent() : aChildrenID()
	{}

	std::vector<entt::entity> aChildrenID; // 子供のentity
};
#endif