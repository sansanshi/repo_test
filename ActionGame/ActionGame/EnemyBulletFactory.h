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
	//�S�̂̃A�b�v�f�[�g
	void Update();
	std::list<std::shared_ptr<EnemyBullet>>& GetEnemyBullets(){ return _enemyBullets; };//enemies�̎Q�Ƃ�Ԃ�

	void Draw();
};

