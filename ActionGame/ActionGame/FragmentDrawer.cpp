#include "FragmentDrawer.h"
#include <math.h>
#include<assert.h>

Fragment::Fragment() {

	_headVert = NULL;
	_velocity.x = 0.0f;
	_velocity.y = -3.0f - 0.1f*static_cast<float>(rand() % 20 - 10);
	_center.x = 0.f;
	_center.y = 0.f;
	_accel.x = 0.04f*static_cast<float>(rand() % 21 - 10) ;//rand()%20-10=-10~9
	_accel.y = 0.3f;
	_angleVel = 0.0015f*static_cast<float>(rand() % 20 - 10);
	_fallTimer = 0;
}

Fragment::~Fragment()
{

}

void
Fragment::CalculateCenter(){
	this->_center.x = (_headVert->x + (_headVert + 1)->x + (_headVert + 2)->x) / 3.f;//3�_�̕��ς�����Ē��S�����߂Ă�
	this->_center.y = (_headVert->y + (_headVert + 1)->y + (_headVert + 2)->y) / 3.f;
}
void
Fragment::Fall(){
	Vector2 temp[3];
	++_fallTimer;

	_fallTimer>5 ? _velocity.x += _accel.x:0;
	_fallTimer > 5 ? _velocity.y += _accel.y:0;
	//�܂���](�d�S���S)
	//���s�ړ��Ɏn�܂蕽�s�ړ��ɏI���()
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//����vert��input/output�݂����Ȉ����ɂ��ĉ���v�𓮂����čŌ��vert=v�Ƃ����Ă݂�//�����ɓ����Ă�������vert�͊g�傳��Ă�͂�
		DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		vert.x -= _center.x;//���_���S�ɂȂ�悤�ɕ��s�ړ�
		vert.y -= _center.y;
		//v.x -= _center.x;
		//v.y -= _center.y;
		//���̕ӂŃX�P�[����1�ɏo����΂�����
		//���̏�Ԃ�vert�̏�񂪂ق�����identityVert�Ɏ�ꂽ���ۂ�
		
		//�R�u���̃}�V���̓T�C�R�K��
		//v.x = v.x*5.0f + v.y*0.f;//�g��v�Z//���t���[���g�傪�������Ă���ۂ�
		//v.y = v.x*0.0f + v.y*5.0f;
		float x = vert.x*cos(_angleVel) - vert.y*sin(_angleVel);//��]�v�Z
		float y = vert.x*sin(_angleVel) + vert.y*cos(_angleVel);//������v.x�g���Ă�̂ɏ��v.x=~~���Ă��Ă�̂�������
		//v.x *= 2.0f;
		//v.y *= 2.0f;
		//v.x += _center.x;
		//v.y += _center.y;
		
		vert.x = x;//�v�Z�ɂ���ċ��߂���]����W�Ɉړ�
		vert.y = y;
		//vert = v;
		vert.x *= 0.985f;//���������������Ă�H
		vert.y *= 0.985f;
		vert.x += _center.x;//���_���S�ɂ��Ă��̂Ō��̈ʒu�ɖ߂�
		vert.y += _center.y;
		
		//vert.x = expansionVert[i].x + x;
		//vert.y = expansionVert[i].y + y;

		//����
		_fallTimer>3 ? vert.x += _velocity.x : 0 ;
		_fallTimer>3 ? vert.y += _velocity.y : 0 ;
		//vert.x = 200 + expansionVert[i].x;//expansionVert�͑�������Ȃ� �����ɓ����Ă����Ƃ���vert��expansionVert�Ɠ���
		//vert.y = 200 + expansionVert[i].y;

		//�����xUP
		vert.a -= 1;

		temp[i].x = vert.x;
		temp[i].y = vert.y;

	}
	CalculateCenter();

	for (int i = 0; i < 3; i++){
		//DrawLine(temp[i].x, temp[i].y, temp[(i + 1)%3].x, temp[(i + 1)%3].y, 0xffffff);
	}
	//for (int i = 0; i < 3; i++){//�����͂ł��Ă���ۂ�
	//	DxLib::VERTEX v = identityVert[i];
	//	DxLib::VERTEX v_ = identityVert[(i+1)%3];
	//	DrawLine(v.x, v.y, v_.x, v_.y, 0xffffff);
	//}
}
void
Fragment::Scalling(float size)//���t���[������Ă�ł�Ɠ����Ȃ��i���ō��W��M���Ă�̂�
{//���̒���identityVert���g���Čv�Z���Ă�̂őO��Translation�̌��ʂ����f����ĂȂ�
	Vector2 temp[3];

	_velocity.x += _accel.x;
	_velocity.y += _accel.y;
	//�܂���](�d�S���S)
	//���s�ړ��Ɏn�܂蕽�s�ړ��ɏI���()
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//����vert��input/output�݂����Ȉ����ɂ��ĉ���v�𓮂����čŌ��vert=v�Ƃ����Ă݂�
		DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		//vert.x -= _center.x;//���_���S�ɂȂ�悤�ɕ��s�ړ�
		//vert.y -= _center.y;
		v.x -= _center.x;
		v.y -= _center.y;
		//���̕ӂŃX�P�[����1�ɏo����΂�����
		//���̏�Ԃ�vert�̏�񂪂ق�����identityVert�Ɏ�ꂽ���ۂ�

		//�R�u���̃}�V���̓T�C�R�K��
		//float tempx = identityVert[i].x - _center.x;
		//float tempy = identityVert[i].y - _center.y;
		float x = v.x*size + v.y*0.f;//�g��v�Z//���t���[���g�傪�������Ă���ۂ�
		float y = v.x*0.0f + v.y*size;
		
		x += _center.x*size;//�����ŋl�܂��Ă��@�g���ɂ͒��S���W*�g��{�����{����
		y += _center.y*size;

		vert.x = x;//�v�Z�ɂ���ċ��߂���]����W�Ɉړ�
		vert.y = y;
		//vert = v;
		//vert.x *= 0.985f;//���������������Ă�H
		//vert.y *= 0.985f;
		//vert.x += _center.x;//���_���S�ɂ��Ă��̂Ō��̈ʒu�ɖ߂�
		//vert.y += _center.y;
		expansionVert[i] = vert;
		

		temp[i].x = vert.x;
		temp[i].y = vert.y;

	}
	CalculateCenter();

	/*for (int i = 0; i < 3; i++){
	DrawLine(temp[i].x, temp[i].y, temp[(i + 1)%3].x, temp[(i + 1)%3].y, 0xffffff);
	}*/
	//for (int i = 0; i < 3; i++){//�����͂ł��Ă���ۂ�
	//	DxLib::VERTEX v = identityVert[i];
	//	DxLib::VERTEX v_ = identityVert[(i + 1) % 3];
	//	DrawLine(v.x, v.y, v_.x, v_.y, 0xffffff);
	//}
}

