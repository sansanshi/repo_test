// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 DiffuseColor       : COLOR0 ;
    float4 SpecularColor      : COLOR1 ;
    float2 TextureCoord0      : TEXCOORD0 ;
    float2 TextureCoord1      : TEXCOORD1 ;
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Output             : COLOR0 ;
} ;

// 描画するテクスチャ
sampler Texture : register( s0 ) ;

//タイマー(外部から
float timer : register( c0 );

PS_OUTPUT main( PS_INPUT PSInput )  //入力値
{
	float2 uvCenter = float2(0.5f, 0.5f);
	float radius = timer / 100.0f;//適用する半径
    PS_OUTPUT PSOutput ;
    float4 TextureColor;
    TextureColor = tex2D( Texture , PSInput.TextureCoord0 );
	float temp=(uvCenter.x - PSInput.TextureCoord0.x)*(uvCenter.x - PSInput.TextureCoord0.x) + (uvCenter.y - PSInput.TextureCoord0.y)*(uvCenter.y - PSInput.TextureCoord0.y);
	float temp2 = sqrt(temp);//中央(uvCenter)からの距離

	if (temp2 < radius){
		TextureColor.r = 1.0f - TextureColor.r;
		TextureColor.g = 1.0f - TextureColor.g;
		TextureColor.b = 1.0f - TextureColor.b;
	}

    PSOutput.Output = TextureColor;
    return PSOutput;
}