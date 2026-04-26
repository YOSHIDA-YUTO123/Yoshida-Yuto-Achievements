//===================================================
//
// モーションのロード処理 [motion_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion_loader.h"
#include "fstream"
#include "manager.h"
#include "model_manager.h"
#include "parent_component.hpp"
#include "motion_component.hpp"
#include "character_model_component.hpp"
#include "transform_component.hpp"
#include "factory_system_entity.h"

//***************************************************
// 匿名の名前空間(見やすくするための処理分割)
//***************************************************
namespace
{
    //***************************************************
    // モデルのロード処理
    //***************************************************
    void LoadModel(entt::entity character, entt::registry& registry, std::string line)
    {
        std::string input;

        // モデルマネージャーの取得
        CModelManager* pModelManager = CManager::GetInstance()->GetModelManager();

        // 子供のentityを取得するためのコンポーネント
        auto& childrenComp = registry.get<ChildrenComponent>(character);
        auto& MotionComp = registry.get<MotionComponent>(character);

        size_t equal_pos = line.find("="); // =の位置

        // [=] から先を求める
        input = line.substr(equal_pos + 1);

        // 読み取れたら
        if (line.find("NUM_MODEL") != std::string::npos)
        {
            int nNumModel = 0;

            // 値を取得
            std::istringstream value(input);

            value >> nNumModel;

            // モデルの総数の設定
            MotionComp.nNumModel = nNumModel;

            // メモリの確保
            childrenComp.aChildrenID.reserve(nNumModel);
        }

        if (line.find("MODEL_FILENAME") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // モデルの名前格納用変数
            std::string modelName;

            // 数値を代入する
            value >> modelName;

            // モデルのIDの登録
            int nModelID = pModelManager->Register(modelName.c_str());

            // 構造体の取得
            CModelManager::ModelInfo info = pModelManager->GetModelInfo(nModelID);

            // モデルの生成
            entt::entity entity = registry.create();

            // キャラクターのモデルのデータの追加
            registry.emplace<CharacterModelComponent>(entity, nModelID,-1, info.size);
            registry.emplace<Transform3DComponent>(entity);
            registry.emplace<ParentComponent>(entity, character);

            // entity番号の取得
            childrenComp.aChildrenID.push_back(entity);
        }
    }
    //***************************************************
    // キャラクターセットのロード処理
    //***************************************************
    bool LoadCharacterSet(entt::entity character, entt::registry& registry, std::string line)
    {
        std::string input;
        static int nIdx = 0;
        int nNumParts = 0;
        int nParent = 0;
        D3DXVECTOR3 offset = Const::VEC3_NULL;

        // 子供のentityを取得するためのコンポーネント
        auto& childrenComp = registry.get<ChildrenComponent>(character);

        size_t equal_pos = line.find("="); // =の位置

        // [=] から先を求める
        input = line.substr(equal_pos + 1);

        if (line.find("NUM_PARTS") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> nNumParts;
        }

        if (line.find("INDEX") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> nIdx;

            auto& modelComp = registry.get<CharacterModelComponent>(childrenComp.aChildrenID[nIdx]);

            modelComp.nIdex = nIdx;
        }

        if (line.find("PARENT") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> nParent;

            // 親子関係のコンポーネント
            auto& parentComp = registry.get<ParentComponent>(childrenComp.aChildrenID[nIdx]);

            if (nParent != -1)
            {// 親が存在していたら
                parentComp.parentID = childrenComp.aChildrenID[nParent];
            }
            else
            {// 親が存在していなかったら
                parentComp.parentID = character;
            }
        }

        if (line.find("MODEL_PART") != std::string::npos)
        {
            int nPart = -1;

            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> nPart;

            if (nPart != -1)
            {
                //pModel[nIdx]->SetParts(nPart == 1 ? true : false);
            }
        }

        if (line.find("POS") != std::string::npos)
        {
            // 親子関係のコンポーネント
            auto& parentComp = registry.get<ParentComponent>(childrenComp.aChildrenID[nIdx]);

            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> offset.x;
            value >> offset.y;
            value >> offset.z;

            // オフセットの設定
            parentComp.offSetPos = offset;
        }

        if (line.find("ROT") != std::string::npos)
        {
            // 親子関係のコンポーネント
            auto& parentComp = registry.get<ParentComponent>(childrenComp.aChildrenID[nIdx]);

            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> offset.x;
            value >> offset.y;
            value >> offset.z;

            // クォータニオンの作成
            D3DXQuaternionRotationYawPitchRoll(&parentComp.offSetQuaternion, offset.y, offset.x, offset.z);
        }

        if (line.find("END_CHARACTERSET") != std::string::npos)
        {
            nIdx = 0;
            return false;
        }

        return true;
    }
    //***************************************************
    // モーション設定のロード処理
    //***************************************************
    bool LoadMotionSet(entt::entity character, entt::registry& registry, std::string line, int* pCntMotion)
    {
        // モーションのコンポーネント
        auto& motionComp = registry.get<MotionComponent>(character);

        // キャラクターのコンポーネント
        auto& childrenComp = registry.get<ChildrenComponent>(character);

        std::string input; // 値取得用文字列
        int loop = 0;      // ループ条件取得用変数
        static int nKey = 0;        // 現在のキー
        static int nCntModel = 0;   // モデルのカウント

        int nCntMotion = *pCntMotion;

        // モーションのカウントが総数以上だったら読み込まない
        if (*pCntMotion >= motionComp.nNumMotion)
        {
            nKey = 0;        // 現在のキーリセット
            nCntModel = 0;   // モデルのカウントリセット

            return false;
        }

        size_t equal_pos = line.find("="); // =の位置

        // [=] から先を求める
        input = line.substr(equal_pos + 1);

        if (line.find("LOOP") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> loop;

            // ループするかどうか
            motionComp.aInfo[nCntMotion].bLoop = (loop == 1) ? true : false;
        }

        if (line.find("NUM_KEY") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            int nNumKey = 0;

            // 数値を代入する
            value >> nNumKey;
            motionComp.aInfo[nCntMotion].nNumkey = nNumKey;

            // キーの総数分配列を確保する
            motionComp.aInfo[nCntMotion].aKeyInfo.resize(nNumKey);

            // キーの総数分回す
            for (auto& keies : motionComp.aInfo[nCntMotion].aKeyInfo)
            {
                // モデルのサイズ分配列の確保
                keies.aKey.resize(childrenComp.aChildrenID.size());
            }
        }

        if (line.find("NUM_FRAME") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            int nNumFrame = 0;

            // 数値を代入する
            value >> nNumFrame;
            motionComp.aInfo[nCntMotion].nNumEventFrame = nNumFrame;

            // イベントフレームの総数分メモリの確保
            motionComp.aInfo[nCntMotion].aStartFrame.reserve(nNumFrame);
            motionComp.aInfo[nCntMotion].aEndFrame.reserve(nNumFrame);
        }

        if (line.find("START_FRAME") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            int nFrame = 0;

            // 残りの値をすべて読み込む
            while (value >> nFrame)
            {
                motionComp.aInfo[nCntMotion].aStartFrame.push_back(nFrame);
            }
        }
        if (line.find("END_FRAME") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            int nFrame = 0;

            // 残りの値をすべて読み込む
            while (value >> nFrame)
            {
                motionComp.aInfo[nCntMotion].aEndFrame.push_back(nFrame);
            }
        }

        if (line.find("FRAME") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].nFrame;
        }

