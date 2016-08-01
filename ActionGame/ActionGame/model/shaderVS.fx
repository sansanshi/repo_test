// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position        : POSITION0 ;         // ���W( ���[�J����� )
	int4   BlendIndices0   : BLENDINDICES0 ;    // �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // �X�L�j���O�����p�E�G�C�g�l
	float3 Normal          : NORMAL0 ;          // �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;           // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;           // �X�y�L�����J���[
	float4 TexCoords0      : TEXCOORD0 ;        // �e�N�X�`�����W
} ;

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Position        : POSITION0 ;         // ���W( �v���W�F�N�V������� )
	float4 Diffuse         : COLOR0 ;           // �f�B�t���[�Y�J���[
	float4 Specular        : COLOR1 ;           // �X�y�L�����J���[
	float2 TexCoords0      : TEXCOORD0 ;        // �e�N�X�`�����W
	float2 ToonCoords0     : TEXCOORD1 ;        // �g�D�[���e�N�X�`�����W
} ;

// �}�e���A���p�����[�^
struct VS_CONST_MATERIAL
{
	float4 Diffuse ;                // �}�e���A���f�B�t���[�Y�J���[
	float4 Specular ;               // �}�e���A���X�y�L�����J���[
	float4 Power ;                  // �}�e���A���X�y�L�����n�C���C�g�̋���
} ;

// ���C�g�p�����[�^
struct VS_CONST_LIGHT
{
	float4 Position ;               // ���W( �r���[��� )
	float3 Direction ;              // ����( �r���[��� )
	float4 Diffuse ;                // �f�B�t���[�Y�J���[
	float4 Specular ;               // �X�y�L�����J���[
	float4 Ambient ;                // �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
	float4 Range_FallOff_AT0_AT1 ;  // x:�L������  y:�X�|�b�g���C�g�pFallOff  z:�����ɂ�錸�������p�p�����[�^�O  w:�����ɂ�錸�������p�p�����[�^�P
	float4 AT2_SpotP0_SpotP1 ;      // x:�����ɂ�錸�������p�p�����[�^�Q  y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;



// C++ ���Őݒ肷��萔�̒�`
float4              cfAmbient_Emissive        : register( c1  ) ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[
float4              cfProjectionMatrix[ 4 ]   : register( c2  ) ;		// �r���[�@�@���@�ˉe�s��
float4              cfViewMatrix[ 3 ]         : register( c6  ) ;		// ���[���h�@���@�r���[�s��
float4              cfTextureMatrix[ 3 ][ 2 ] : register( c88 ) ;		// �e�N�X�`�����W����p�s��
float4              cfLocalWorldMatrix[ 162 ] : register( c94 ) ;		// ���[�J���@���@���[���h�s��
VS_CONST_MATERIAL   cfMaterial                : register( c11 ) ;		// �}�e���A���p�����[�^
VS_CONST_LIGHT      cfLight[ 1 ]              : register( c14 ) ;		// �L�����C�g�̃p�����[�^




// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float4 lTotalDiffuse ;
	float4 lTotalSpecular ;
	float3 lLightDir ;
	float3 lLightTemp ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float4 lTotalAmbient ;
	float lTotalLightGen ;


	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[ 0 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 0 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 1 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 2 ] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 0 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 1 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 2 ] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 0 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 1 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 2 ] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 0 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 1 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 2 ] * VSInput.BlendWeight0.w;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot( VSInput.Position, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )




	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �J�n )

	// ���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	lWorldNrm.x = dot( VSInput.Normal, lLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, lLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, lLocalWorldMatrix[ 2 ].xyz ) ;

	// ���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	lViewNrm.x = dot( lWorldNrm, cfViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, cfViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, cfViewMatrix[ 2 ].xyz ) ;

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �I�� )


	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�ƃA���r�G���g�J���[�̍��v�l��������
	lTotalDiffuse  = float4( 0, 0, 0, 0 ) ;
	lTotalSpecular = float4( 0, 0, 0, 0 ) ;
	lTotalAmbient  = float4( 0, 0, 0, 0 ) ;

	// ���C�g�̌��������v�l�̏�����
	lTotalLightGen = 0.0f ;




	// �f�B���N�V���i�����C�g�̏��� *****************************************************( �J�n )


	// ���C�g�̕����Z�b�g
	lLightDir = cfLight[ 0 ].Direction ;


	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z ===================( �J�n )

	// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
	lLightLitParam.x = dot( lViewNrm, -lLightDir ) ;

	// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - lLightDir ) ;

	// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
	lLightLitParam.w = cfMaterial.Power.x ;

	// ���C�g�v�Z
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// �f�B�t���[�Y���C�g�ƃX�y�L�������C�g�̊p�x�����v�Z ===================( �I�� )


	// �J���[�v�Z ===========================================================( �J�n )

	// ���C�g�̃f�B�t���[�Y�J���[�����v�l�ɉ��Z
	lTotalDiffuse  += cfLight[ 0 ].Diffuse ;

	// ���C�g�̃A���r�G���g�J���[�����v�l�ɉ��Z
	lTotalAmbient  += cfLight[ 0 ].Ambient ;

	// �f�B�t���[�Y�J���[�p�x�����������v�l�ɉ��Z
	lTotalLightGen += lLightLitDest.y ;

	// �X�y�L�������C�g���v�l += �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[
	lTotalSpecular += lLightLitDest.z * cfLight[ 0 ].Specular ;

	// �X�y�L�����J���[�v�Z =================================================( �I�� )


	// �f�B���N�V���i�����C�g�̏��� *****************************************************( �I�� )





	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )







	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �o�̓f�B�t���[�Y�J���[ = ���C�g�f�B�t���[�Y�J���[�~�ϒl * �}�e���A���f�B�t���[�Y�J���[ + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
	VSOutput.Diffuse = lTotalDiffuse * cfMaterial.Diffuse + cfAmbient_Emissive ;

	// �A���t�@�̓f�B�t���[�Y�J���[�̃A���t�@�����̂܂܎g��
	VSOutput.Diffuse.w = cfMaterial.Diffuse.w ;

	// �o�̓X�y�L�����J���[ = ���C�g�X�y�L�����J���[�~�ϒl * �X�y�L�����J���[
	VSOutput.Specular = lTotalSpecular * cfMaterial.Specular ;

	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 1 ] ) ;

	// �g�D�[���e�N�X�`�����W�̓��C�g�̌�����
	VSOutput.ToonCoords0 = lTotalLightGen ;

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓p�����[�^��Ԃ�
	return VSOutput ;
}