#include "common.hlsl"

Texture2D g_Texture : register(t0); // 通常テクスチャ
Texture2D g_TextureDepthShadow : register(t1); // シャドウマップ
SamplerState g_SamplerState : register(s0);

// PCFのサンプル数（大きいほど滑らかな影になる）
#define PCF_SAMPLE_COUNT 4

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 日向での色＝普通のテクスチャ＊頂点色を作成しておく
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    In.ShadowPosition.xyz /= In.ShadowPosition.w; // 正規化デバイス座標に変換
    In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; // テクスチャ座標に変換
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

    float2 shadowMapSize = float2(1024.0, 1024.0); // シャドウマップのサイズを手動で指定する

    float totalDarkness = 0.0;

    // PCFサンプリングループ
    for (int i = -PCF_SAMPLE_COUNT / 2; i <= PCF_SAMPLE_COUNT / 2; ++i)
    {
        for (int j = -PCF_SAMPLE_COUNT / 2; j <= PCF_SAMPLE_COUNT / 2; ++j)
        {
            // サンプル位置を計算
            float2 samplePos = In.ShadowPosition.xy + float2(i, j) / shadowMapSize;

            // シャドウマップテクスチャより、ライトカメラからピクセルまでの距離（深度値）を取得
            float depth = g_TextureDepthShadow.Sample(g_SamplerState, samplePos).r;

            // 取得値が通常カメラからピクセルへの距離より小さい
            if (depth < In.ShadowPosition.z - 0.001) // 0.001はZファイティング補正値
            {
                totalDarkness += 1.0; // 影のピクセルが見つかればカウントアップ
            }
        }
    }



    // 影の暗さを調整
    float darknessRatio = totalDarkness / (PCF_SAMPLE_COUNT * PCF_SAMPLE_COUNT);
    outDiffuse.rgb *= lerp(1.0, 0.5, darknessRatio); // 影の暗さを調整
}
