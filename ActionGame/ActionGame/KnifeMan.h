#pragma once
#include "Enemy.h"
#include<map>
#include"FragmentDrawer.h"
class EnemyBulletFactory;
class Player;
class Camera;
class KnifeMan :
	public Enemy
{
private:
	enum State
	{
		state_none,
		state_near,
		state_far,
		state_arial,
		state_underThrow,
		state_overThrow,
		state_preUnderThrow,
		state_preOverThrow,
		state_dead,
		state_wait,

	};

	int _walkFrame;
	State _state;
	typedef void(KnifeMan::*func_void)();
	std::map<KnifeMan::State, func_void> _pfuncMap;
	std::map<KnifeMan::State, func_void> _drawFuncMap;
	std::map<KnifeMan::State, int> _stateFrame;

	void(KnifeMan::*_pFunc)();
	void AliveUpdate();
	//Vector2 _pos;
	//Vector2 _velocity;
	Vector2 _acceleration;
	int _handle;
	int _deadhandle;

	int _hpMax;
	int _hp;

	Player& _playerRef;

	void DeadUpdate();
	void NearUpdate();
	void FarUpdate();
	void ArialUpdate();
	void OverThrowUpdate();
	void UnderThrowUpdate();
	void PreOverThrowUpdate();
	void PreUnderThrowUpdate();
	void WaitUpdate();


	void DrawNear();
	void DrawFar();
	void DrawArial();
	void DrawDead();
	void DrawOverThrow();
	void DrawUnderThrow();
	void DrawPreOverThrow();
	void DrawPreUnderThrow();
	void DrawWait();

	void ChangeState(KnifeMan::State);


	bool _isNear;//プレイヤーに近いか（NearUpdate）に入ったか
	bool _isGrabbing;//プレイヤーをつかんでいるか
	bool _isDead;
	bool _isLeft;//反転するか

	Camera& _cameraRef;
	float _cOffsetX;


	FragmentDrawer _fragDrawer;

	bool _prevRejectY;

	EnemyBulletFactory& _ebulletFac;
	Vector2 _overThrowOffset;
	Vector2 _underThrowOffset;

public:
	KnifeMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera,EnemyBulletFactory&);
	~KnifeMan();
	void Update();
	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);
	void Draw();
	void Kill();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);


	void Reject(Vector2);

	void Damage(int value);

	
};

