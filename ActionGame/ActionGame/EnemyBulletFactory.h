#pragma once
#include<list>
#include<memory>
#include"EnemyBullet.h"

class Camera;
class EnemyBulletFactory
{
private:
	std::list<std::shared_ptr<EnemyBullet>> _enemyBullets;
	int _handle;
	Camera& _cameraRef;
public:
	EnemyBulletFactory(Camera&);
	~EnemyBulletFactory();

	std::shared_ptr<EnemyBullet> Create(Vector2 pos,Vector2 velocity);
	std::shared_ptr<EnemyBullet> Create(unsigned char et, Vector2& pos);
	//全体のアップデート
	void Update();
	std::list<std::shared_ptr<EnemyBullet>>& GetEnemyBullets(){ return _enemyBullets; };//enemiesの参照を返す

	void Draw();
};

