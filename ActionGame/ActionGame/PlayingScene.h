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
#include"EnemyBulletFactory.h"
#include"EffectFactory.h"
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
	EnemyBulletFactory _ebulletFac;
	EffectFactory _effectFac;

	int _hpBarHandle;
	Rect _hpBarRect;

	Rect rect;//ŒŠŠJ‚¯‚é—p‚Ìrect

	//FragmentDrawer _fragDrawer;

	int testHandle;
	int shaderHandle;
	int thirdscreen;
	VERTEX2DSHADER vertex[4];

	float _timer;
	float _timerAccel;
	
public:
	PlayingScene();
	~PlayingScene();
	void Update();
	//bool HitCheck(GameObject*, GameObject*);
};

