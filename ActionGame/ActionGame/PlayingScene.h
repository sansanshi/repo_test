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

	FragmentDrawer _fragDrawer;

	int _stoppingTimer;//Ž~‚Ü‚Á‚Ä‚¢‚éƒtƒŒ[ƒ€”
	int _stoppingLimit;//Ž~‚ß‚ç‚ê‚éŽžŠÔ
	
public:
	PlayingScene();
	~PlayingScene();
	void Update();

	void TimeBreak();
	void TimeStop();
	void TimeMove();
	//bool HitCheck(GameObject*, GameObject*);
	bool IsTimeStopping(){ return _timer >= 100.0f; }
	bool _isTimeBreaking;//’âŽ~‹óŠÔ‚ªk‚ñ‚Å‚¢‚éó‘Ô‚©‚Ç‚¤‚©
	bool _isTimeStopping;//’âŽ~‹óŠÔ‚ªL‚ª‚Á‚Ä‚¢‚éó‘Ô‚©‚Ç‚¤‚©
	bool _isTimeStop;//Š®‘S‚É’âŽ~‹óŠÔ‚ªL‚ª‚è‚«‚Á‚½‚©‚Ç‚¤‚©
	bool _isTimeMove;//Š®‘S‚É’âŽ~‹óŠÔ‚ªk‚Ý‚«‚Á‚½‚©‚Ç‚¤‚©
};

