#include "EnemyBullet.h"
#include<DxLib.h>


EnemyBullet::EnemyBullet(Vector2 pos,Vector2 vel,int handle)
{
	_pos = pos;
	_velocity = vel;
	_handle = handle;
}


EnemyBullet::~EnemyBullet()
{
}

void
EnemyBullet::Update()
{

}

void
EnemyBullet::Draw()
{
	DrawRectRotaGraph2(_pos.x, _pos.y, 0, 0, 8, 4, 4, 2, 1.0f, 0.0f, _handle, true, _isLeft);
}

void
EnemyBullet::OnCollided(std::shared_ptr<GameObject> obj)
{

}
void
EnemyBullet::OnCollided(Collider* col)
{

}

void 
EnemyBullet::OnCollided(GameObject* obj)
{

}
void
EnemyBullet::Reject(Vector2 vec)
{

}