FragmentDrawer::FragmentDrawer(const char* filePath) : BaseDrawer(filePath),//�t�@�C���p�X������炻�̉摜�Ńo���o���ɂ��邽�߂̉摜���쐬�i_capHandle�ɓ����j
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}
FragmentDrawer::FragmentDrawer(int handle) : BaseDrawer(handle),//�t�@�C���p�X������炻�̉摜�Ńo���o���ɂ��邽�߂̉摜���쐬�i_capHandle�ɓ����j
_breaking(false),
_capHandle(0)
{
	SetUseDivGraphFlag(FALSE);
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	CreateVertices(3, 4, sizeX, sizeY);
	_capHandle = handle;
}

FragmentDrawer::FragmentDrawer(void) : BaseDrawer(), _breaking(false), _capHandle(0)//����ǂ�������Ă΂�Ă邩�������
{//�������@Grabman�̃R���X�g���N�^��FragmentDrawer�̎��̂��`���Ă�̂�Grabman�����ꂽ���_��FragmentDrawer�̃f�t�H���g�R���X�g���N�^������
	SetUseDivGraphFlag(FALSE);
	CreateVertices(8, 6, 640, 480);
	_capHandle = MakeGraph(640, 480);
}//Grabman�̃R���X�g���N�^�ŏ������q���g�����|�C���^�^�Œ�`���邱�Ƃŉ���



