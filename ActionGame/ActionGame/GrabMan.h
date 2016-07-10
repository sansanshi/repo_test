#pragma once
#include "Enemy.h"
#include<map>
enum State
{
	state_none,
	state_near,
	state_far,
	state_grab,
	state_arial,
	state_dead,

};
class Player;
class Camera;
class GrabMan :
	public Enemy
{
private:
	int _walkFrame;
	State _state;
	typedef void(GrabMan::*func_void)();
	std::map<State, func_void> _pfuncMap;
	std::map<State, func_void> _drawFuncMap;
	std::map<State, int> _stateFrame;
	
	void(GrabMan::*_pFunc)();
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
	void GrabUpdate();
	void ArialUpdate();

	void DrawNear();
	void DrawFar();
	void DrawGrab();
	void DrawArial();
	void DrawDead();

	void ChangeState(State);


	bool _isNear;//プレイヤーに近いか（NearUpdate）に入ったか
	bool _isGrabbing;//プレイヤーをつかんでいるか
	bool _isDead;
	bool _isLeft;//反転するか

	Camera& _cameraRef;
	float _cOffsetX;

	int _shakedCnt;

public:
	GrabMan(Vector2 pos,int& handle,int& deadHandle,Player& player,Camera& camera);
	~GrabMan();
	void Update();
	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);
	void Draw();
	void Kill();
	void ChangeState();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
	void Shaked();//掴み状態でプレイヤーにシェイクされた

	bool IsGrabbing(){ return _isGrabbing; }
	void Reject(Vector2);

	void Damage(int value);
};

