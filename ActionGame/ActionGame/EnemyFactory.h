#pragma once


//Enemy��list��Enemy*�ɂȂ��Ă�Ƃ����S��std::shared_ptr<Enemy>�ɂ���@���Ƃ���ptr�Ȃ̂�*�͂��Ȃ�
//Enemy�̃|�C���^�����߂���Ƃ���ɂ�.get�ɂ���
//create�̂Ƃ�Enemy����Ԃ��Ă����Ƃ���(Enemy* ret=new ~~~���Ă��Ă�Ƃ����ret.reset(new ~~~~)�ɏ���������

/*//�s�v�Ȃ��̍폜���[�v������ɒu��������
_enmies.remove_if(
	[](std::shared_ptr<Enemy>& e){//�����_�����g���Ă���
	return !e->IsAvailable(); 
});*/

#include<vector>
#include"Geometry.h"
#include<map>
#include<algorithm>
#include<memory>
#include<list>


class Enemy;
class Player;
class Camera;

enum EnemyType{
	et_none,
	et_grabman,
	et_knifeman,
	et_batman,
};

class EnemyFactory
{
private:
	std::list<std::shared_ptr<Enemy>> _enemies;//�V�F�A�h�|�C���^�@���g���Q�Ƃ��Ă������ƂɎQ�ƃJ�E���^��+1�@0�ɂȂ�i�N���Q�Ƃ��Ă��Ȃ���ԁj�Ɣj�������

	std::map<EnemyType, int> _enemyImgMap;
	std::map<EnemyType, int> _enemydeadImgMap;

	Player& _playerRef;
	int _timer;
	Camera& _cameraRef;

public:
	EnemyFactory(  Player& player,Camera& camera);
	//EnemyFactory();
	~EnemyFactory();
	//�G�̐���
	std::shared_ptr<Enemy> Create(EnemyType et, Vector2& pos);
	std::shared_ptr<Enemy> Create(unsigned char et, Vector2& pos);
	//�S�̂̃A�b�v�f�[�g
	void Update();
	std::list<std::shared_ptr<Enemy>>& GetEnemies();//enemies�̎Q�Ƃ�Ԃ�
	void Draw();


};

