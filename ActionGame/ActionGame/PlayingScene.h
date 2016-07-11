#pragma once
#include"Scene.h"
#include"Player.h"
#include"GrabMan.h"
#include"Camera.h"
#include"EnemyFactory.h"
#include"EnemyBulletFactory.h"
#include"BlockFactory.h"
#include"Stage.h"
#include"FragmentDrawer.h"
class PlayingScene:public Scene
{
private:
	float _groundZero;
	Player _player;
	int _stageGrHandle;
	Camera _camera;
	EnemyFactory _enemyFac;
	BlockFactory _blockFac;
	Stage _stage;

	int _hpBarHandle;
	Rect _hpBarRect;

	//FragmentDrawer _fragDrawer;
	
public:
	PlayingScene();
	~PlayingScene();
	void Update();
	//bool HitCheck(GameObject*, GameObject*);
};

