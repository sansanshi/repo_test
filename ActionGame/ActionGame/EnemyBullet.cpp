#include "EnemyBullet.h"
#include<DxLib.h>
#include"Camera.h"
#include"Player.h"

EnemyBullet::EnemyBullet(Vector2 pos, Vector2 vel, int handle, Camera& camera) :_cameraRef(camera)
{
	_isAvailable = true;
	_pos = pos;
	_velocity = vel;
	_handle = handle;
	_collider = Collider(this, ct_knifeMan, col_attack);
	_collider.width = 32;
	_collider.height = 16;
	_collider.ToEnable();
	_isLeft = vel.x < 0.0f;
}


EnemyBullet::~EnemyBullet()
{
}

void
EnemyBullet::Update()
{
	_pos += _velocity;
	_collider.SetCenter_Cam(_pos, _cameraRef.OffsetX());
}

void
EnemyBullet::Draw()
{
	DrawCameraGraph(_pos.x, _pos.y, 0, 0, 8, 4, 4, 2, 3.0f, 0.0f, _handle, true, _isLeft);
	//_collider.Draw();
}

void
EnemyBullet::OnCollided(std::shared_ptr<GameObject> obj)
{

}
void
EnemyBullet::OnCollided(Collider* col)
{
	if (col->CharaType() == ct_player&&col->Type() == col_default)
	{
		Player* p = dynamic_cast<Player*>(col->_gameObject);
		p->Damage(10.0f);
		_collider.ToDisable();
		_isAvailable = false;
	}
}

void 
EnemyBullet::OnCollided(GameObject* obj)
{

}
void
EnemyBullet::Reject(Vector2 vec)
{

}

void
EnemyBullet::DrawCameraGraph(int x, int y, int srcX, int srcY, int width, int height, int cx, int cy, double extRate, double angle, int handle, int transFlag, int turnFlag)
{
	DrawRectRotaGraph2(x + _cameraRef.OffsetX(), y, srcX, srcY, width, height, cx, cy, extRate, angle, handle, transFlag, turnFlag);
}