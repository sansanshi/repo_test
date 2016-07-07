#include "EnemyBulletFactory.h"
#include<DxLib.h>


EnemyBulletFactory::EnemyBulletFactory()
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
	ret.reset(new EnemyBullet(pos,vel,_handle));
	_enemyBullets.push_back(ret);
	return _enemyBullets.back();
}

void
EnemyBulletFactory::Update()
{
	for (auto& ebullet : _enemyBullets)
	{
		ebullet->Update();
	}
}
