#pragma once
#include<DxLib.h>
#include"Geometry.h"
#include<vector>
class Fragment
{
private:
	Vector2 _velocity;
	Vector2 _center;
	Vector2 _accel;
	float _angleVel;
public:
	DxLib::VERTEX* _headVert;
	Fragment();
	~Fragment();
	void CalculateCenter();
	void Fall();
};


class BaseDrawer
{
private:

public:
	BaseDrawer(){};
	~BaseDrawer(){};
	BaseDrawer(const char* filePath){};
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
	void SetUseDivGraphFlag(bool flag){ _breaking = flag; }
	void Break();
	void CreateVertices(int divx, int divy, int screenW, int screenH);
	void Capture();
	void Draw();
};