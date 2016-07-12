#pragma once
#include<DxLib.h>
#include"Geometry.h"
#include<vector>
class Fragment
{
private:
	Vector2 _velocity;
	Vector2 _center;//����͔j�Ђ̒��S���W�ł����ĕ`��ꏊ�Ƃ��ł͂Ȃ�
	Vector2 _accel;
	float _angleVel;
public:
	DxLib::VERTEX* _headVert;
	Fragment();
	~Fragment();
	void CalculateCenter();
	void Fall();
	DxLib::VERTEX identityVert[3];//�������ĂȂ��i�g��Ƃ���]�Ƃ��j���_����ۑ�
	DxLib::VERTEX expansionVert[3];//�g���̒��_���
	void Scalling();
	void SetCenter(Vector2);
	void Translation(Vector2 vec);
};


class BaseDrawer
{
private:

public:
	BaseDrawer(){};
	~BaseDrawer(){};
	BaseDrawer(const char* filePath){};
	BaseDrawer(int handle){};
	void Draw(){};
};

class FragmentDrawer:public BaseDrawer
{
private:
	bool _breaking;
	int _capHandle;

	std::vector<Fragment> _fragments;
	std::vector<DxLib::VERTEX> _vertices;
public:
	FragmentDrawer();
	~FragmentDrawer();
	FragmentDrawer(const char* filePath);
	FragmentDrawer(int handle);
	//void SetUseDivGraphFlag(bool flag){ _breaking = flag; }//����v��Ȃ������@DxLib�ɒ�`�ς�
	void Break();
	void CreateVertices(int divx, int divy, int screenW, int screenH);
	void Capture();
	void CreateGraph(int handle);
	void Draw();
	void FragmentScalling();//�j�Ђ̒��_���g��
	void FragmentTranslation(Vector2 vec);

	void TurnUV();
	void SetPos(Vector2);
};