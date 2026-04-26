//===================================================
//
// グレースケール [gray_scale.hlsl]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
texture2D g_sceneTexture; // シーンのテクスチャ
texture2D g_bulerTexture; // ブラーのテクスチャ

//***************************************************
// サンプラーの設定
//***************************************************
sampler sceneSampler = sampler_state
{
    Texture = <g_sceneTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//***************************************************
// サンプラーの設定
//***************************************************
sampler blurSampler = sampler_state
{
    Texture = <g_bulerTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//***************************************************
// ピクセルシェーダー入力
//***************************************************
struct PSInput
{
    float2 uv : TEXCOORD0;
};

//***************************************************
// メイン関数
//***************************************************
float4 main(PSInput input) : COLOR0
{
    // シーンの色の取得
    float4 fSceneColor = tex2D(sceneSampler, input.uv);
    
    // 輝度抽出をし、ガウスブラーを掛けたテクスチャを取得
    float4 fBlurColor = tex2D(blurSampler, input.uv);
    
    // 合成する
    return fSceneColor + fBlurColor;
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        PixelShader = compile ps_3_0 main();
    }
}