struct PS_INPUT {
    float4 DiffuseColor     : COLOR0;
    float4 SpecularColor    : COLOR1;
    float2 TextureCoord0    : TEXCOORD0;
    float2 TextureCoord1    : TEXCOORD1;
};
 
struct PS_OUTPUT {
    float4 Output           : COLOR0;
};
 
sampler DiffuseMapTexture : register(s0);
 
PS_OUTPUT main(PS_INPUT PSInput) {
    PS_OUTPUT PSOutput;
    float4 nowColor;
    nowColor=tex2D(DiffuseMapTexture,PSInput.TextureCoord0);
 
    PSOutput.Output.r=1.0f-nowColor.r;
    PSOutput.Output.g=1.0f-nowColor.g;
    PSOutput.Output.b=1.0f-nowColor.b;
    PSOutput.Output.a=nowColor.a;
 
    return PSOutput;
}