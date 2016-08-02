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

PS_OUTPUT main( PS_INPUT PSInput )  //入力値
{
    PS_OUTPUT PSOutput ;
    float4 TextureColor;
    TextureColor = tex2D( Texture , PSInput.TextureCoord0 );

    TextureColor.r = 1;

    PSOutput.Output = TextureColor;
    return PSOutput;
}