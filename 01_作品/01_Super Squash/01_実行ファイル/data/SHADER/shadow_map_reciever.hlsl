//===================================================
//
// シャドウマップ [shadow_map_reciever.hlsl]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// マクロ定義
//***************************************************

//***************************************************
// グローバル変数宣言
//***************************************************
float4x4 g_mtxWorld;            // ワールドマトリックス
float4x4 g_mtxView;             // ビューマトリックス
float4x4 g_mtxProjection;       // プロジェクションマトリックス
float4x4 g_mtxLVP;              // ライトビュープロジェクションマトリックス
texture2D g_baseTexture;        // もとのテクスチャ
texture2D g_shadowMapTexture;   // シャドウテクスチャ
float3 g_lightDir;              // ライトの方向

//***************************************************
// サンプラーの設定
//***************************************************
sampler baseTextureSampler = sampler_state
{
    Texture = <g_baseTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//***************************************************
// サンプラーの設定
//***************************************************
sampler shadowMapSampler = sampler_state
{
    Texture = <g_shadowMapTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

//***************************************************
// 頂点シェーダーへの入力
//***************************************************
struct SVSIn
{
    float4 pos      : POSITION; // モデルの頂点座標
    float3 noraml   : NORMAL;
    float2 uv       : TEXCOORD0;
};

//***************************************************
// 出力 / ピクセルシェーダーへの入力
//***************************************************
struct VSOut
{
    float4 pos      : POSITION; // クリップ空間座標
    float3 noraml   : TEXCOORD1;
    float2 uv       : TEXCOORD0; // テクスチャ座標
    float4 posInLVP : TEXCOORD2; // ライト空間クリップ座標
};

//===================================================
// 影が落とされるモデルの頂点シェーダー
//===================================================
VSOut VSMain(SVSIn vsInput)
{
    // 通常の座標変換
    VSOut vsOut;
    
    // ワールド座標の計算
    float4 worldPos = mul(vsInput.pos, g_mtxWorld);
    
    // 通常のカメラ視点の座標変換
    float4 viewPos = mul(worldPos, g_mtxView);
    
    // ライトの空間の座標を計算(シャドウマップ比較用)
    vsOut.pos = mul(viewPos, g_mtxProjection);
    
    // ライトビュースクリーン空間の座標を計算する
    vsOut.posInLVP = mul(worldPos, g_mtxLVP); // TEXCOORD1
    
    vsOut.uv = vsInput.uv;
    vsOut.noraml = mul(vsInput.noraml, (float3x3) g_mtxWorld);
    
    return vsOut;
}

//===================================================
// 影が落とされる3Dモデル用のピクセルシェーダー
//===================================================
float4 PSMain(VSOut psInput) : COLOR0
{    
    float4 color = tex2D(baseTextureSampler, psInput.uv);
        
    // ライト空間座標からUV計算
    float2 shadowMapUV = psInput.posInLVP.xy / psInput.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    // ライトとモデルの法線の内積をする
    float dotNL = dot(normalize(psInput.noraml), g_lightDir);
    
    // ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = psInput.posInLVP.z / psInput.posInLVP.w;
    
    if(shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
        && psInput.posInLVP.z > 0.0f
        && dotNL < 0.0f)
    {
        // シャドウマップに書き込まれているZ値と比較する
        // 計算したUV座標を使って、シャドウマップから深度値をサンプリング
        float shadowMap = tex2D(shadowMapSampler, shadowMapUV).r;
        
        if (zInLVP > shadowMap)
        {
            // 遮蔽されている
            color.xyz *= 0.2f;
        }
    }
        
    return color;
}

//===================================================
// テクニック宣言
//===================================================
technique tech
{
    pass P0
    {
        VertexShader = compile vs_3_0 VSMain();
        PixelShader = compile ps_3_0 PSMain();
    }
}