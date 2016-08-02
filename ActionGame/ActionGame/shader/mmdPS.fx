// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;
	float4 Specular        : COLOR1 ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ ���Őݒ肷��e�N�X�`���̒�`
sampler  DiffuseMapTexture             : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
float4   cfFactorColor                 : register( c5 ) ;		// �s�����x��



// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;

	// �o�̓J���[ = �f�B�t���[�Y�J���[ * �e�N�X�`���J���[ + �X�y�L�����J���[
	PSOutput.Color0 = PSInput.Diffuse * TextureDiffuseColor + PSInput.Specular ;

	// �o�̓A���t�@ = �f�B�t���[�Y�A���t�@ * �e�N�X�`���A���t�@ * �s�����x
	PSOutput.Color0.a = PSInput.Diffuse.a * TextureDiffuseColor.a * cfFactorColor.a ;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}