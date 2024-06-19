
#include "common.hlsl"



void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;//matrix型の変数wvpを定義する
	//wvp = ワールド変換行列×ビュー変換行列
	wvp = mul(World, View);
	//wvp = wvp × プロジェクション変換行列
	wvp = mul(wvp, Projection);
	//入力された頂点座標を変換して出力先へ代入する
	Out.Position = mul(In.Position, wvp);

	//頂点の法線をワールド行列で変換する
	float4 worldNormal, normal; //法線のワールド変換
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	//光源処理
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;

	Out.Tangent = float4(0.0, 0.0, 0.0, 0.0); //接ベクトルを出力
	Out.Binormal = float4(0.0, 0.0, 0.0, 0.0); //従法線を出力
	Out.TexCoord = In.TexCoord; //テクスチャ座標を出力

	matrix lightwvp;
	lightwvp = mul(World, Light.View);//ワールド行列＊ライトビュー行列
	lightwvp = mul(lightwvp, Light.Projection);//さらに＊ライトプロジェクション行列
	Out.ShadowPosition = mul(In.Position, lightwvp);
}
