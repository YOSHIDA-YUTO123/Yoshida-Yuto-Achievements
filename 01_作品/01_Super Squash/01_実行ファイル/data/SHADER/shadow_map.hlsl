//===================================================
//
// シャドウマップ [shadow_map.hlsl]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// マクロ定義
//***************************************************

//***************************************************
// グローバル変数宣言
//***************************************************
float4x4 g_mtxWorld;        // ワールドマトリックス
float4x4 g_mtxView;         // ビューマトリックス
float4x4 g_mtxProjection;   // プロジェクションマトリックス

//***************************************************
// 頂点シェーダーへの入力
//***************************************************
struct SVSIn
{
    float4 pos : POSITION; // モデルの頂点座標
};

//===================================================
// 頂点シェーダー出力（PS入力用）
//===================================================
struct VSOut
{
    float4 pos : POSITION; 
    float4 posTex : TEXCOORD0;
};

//===================================================
// シャドウマップ描画用の頂点シェーダー
//===================================================
VSOut VSMain(SVSIn VsIn)
{
    VSOut vsOutput;

    float4 worldPos = mul(VsIn.pos, g_mtxWorld);
    float4 viewPos = mul(worldPos, g_mtxView);
    float4 projPos = mul(viewPos, g_mtxProjection);

    vsOutput.pos = projPos; // TEXCOORD0 に書き込む
    vsOutput.posTex = projPos; // TEXCOORD0 に書き込む
    return vsOutput;
}

//===================================================
// シャドウマップ描画用ピクセルシェーダー
//===================================================
float4 PSMain(VSOut PsIn) : COLOR0
{    
    float depth = PsIn.posTex.z / PsIn.posTex.w;
    
    // シャドウマップにz値を書き込む
    return float4(depth, depth, depth, 1.0f);
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