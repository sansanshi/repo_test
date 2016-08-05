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

	Rect rect;//���J����p��rect

	//FragmentDrawer _fragDrawer;

	int testHandle;
	int shaderHandle;
	int thirdscreen;
	VERTEX2DSHADER vertex[4];

	float _timer;
	float _timerAccel;

	FragmentDrawer _fragDrawer;

	int _stoppingTimer;//�~�܂��Ă���t���[����
	int _stoppingLimit;//�~�߂��鎞��
	
public:
	PlayingScene();
	~PlayingScene();
	void Update();

	void TimeBreak();
	void TimeStop();
	void TimeMove();
	//bool HitCheck(GameObject*, GameObject*);
	bool IsTimeStopping(){ return _timer >= 100.0f; }
	bool _isTimeBreaking;//��~��Ԃ��k��ł����Ԃ��ǂ���
	bool _isTimeStopping;//��~��Ԃ��L�����Ă����Ԃ��ǂ���
	bool _isTimeStop;//���S�ɒ�~��Ԃ��L���肫�������ǂ���
	bool _isTimeMove;//���S�ɒ�~��Ԃ��k�݂��������ǂ���
};

