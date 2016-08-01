// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;       // �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;    // �e�N�X�`�����W
	float2 ToonCoords0     : TEXCOORD1 ;    // �g�D�[���e�N�X�`�����W
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;



// C++ ���Őݒ肷��e�N�X�`���̒�`
sampler  DiffuseMapTexture             : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
sampler  ToonTexture                   : register( s1 ) ;		// �g�D�[���e�N�X�`��
float4   cfFactorColor                 : register( c5 ) ;		// �s�����x��



// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 ToonColor ;

	// �e�N�X�`���J���[�̎擾
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0 ) ;

	// �g�D�[���e�N�X�`���J���[���擾
	ToonColor = tex2D( ToonTexture, PSInput.ToonCoords0 ) ;

	// �o�� = saturate( �f�B�t���[�Y�J���[ * �g�D�[���e�N�X�`���J���[ + �X�y�L�����J���[ ) * �e�N�X�`���J���[
	PSOutput.Color0.rgb = saturate( PSInput.Diffuse.rgb * ToonColor.rgb + PSInput.Specular.rgb ) * TextureDiffuseColor.rgb ;

	// �A���t�@�l = �f�B�t���[�Y�A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	PSOutput.Color0.a = TextureDiffuseColor.a * PSInput.Diffuse.a * cfFactorColor.a ;


	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}