#pragma once
#include "Enemy.h"

class Player;
class Camera;
class GrabMan :
	public Enemy
{
private:
	int _walkFrame;

	void(GrabMan::*_pFunc)();
	void AliveUpdate();
	Vector2 _pos;
	Vector2 _velocity;
	int _handle;
	int _deadhandle;

	Player& _playerRef;

	void DeadUpdate();
	void NearUpdate();
	void FarUpdate();
	void GrabUpdate();

	bool _isNear;//�v���C���[�ɋ߂����iNearUpdate�j�ɓ�������
	bool _isGrab;//�v���C���[������ł��邩
	bool _isDead;
	bool _isLeft;//���]���邩

	Camera& _cameraRef;
	float _cOffsetX;

	int _shakedCnt;

public:
	GrabMan(Vector2 pos,int handle,int deadHandle,Player& player,Camera& camera);
	~GrabMan();
	void Update();
	void OnCollided(Collider*);
	void OnCollided(GameObject*);
	void OnCollided(std::shared_ptr<GameObject>);
	void Draw();
	void Kill();
	void ChangeState();
	void DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag);
	void Shaked();//�͂ݏ�ԂŃv���C���[�ɃV�F�C�N���ꂽ
};

