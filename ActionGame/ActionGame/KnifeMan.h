#pragma once
#include "Enemy.h"

class Player;
class Camera;
class KnifeMan :
	public Enemy
{
private:
	int _walkFrame;

	void(KnifeMan::*_pFunc)();
	Vector2 _pos;
	Vector2 _velocity;
	int _handle;
	int _deadhandle;

	Player& _playerRef;

	void DeadUpdate();
	void NearUpdate();
	void FarUpdate();

	bool _isNear;//プレイヤーに近いか（NearUpdate）に入ったか
	bool _isDead;
	bool _isLeft;//反転するか

	Camera& _cameraRef;
	float _cOffsetX;


public:
	KnifeMan(Vector2 pos, int handle, int deadHandle, Player& player, Camera& camera);
	~KnifeMan();
	void Update();
	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);
	void Draw();
	void Kill();
	void ChangeState();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
	

};