FragmentDrawer::~FragmentDrawer(void)
{
	SetUseDivGraphFlag(TRUE);
}

void
FragmentDrawer::Break(){
	_breaking = true;
}
void
FragmentDrawer::Break(Vector2 power)
{
	_breaking = true;
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->AddPower(power);
		it++;
	}
}
void
Fragment::AddPower(Vector2 power)
{
	_velocity += power;
}

void
FragmentDrawer::Capture(){
	int r = DxLib::GetDrawScreenGraph(0, 0, 640, 480, _capHandle);
}
void
FragmentDrawer::CreateGraph(int handle)//�ꉞ��������ǃR���X�g���N�^��_capHandle�ݒ肵�Ă邩��g��Ȃ�����
{
	_capHandle = handle;
}

void
FragmentDrawer::Draw(){

	//�j�Е`��
	if (_breaking){
		DxLib::DrawPolygon(&_vertices[0], _vertices.size() / 3, _capHandle, true, true);
		std::vector<Fragment>::iterator it = _fragments.begin();
		for (; it != _fragments.end(); ++it){
			it->Fall();
			//it->Scalling();
		}

	}
	else{
		BaseDrawer::Draw();
	}
}


void
FragmentDrawer::CreateVertices(int divx, int divy, int screenW, int screenH){
	_fragments.resize(divx*divy * 2);//�l�p�`���O�p�`�ɕ������Ă�̂�fragment�i�O�p�`�̌��ЁH�j�̐��͕������i�l�p�`�̐��j*2
	_vertices.resize(divx*divy * 6);//�l�p�`��2�̎O�p�`�ɂȂ������l�p�`1�Œ��_6�ɂȂ��Ă���̂�*6
	for (int j = 0; j<divy; ++j){
		float top = (screenH / divy)*j;
		float bottom = (screenH / divy)*(j + 1);
		float topv = (1.f / divy)*static_cast<float>(j);
		float bottomv = (1.f / divy)*static_cast<float>(j + 1);
		for (int i = 0; i<divx; ++i){
			float left = (screenW / divx)*static_cast<float>(i);
			float right = (screenW / divx)*static_cast<float>(i + 1);
			float leftu = (1.f / divx)*static_cast<float>(i);
			float rightu = (1.f / divx)*static_cast<float>(i + 1);

			int headidx = (i + divx*j) * 6;

			//�㔼��
			_vertices[headidx + 0].r = _vertices[headidx + 0].g = _vertices[headidx + 0].b = _vertices[headidx + 0].a = 255;
			_vertices[headidx + 0].g = _vertices[headidx + 0].b = 0;//�����ɒ��_�J���[��Ԃɂ��Ă݂�
			_vertices[headidx + 0].x = left; _vertices[headidx + 0].y = top;
			_vertices[headidx + 0].u = leftu; _vertices[headidx + 0].v = topv;
			_vertices[headidx + 1].r = _vertices[headidx + 1].g = _vertices[headidx + 1].b = _vertices[headidx + 1].a = 255;
			//_vertices[headidx + 1].g = _vertices[headidx + 1].b = 0;
			_vertices[headidx + 1].x = right; _vertices[headidx + 1].y = top;
			_vertices[headidx + 1].u = rightu; _vertices[headidx + 1].v = topv;
			_vertices[headidx + 2].r = _vertices[headidx + 2].g = _vertices[headidx + 2].b = _vertices[headidx + 2].a = 255;
			//_vertices[headidx + 2].g = _vertices[headidx + 2].b = 0;
			_vertices[headidx + 2].x = left; _vertices[headidx + 2].y = bottom;
			_vertices[headidx + 2].u = leftu; _vertices[headidx + 2].v = bottomv;
			int fragHeadIdx = (i + divx*j) * 2;//�㔼�����������ŁA1���̏㔼���́{1�ł͂Ȃ��̂Ł�2
			_fragments[fragHeadIdx]._headVert = &_vertices[headidx + 0];
			_fragments[fragHeadIdx].CalculateCenter();
			for (int i = 0; i < 3; i++)//�������ĂȂ����_����ۑ��e�X�g
			{
				_fragments[fragHeadIdx].identityVert[i] = _vertices[headidx + 0 + i];
			}

			//������
			_vertices[headidx + 3].r = _vertices[headidx + 3].g = _vertices[headidx + 3].b = _vertices[headidx + 3].a = 255;
			_vertices[headidx + 3].g = _vertices[headidx + 3].b = 0;//���_�J���[��Ԃ�
			_vertices[headidx + 3].x = right; _vertices[headidx + 3].y = top;
			_vertices[headidx + 3].u = rightu; _vertices[headidx + 3].v = topv;
			_vertices[headidx + 4].r = _vertices[headidx + 4].g = _vertices[headidx + 4].b = _vertices[headidx + 4].a = 255;
			_vertices[headidx + 4].x = left; _vertices[headidx + 4].y = bottom;
			_vertices[headidx + 4].u = leftu; _vertices[headidx + 4].v = bottomv;
			_vertices[headidx + 5].r = _vertices[headidx + 5].g = _vertices[headidx + 5].b = _vertices[headidx + 5].a = 255;
			_vertices[headidx + 5].g = _vertices[headidx + 5].b = 0;//���_�J���[��Ԃ�
			_vertices[headidx + 5].x = right; _vertices[headidx + 5].y = bottom;
			_vertices[headidx + 5].u = rightu; _vertices[headidx + 5].v = bottomv;
			_fragments[fragHeadIdx + 1]._headVert = &_vertices[headidx + 3];
			_fragments[fragHeadIdx + 1].CalculateCenter();
			for (int i = 0; i < 3; i++)//�������ĂȂ����_����ۑ��e�X�g
			{
				_fragments[fragHeadIdx+1].identityVert[i] = _vertices[headidx + 3 + i];
			}

		}
	}
}

