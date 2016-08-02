// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 DiffuseColor       : COLOR0 ;
    float4 SpecularColor      : COLOR1 ;
    float2 TextureCoord0      : TEXCOORD0 ;
    float2 TextureCoord1      : TEXCOORD1 ;
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Output             : COLOR0 ;
} ;

// �`�悷��e�N�X�`��
sampler Texture : register( s0 ) ;

//�^�C�}�[(�O������
float timer : register( c0 );

PS_OUTPUT main( PS_INPUT PSInput )
{
    PS_OUTPUT PSOutput ;
    float4 TextureColor;
    float2 CalcPos;

    CalcPos.x = fmod( 1.0 + PSInput.TextureCoord0.x + 0.2*sin( PSInput.TextureCoord0.y * 24.0 + timer ) , 1.0 ) ;
    CalcPos.y = PSInput.TextureCoord0.y;

    TextureColor = tex2D( Texture , CalcPos );
    PSOutput.Output = TextureColor;
    return PSOutput;
}