        if (line.find("POS") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 最大数をオーバーしていたら
            if (nCntModel >= motionComp.nNumModel)
            {
                // エラー情報を設定
                std::ostringstream oss;

                oss << "現在のモーション = " << nCntMotion << "現在のキー = " << nKey << "現在のモデル = " << nCntModel << "\n"
                    << "最大モーション数 = " << motionComp.nNumMotion << "最大モデル数 = " << motionComp.nNumModel;

                // 情報を取得
                std::string message = oss.str();

                // エラー情報を表示
                MessageBox(NULL, message.c_str(), "MOTION_POS", MB_OK | MB_ICONWARNING);
                return false;
            }

            // 数値を代入する
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fPosX;
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fPosY;
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fPosZ;
        }
        if (line.find("ROT") != std::string::npos)
        {
            // 数値を読み込む準備
            std::istringstream value(input);

            // 数値を代入する
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fRotX;
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fRotY;
            value >> motionComp.aInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntModel].fRotZ;
        }

        if (line.find("END_KEY") != std::string::npos)
        {
            nCntModel++;
        }

        if (line.find("END_KEYSET") != std::string::npos)
        {
            nKey++;
            nCntModel = NULL;
        }
        if (line.find("END_MOTIONSET") != std::string::npos)
        {

            nKey = NULL;

            // 最大数じゃないなら
            (*pCntMotion)++;
        }

        return true;
    }
}

//===================================================
// モーションのロード処理
//===================================================
HRESULT Loader::Motion::Load(entt::entity character, entt::registry& registry, const char* pMotionTxtPath)
{
    std::string filePath = pMotionTxtPath;

    // 省略されていたら
    if (filePath.find("data/MOTION/") == std::string::npos)
    {
        filePath = "data/MOTION/";
    }

    // パスの連結
    filePath += pMotionTxtPath;

    std::fstream file(filePath);
    std::string line;

    // キャラクター設定ができるかどうか
    bool bCharacterSet = true;

    // カウント用モーション
    int nCntMotion = 0;

    // ファイルが開けたら
    if (file.is_open())
    {
        // ファイルを一行ずつ読み取る
        while (std::getline(file, line))
        {
            // モデルの読み込み処理
            LoadModel(character, registry, line);

            // キャラクターの設定ができるなら
            if (bCharacterSet)
            {
                // キャラクター設定の読み込み処理
                bCharacterSet = LoadCharacterSet(character, registry, line);
            }
            else
            {
                // モーション設定の読み込み処理
                if (LoadMotionSet(character, registry, line, &nCntMotion) == false)
                {
                    // 読み込めなかったら処理を抜ける
                    break;
                }
            }
        }
    }
    else
    {
        MessageBox(NULL, "読み込み失敗", filePath.c_str(), MB_OK | MB_ICONWARNING);
        return E_FAIL;
    }
    return S_OK;
}