void
FragmentDrawer::FragmentScalling(float size)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->Scalling(size);
		it++;
	}
}

void
FragmentDrawer::TurnUV()
{
	std::vector<DxLib::VERTEX>::iterator it = _vertices.begin();
	for (; it != _vertices.end();)
	{
		it->u = static_cast<float>(1.0f - it->u);
		it++;
	}
}

void
FragmentDrawer::SetPos(Vector2 pos)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->SetCenter(pos);
		it++;
	}
}

void
Fragment::SetCenter(Vector2 pos)
{
	_center += pos;
}

void 
FragmentDrawer::FragmentTranslation(Vector2 vec)
{
	std::vector<Fragment>::iterator it = _fragments.begin();
	for (; it != _fragments.end();)
	{
		it->Translation(vec);
		it++;
	}
}

void
Fragment::Translation(Vector2 pos)
{
	for (int i = 0; i<3; ++i){
		DxLib::VERTEX& vert = *(_headVert + i);//����vert��input/output�݂����Ȉ����ɂ��ĉ���v�𓮂����čŌ��vert=v�Ƃ����Ă݂�//�����ɓ����Ă�������vert�͊g�傳��Ă�͂�
		//DxLib::VERTEX v = identityVert[i];//*(_headVert+i);

		vert.x -= _center.x;//pos�̈ʒu�ɍ��W��ݒ肷��֐��Ȃ̂ň�U���S��0,0�Ɏ����Ă���
		vert.y -= _center.y;
		
		
		vert.x += pos.x+_center.x;//���_���S�ɂ��Ă��̂Ō��̈ʒu�ɖ߂�
		vert.y += pos.y + _center.y;

	
	}
	CalculateCenter();
	Vector2 temp;
	temp = _center;
	int a;
	
}