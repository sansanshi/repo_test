#pragma once
#include "Enemy.h"
#include<map>
#include"FragmentDrawer.h"
class EnemyBulletFactory;
class Player;
class Camera;
class BatMan :
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
	typedef void(BatMan::*func_void)();
	std::map<BatMan::State, func_void> _pfuncMap;
	std::map<BatMan::State, func_void> _drawFuncMap;
	std::map<BatMan::State, int> _stateFrame;

	void(BatMan::*_pFunc)();
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

	void ChangeState(BatMan::State);


	bool _isNear;//�v���C���[�ɋ߂����iNearUpdate�j�ɓ�������
	bool _isGrabbing;//�v���C���[������ł��邩
	bool _isDead;
	bool _isLeft;//���]���邩

	Camera& _cameraRef;
	float _cOffsetX;


	FragmentDrawer _fragDrawer;

	bool _prevRejectY;

	Vector2 _overThrowOffset;
	Vector2 _underThrowOffset;

	Vector2 _attackOffset;
	Vector2 _attackOffset_under;
	float _attackRange;

public:
	BatMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera);
	~BatMan();
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

