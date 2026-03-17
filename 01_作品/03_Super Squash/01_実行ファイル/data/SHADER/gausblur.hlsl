//===================================================
//
// ガウスブラー [gausblur.hlsl]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// グローバル変数宣言
//***************************************************
static const float g_weight[9] =
{
    0.14357 * 1.392, 0.21386 * 1.392, 0.21386 * 1.392,
    0.06605 * 1.392, 0.06605 * 1.392,
    0.00731 * 1.392, 0.00731 * 1.392,
    0.000161 * 1.392, 0.000161 * 1.392
};

//float g_weight[9] : register(c0);

texture2D g_sceneTexture; // シーンのテクスチャ

float BLUR_STRENGTH;    // ブラーの強さ
float SCREEN_WIDTH;     // 画面の横幅
float SCREEN_HEIGHT;    // 画面の縦幅

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
// 横ブラー用頂点シェーダー
//***************************************************
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

//***************************************************
// 横ブラー用ピクセルシェーダー
//***************************************************
struct PSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

//***************************************************
// 頂点シェーダー
//***************************************************
PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos = input.pos;
    output.uv = input.uv;
    return output;
}

//***************************************************
// ブラーピクセルシェーダー
//***************************************************
float4 PSBlurX(PSInput input) : COLOR0
{   
    float2 fTexSize = float2(1.0f / SCREEN_WIDTH, 0.0f) * BLUR_STRENGTH;
    float4 fColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    fColor += g_weight[0] * tex2D(SceneSampler, input.uv);
    fColor += g_weight[1] * tex2D(SceneSampler, input.uv + fTexSize);
    fColor += g_weight[2] * tex2D(SceneSampler, input.uv - fTexSize);
    fColor += g_weight[3] * tex2D(SceneSampler, input.uv + fTexSize * 3.0f);
    fColor += g_weight[4] * tex2D(SceneSampler, input.uv - fTexSize * 3.0f);
    fColor += g_weight[5] * tex2D(SceneSampler, input.uv + fTexSize * 5.0f);
    fColor += g_weight[6] * tex2D(SceneSampler, input.uv - fTexSize * 5.0f);
    fColor += g_weight[7] * tex2D(SceneSampler, input.uv + fTexSize * 7.0f);
    fColor += g_weight[8] * tex2D(SceneSampler, input.uv - fTexSize * 7.0f);
    
    return fColor;
}

//***************************************************
// ブラーピクセルシェーダー
//***************************************************
float4 PSBlurY(PSInput input) : COLOR0
{
    float2 fTexSize = float2(0.0f, 1.0f / SCREEN_HEIGHT) * BLUR_STRENGTH;
    float4 fColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    fColor += g_weight[0] * tex2D(SceneSampler, input.uv);
    fColor += g_weight[1] * tex2D(SceneSampler, input.uv + fTexSize);
    fColor += g_weight[2] * tex2D(SceneSampler, input.uv - fTexSize);
    fColor += g_weight[3] * tex2D(SceneSampler, input.uv + fTexSize * 3.0f);
    fColor += g_weight[4] * tex2D(SceneSampler, input.uv - fTexSize * 3.0f);
    fColor += g_weight[5] * tex2D(SceneSampler, input.uv + fTexSize * 5.0f);
    fColor += g_weight[6] * tex2D(SceneSampler, input.uv - fTexSize * 5.0f);
    fColor += g_weight[7] * tex2D(SceneSampler, input.uv + fTexSize * 7.0f);
    fColor += g_weight[8] * tex2D(SceneSampler, input.uv - fTexSize * 7.0f);
    
    return fColor;
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass XBluer
    {
        PixelShader = compile ps_3_0 PSBlurX();
    }

    pass YBluer
    {
        PixelShader = compile ps_3_0 PSBlurY();
    }
}