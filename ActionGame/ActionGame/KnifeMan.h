#pragma once
#include "Enemy.h"
#include<map>
#include"FragmentDrawer.h"
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

};
class Player;
class Camera;
class KnifeMan :
	public Enemy
{
private:
	int _walkFrame;
	State _state;
	typedef void(KnifeMan::*func_void)();
	std::map<State, func_void> _pfuncMap;
	std::map<State, func_void> _drawFuncMap;
	std::map<State, int> _stateFrame;

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


	void DrawNear();
	void DrawFar();
	void DrawArial();
	void DrawDead();
	void DrawOverThrow();
	void DrawUnderThrow();
	void DrawPreOverThrow();
	void DrawPreUnderThrow();

	void ChangeState(State);


	bool _isNear;//プレイヤーに近いか（NearUpdate）に入ったか
	bool _isGrabbing;//プレイヤーをつかんでいるか
	bool _isDead;
	bool _isLeft;//反転するか

	Camera& _cameraRef;
	float _cOffsetX;


	FragmentDrawer _fragDrawer;

	bool _prevRejectY;

public:
	KnifeMan(Vector2 pos, int& handle, int& deadHandle, Player& player, Camera& camera);
	~KnifeMan();
	void Update();
	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);
	void Draw();
	void Kill();
	void ChangeState();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);


	void Reject(Vector2);

	void Damage(int value);

};

