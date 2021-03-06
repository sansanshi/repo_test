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

	Rect rect;//穴開ける用のrect

	//FragmentDrawer _fragDrawer;

	int testHandle;
	int shaderHandle;
	int thirdscreen;
	VERTEX2DSHADER vertex[4];

	float _timer;
	float _timerAccel;

	FragmentDrawer _fragDrawer;

	int _stoppingTimer;//止まっているフレーム数
	int _stoppingLimit;//止められる時間
	
public:
	PlayingScene();
	~PlayingScene();
	void Update();

	void TimeBreak();
	void TimeStop();
	void TimeMove();
	//bool HitCheck(GameObject*, GameObject*);
	bool IsTimeStopping(){ return _timer >= 100.0f; }
	bool _isTimeBreaking;//停止空間が縮んでいる状態かどうか
	bool _isTimeStopping;//停止空間が広がっている状態かどうか
	bool _isTimeStop;//完全に停止空間が広がりきったかどうか
	bool _isTimeMove;//完全に停止空間が縮みきったかどうか
};

