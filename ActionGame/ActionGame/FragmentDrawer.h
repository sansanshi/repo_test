#pragma once
#include<DxLib.h>
#include"Geometry.h"
#include<vector>
class Fragment
{
private:
	Vector2 _velocity;
	Vector2 _center;//‚±‚ê‚Í”j•Ğ‚Ì’†SÀ•W‚Å‚ ‚Á‚Ä•`‰æêŠ‚Æ‚©‚Å‚Í‚È‚¢
	Vector2 _accel;
	float _angleVel;
public:
	DxLib::VERTEX* _headVert;
	Fragment();
	~Fragment();
	void CalculateCenter();
	void Fall();
	DxLib::VERTEX identityVert[3];//‰½‚à‚µ‚Ä‚È‚¢iŠg‘å‚Æ‚©‰ñ“]‚Æ‚©j’¸“_î•ñ‚ğ•Û‘¶
	DxLib::VERTEX expansionVert[3];//Šg‘åŒã‚Ì’¸“_î•ñ
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
	//void SetUseDivGraphFlag(bool flag){ _breaking = flag; }//‚±‚ê—v‚ç‚È‚©‚Á‚½@DxLib‚É’è‹`Ï‚İ
	void Break();
	void CreateVertices(int divx, int divy, int screenW, int screenH);
	void Capture();
	void CreateGraph(int handle);
	void Draw();
	void FragmentScalling();//”j•Ğ‚Ì’¸“_‚ğŠg‘å
	void FragmentTranslation(Vector2 vec);

	void TurnUV();
	void SetPos(Vector2);
};