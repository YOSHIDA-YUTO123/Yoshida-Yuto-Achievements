//===================================================
//
// 輝度抽出用シェーダー [luminance.hlsl]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
texture2D g_sceneTexture; // シーンのテクスチャ

//***************************************************
// サンプラーの設定
//***************************************************
sampler SceneSampler = sampler_state
{
    Texture = <g_sceneTexture>;
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

//===================================================
// 輝度抽出ピクセルシェーダー
//===================================================
float4 PSLuminance(PSInput input) : COLOR0
{
    // メインレンダーターゲットから色を取得
    float4 fColor = tex2D(SceneSampler, input.uv);
    
    // サンプリングしたカラーの明るさを計算
    float fBright = dot(fColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
#if 0
    // 関数の値がマイナスになると、以降の処理をスキップする
    // なのでマイナスになるとピクセルから―は出力されない
    // 今回の実装はカラーの明るさが1以下ならピクセルキルする
    clip(fBright - 1.0f);
    
    return fColor;
#else
    if (fBright > 0.78f)
        return fColor;
    else
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        PixelShader = compile ps_3_0 PSLuminance();
    }
}