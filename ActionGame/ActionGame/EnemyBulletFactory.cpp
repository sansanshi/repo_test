#include "EnemyBulletFactory.h"
#include<DxLib.h>
#include"Camera.h"

EnemyBulletFactory::EnemyBulletFactory(Camera& camera) :_cameraRef(camera)
{
	_handle = LoadGraph("img/knife.png");
}


EnemyBulletFactory::~EnemyBulletFactory()
{
	DeleteGraph(_handle);
}

std::shared_ptr<EnemyBullet>
EnemyBulletFactory::Create(Vector2 pos,Vector2 vel)
{
	std::shared_ptr<EnemyBullet> ret;
	ret.reset(new EnemyBullet(pos,vel,_handle,_cameraRef));
	_enemyBullets.push_back(ret);
	return _enemyBullets.back();
}

void
EnemyBulletFactory::Update()
{
	int n;
	std::shared_ptr<EnemyBullet> temp;
	for (auto& ebullet : _enemyBullets)
	{
		ebullet->Update();
		bool b = ebullet->IsAvailable();
	}

	_enemyBullets.remove_if(
		[](std::shared_ptr<EnemyBullet>& e){//ƒ‰ƒ€ƒ_Ž®‚ªŽg‚í‚ê‚Ä‚¢‚é
		return !e->IsAvailable();
	});

}

void
EnemyBulletFactory::Draw()
{
	for (auto& ebullet : _enemyBullets)
	{
		ebullet->Draw();
	}
}