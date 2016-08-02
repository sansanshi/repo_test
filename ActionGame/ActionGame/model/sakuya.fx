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

PS_OUTPUT main( PS_INPUT PSInput )  //���͒l
{
	float2 uvCenter = float2(0.5f, 0.5f);
	float radius = timer / 100.0f;//�K�p���锼�a
    PS_OUTPUT PSOutput ;
    float4 TextureColor;
    TextureColor = tex2D( Texture , PSInput.TextureCoord0 );
	float2 CalcPos;

	float temp=(uvCenter.x - PSInput.TextureCoord0.x)*(uvCenter.x - PSInput.TextureCoord0.x) + (uvCenter.y - PSInput.TextureCoord0.y)*(uvCenter.y - PSInput.TextureCoord0.y);
	float temp2 = sqrt(temp);//����(uvCenter)����̋���

	if (temp2 < radius){
		//�F���]
		//TextureColor.r = 1.0f - TextureColor.r;
		//TextureColor.g = 1.0f - TextureColor.g;
		//TextureColor.b = 1.0f - TextureColor.b;

		//�O���C�X�P�[����
		float Y = TextureColor.r * 0.29891f + TextureColor.g * 0.58661f + TextureColor.b * 0.11448f;
		TextureColor.r = Y;
		TextureColor.g = Y;
		TextureColor.b = Y;
	}
	/*if (temp2 < radius + 0.2f)//���ɂႮ�ɂ�
	{
		CalcPos.x = fmod(1.0 + PSInput.TextureCoord0.x + 0.2*sin(PSInput.TextureCoord0.y * 24.0 + timer), 1.0);
		CalcPos.y = PSInput.TextureCoord0.y;

		TextureColor = tex2D(Texture, CalcPos);
	}*/

    PSOutput.Output = TextureColor;
    return PSOutput;
}