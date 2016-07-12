#pragma once
#include<DxLib.h>
#include"Geometry.h"
#include<vector>
class Fragment
{
private:
	Vector2 _velocity;
	Vector2 _center;//これは破片の中心座標であって描画場所とかではない
	Vector2 _accel;
	float _angleVel;
public:
	DxLib::VERTEX* _headVert;
	Fragment();
	~Fragment();
	void CalculateCenter();
	void Fall();
	DxLib::VERTEX identityVert[3];//何もしてない（拡大とか回転とか）頂点情報を保存
	DxLib::VERTEX expansionVert[3];//拡大後の頂点情報
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
	//void SetUseDivGraphFlag(bool flag){ _breaking = flag; }//これ要らなかった　DxLibに定義済み
	void Break();
	void CreateVertices(int divx, int divy, int screenW, int screenH);
	void Capture();
	void CreateGraph(int handle);
	void Draw();
	void FragmentScalling();//破片の頂点を拡大
	void FragmentTranslation(Vector2 vec);

	void TurnUV();
	void SetPos(Vector2);
};